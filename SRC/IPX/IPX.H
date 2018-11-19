// IPX EXAMPLE FOR WATCOM C/C++ (10.0a) UNDER DOS4GW PMODE DPMI.
// Written by Daniel Jeppson d94dj@efd.lth.se 960511 03:28 AM (8

// You are hearby granted the right to use this code freely but
// I would appreciate a credit or greet if you find it useful...

// Please see the readme.txt and chat.c for more information...

#ifndef __IPX_H_
#define __IPX_H_

#define byte unsigned char  // commonly defined ever.. (8 
#define word unsigned short
#define dword unsigned long
#define MAXDATASIZE 1024

extern dword socketid;

struct nodeaddr
{
  byte node[6];
};

struct localaddr
{
  byte network[4];
  struct nodeaddr node;
};

struct ecbtype
{
  word link[2];
  word esr[2];
  byte inuse;
  byte complete;
  word socket;
  byte ipxwork[4];
  byte dwork[12];
  byte immedaddr[6];
  word fragcount;
  word fragdata[2];
  word fragsize;
};

struct netaddress
{
  byte net[4];
  struct nodeaddr node;
  word socket;
};

struct ipxheader
{
  word check;
  word length;
  byte tc;
  byte ptype;
  struct netaddress dest;
  struct netaddress src;
};

struct packet
{
  volatile struct ecbtype ecb;
  struct ipxheader ipx;
  char data[MAXDATASIZE];
};

extern struct localaddr *localadd;

int opensocket(byte,word);
void closesocket(word);
int initipx(void);
void getlocaladdress(void);
void listenforpacket(word packetseg);
void cancelpacket(word packetseg);
char *decode_localaddress(char *str,struct localaddr *la);
char *decode_address(char *str,struct netaddress *la);
void sendpacket(word ecbseg,struct nodeaddr destnode, struct packet *packetet,word len);
void initsendpacket(struct packet *packetet,word lseg);
void initreceivepacket(struct packet *packetet,word lseg);
struct nodeaddr BROADCAST();
#endif
