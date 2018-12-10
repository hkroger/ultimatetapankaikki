#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "NET.H"
#include "NETDEFS.H"
#include "DEFINES.H"
#include "SDL_net.h"

#include <algorithm>
#include <iterator>
#include <vector>

TCPsocket server_socket;
SDLNet_SocketSet set;
std::vector<TCPsocket> sockets(MAX_PLAYERS, nullptr);

namespace Net
{

bool init()
{
    if (SDLNet_Init() == -1)
    {
        printf("Error initializing net API: %s\n", SDLNet_GetError());
        return false;
    }

    set = SDLNet_AllocSocketSet(MAX_PLAYERS);

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

int connect_server(const unsigned int port)
{
    IPaddress ip;
    // TODO: currently server finder works only for
    // localhost - UI selecting the remote is missing.
    const char* server = "127.0.0.1";

    if (SDLNet_ResolveHost(&ip, server, port) == -1)
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
            printf("Server is full\n");
        }
    }
}

static void disconnect(const unsigned int index)
{
    // Disconnect is not propagated up but application
    // relies on IPX_ALIVE keep-alive messages.
    SDLNet_TCP_DelSocket(set, sockets[index]);
    sockets[index] = nullptr;
}

bool receive(struct packet *rec[RECEIVERS], const unsigned int index)
{
    if (index >= sockets.size() || sockets[index] == nullptr) return false;

    if (SDLNet_CheckSockets(set, 0))
    {
        if (SDLNet_SocketReady(sockets[index]))
        {
            if (SDLNet_TCP_Recv(sockets[index], rec[index]->data, MAXDATASIZE))
            {
                rec[index]->header.src.node = index;
                return true;
            }
            else
            {
                disconnect(index);
            }
        }
    }

    return false;
}

struct nodeaddr BROADCAST()
{
    return nodeaddr{ BROADCAST_ID };
}

void sendpacket(struct nodeaddr destnode, struct packet *packetet, word len)
{
    const int datalength = len - 1;
    if (destnode.node == BROADCAST_ID)
    {
        for (auto socket : sockets)
        {
            if (socket != nullptr)
            {
                SDLNet_TCP_Send(socket, packetet->data, datalength);
            }
        }
    }
    else if (sockets[destnode.node] != nullptr)
    {
        if (SDLNet_TCP_Send(sockets[destnode.node], packetet->data, datalength) != datalength)
        {
            disconnect(destnode.node);
        }
    }
}
}
