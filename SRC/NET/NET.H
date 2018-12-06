#ifndef __NET_H__
#define __NET_H__

#define RECEIVERS 16
#define MAXDATASIZE 1024
#define BROADCAST_ID 0xFF

#define word unsigned short

struct nodeaddr
{
  unsigned int node;
};

struct netaddress
{
  struct nodeaddr node;
};

struct header
{
  struct nodeaddr src;
};

struct packet
{
  struct header header;
  char data[MAXDATASIZE];
};

int initnet();
void deinitnet();

int createsocket(const unsigned int port);
int connectsocket(const unsigned int port);

void closesockets();

void acceptconnections();
bool receive(struct packet *rec[RECEIVERS], const unsigned int index);

void sendpacket(struct nodeaddr destnode, struct packet *packetet, word len);
struct nodeaddr BROADCAST();

#endif
