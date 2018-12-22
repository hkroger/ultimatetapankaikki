#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NET.H"
#include "../ERROR/ERROR.H"
#include "NETDEFS.H"
#include "DEFINES.H"
#include "SDL_net.h"

#include <algorithm>
#include <iterator>
#include <vector>


namespace
{
// Add few extra sockets in case clients try to join when server is full. Game
// logic rejects such join attempts
static const int NUMBER_OF_SOCKETS = MAX_PLAYERS + 3;

static TCPsocket server_socket;
static SDLNet_SocketSet set;
static std::vector<TCPsocket> sockets(NUMBER_OF_SOCKETS, nullptr);
static std::vector<std::vector<char>> socket_storages(NUMBER_OF_SOCKETS);

static const int PACKET_HEADER_SIZE = 2; // 2 length bytes (16-bit) at the beginning
static const int PACKET_CHECKSUM_SIZE = 1; // 1 checksum byte at the and
static const int PACKET_OVERHEAD_SIZE = PACKET_HEADER_SIZE + PACKET_CHECKSUM_SIZE;
}

namespace Net
{

bool init()
{
    if (SDLNet_Init() == -1)
    {
        printf("Error initializing net API: %s\n", SDLNet_GetError());
        return false;
    }

    set = SDLNet_AllocSocketSet(NUMBER_OF_SOCKETS);

    if (!set)
    {
        printf("Error allocating socket set: %s\n", SDLNet_GetError());
        return false;
    }

    return true;
}

void deinit()
{
    if (set)
    {
        SDLNet_FreeSocketSet(set);
        set = NULL;
    }

    SDLNet_Quit();
}

int create_server(const unsigned int port)
{
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, NULL, port) == -1)
    {
        printf("Error resolving host: %s\n", SDLNet_GetError());
        return 1;
    }

    server_socket = SDLNet_TCP_Open(&ip);

    if (server_socket == NULL)
    {
        printf("Error opening socket: %s\n", SDLNet_GetError());
        return 2;
    }

    return 0;
}

int connect_server(const char* server_address, const unsigned int port)
{
    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, server_address, port) == -1)
    {
        printf("Error resolving host: %s\n", SDLNet_GetError());
        return 1;
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);

    if (!socket)
    {
        printf("Error opening socket: %s\n", SDLNet_GetError());
        return 2;
    }

    // Client always has just one socket to server
    sockets[0] = socket;
    SDLNet_TCP_AddSocket(set, socket);

    return 0;
}

void close_connections()
{
    if (server_socket)
    {
        SDLNet_TCP_Close(server_socket);
        server_socket = NULL;
    }

    for (auto socket : sockets)
    {
        if (socket != nullptr)
        {
            SDLNet_TCP_DelSocket(set, socket);
            SDLNet_TCP_Close(socket);
        }
    }

    std::fill(sockets.begin(), sockets.end(), nullptr);

    for (auto& storage : socket_storages)
    {
        storage.clear();
    }
}

void accept_connections()
{
    TCPsocket newClient = SDLNet_TCP_Accept(server_socket);

    if (newClient)
    {
        auto empty_slot = std::find(sockets.begin(), sockets.end(), nullptr);

        if (empty_slot != sockets.end())
        {
            *empty_slot = newClient;
            SDLNet_TCP_AddSocket(set, newClient);
        }
        else
        {
            SDLNet_TCP_Close(newClient);
            printf("Out of sockets\n");
        }
    }
}

static void disconnect(const unsigned int index)
{
    // Disconnect is not propagated up but server
    // relies on ALIVE keep-alive messages. It
    // then propagates disconnects to clients with
    // PLAYERDISCONNECTED message.
    SDLNet_TCP_DelSocket(set, sockets[index]);
    sockets[index] = nullptr;
    socket_storages[index].clear();
}

// Very simple checksum for packet sanity check
static char checksum(const char* data, word len)
{
    char c = (char)0xAA;
    for (int i = 0; i < len; ++i)
    {
        c ^= data[i];
    }
    return c;
}

static std::vector<char> get_packet_envelope(const char* data, word len)
{
    std::vector<char> packet;
    packet.push_back((char)(len >> 8));
    packet.push_back((char)(len));
    for (int i = 0; i < len; ++i)
    {
        packet.push_back(data[i]);
    }
    packet.push_back(checksum(data, len));
    return packet;
}

static int get_payload_length(const char* data)
{
    return ((int)data[0] << 8) + (data[1] & 0xFF);
}

static void receive_to_storage(const unsigned int index)
{
    if (index >= sockets.size() || sockets[index] == nullptr) return;

    if (SDLNet_CheckSockets(set, 0))
    {
        if (SDLNet_SocketReady(sockets[index]))
        {
            char data[MAXDATASIZE + PACKET_OVERHEAD_SIZE];
            const int len = SDLNet_TCP_Recv(sockets[index], data, MAXDATASIZE);

            if (len > 0)
            {
                auto& buffer = socket_storages[index];
                for (int i = 0; i < len; ++i)
                {
                    buffer.push_back(data[i]);
                }
            }
            else
            {
                disconnect(index);
            }
        }
    }
}

static bool find_next_packet_from_storage(struct packet *buffer)
{
    // Clients always have just one socket to server
    for (unsigned int i = 0; i < (server_socket ? socket_storages.size() : 1); ++i)
    {
        auto& storage = socket_storages[i];

        // Check if there is enough data for packet consisting
        // of 1 or more payload bytes
        if (storage.size() >= PACKET_OVERHEAD_SIZE + 1)
        {
            const unsigned int payload_length = get_payload_length(storage.data());

            if (storage.size() < payload_length + PACKET_OVERHEAD_SIZE)
            {
                // Not enough data in the storage, i.e. packet not yet fully received
                continue;
            }

            const char received_checksum = storage.data()[PACKET_HEADER_SIZE + payload_length];
            const char calculated_checksum = checksum(storage.data() + PACKET_HEADER_SIZE, payload_length);

            if (received_checksum == calculated_checksum)
            {
                // Packet fully received, return it to the app
                memcpy(buffer->data, storage.data() + PACKET_HEADER_SIZE, payload_length);
                buffer->header.src.node = i;

                // Remove packet from storage
                storage.erase(storage.begin(), storage.begin() + payload_length + PACKET_OVERHEAD_SIZE);

                return true;
            }
            else
            {
                warn( "Corrupted packet\n" );
                storage.clear();
            }
        }
    }

    return false;
}

static unsigned int get_next_socket_to_receive()
{
    if (server_socket)
    {
        static unsigned int socket_index = 0;
        // Loops 0 to NUMBER_OF_SOCKETS - 1
        return socket_index++ % NUMBER_OF_SOCKETS;
    }
    else
    {
        return 0;
    }
}

bool receive(struct packet *buffer)
{
    // Receive data evenly from sockets and store any received
    // data to storage
    receive_to_storage(get_next_socket_to_receive());

    // Look through the socket storages to see if there are
    // any fully received packets. Return one if finding
    // any.
    return find_next_packet_from_storage(buffer);
}

struct nodeaddr BROADCAST()
{
    return nodeaddr{ BROADCAST_ID };
}

void sendpacket(struct nodeaddr destnode, struct packet *packetet, word len)
{
    const int datalength = len - 1;
    auto packet = get_packet_envelope(packetet->data, datalength);

    if (destnode.node == BROADCAST_ID)
    {
        for (unsigned int i = 0; i < sockets.size(); ++i)
        {
            auto& socket = sockets[i];
            if (socket != nullptr)
            {
                if (SDLNet_TCP_Send(socket, packet.data(), (int)packet.size()) != packet.size())
                {
                    disconnect(i);
                }
            }
        }
    }
    else if (sockets[destnode.node] != nullptr)
    {
        if (SDLNet_TCP_Send(sockets[destnode.node], packet.data(), (int)packet.size()) != packet.size())
        {
            disconnect(destnode.node);
        }
    }
}
}
