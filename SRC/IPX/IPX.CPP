#if 0
// IPX EgotoXAMPLE FOR WATCOM C/C++ (10.0a) UNDER DOS4GW PMODE DPMI.
// Written by Daniel Jeppson d94dj@efd.lth.se 960511 03:28 AM (8

// You are hearby granted the right to use this code freely but
// I would appreciate a credit or greet if you find it useful...

// Please see the readme.txt and chat.c for more information...

#define MAXNODES 1      // Maximum number of nodes

#include <i86.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../error/error.h"
#include "ipx.h"              // My ipx header file..
#include "../int/int.h"

struct nodeaddr nodes[MAXNODES+1];  // Last is broadcast and first is local...
dword socketid=0;

static struct rminfo
{
  long EDI;
  long ESI;
  long EBP;
  long reserved_by_system;
  long EBX;
  long EDX;
  long ECX;
  long EAX;
  short flags;
  short ES,DS,FS,GS,IP,CS,SP,SS;
} RMI;

word localaddl=0; // Selector to the localadd lowmem allocated.
struct localaddr *localadd=NULL;

union REGS regs;
struct SREGS sregs;

void clearrmi(void)
{ // Clear the RMI registers...
  memset(&sregs,0,sizeof(sregs));
  memset(&RMI,0,sizeof(RMI));
}

void callrealint(byte inten)
{ // This routine calls a realmode interrupt (interrupt inten)
  int interrupt_no=0x31;

  // Use DMPI call 300h to issue the DOS interrupt
  regs.w.ax = 0x0300;
  regs.h.bh = 0;
  regs.h.bl = inten;
  regs.w.cx = 0;
  sregs.es = FP_SEG(&RMI);
  regs.x.edi = FP_OFF(&RMI);
  int386x( interrupt_no, &regs, &regs, &sregs );
}

int swap(int swappen)
{ // swap the low and high order byte of a word...
  swappen&=0xffff;
  return (swappen<<8)+((swappen&65280)>>8);
}

int opensocket(byte longevity,word socket)
{ // Open up a socket.. Returns 0xff if everything went fine..

  //Set up real-mode call structure
  clearrmi();
  RMI.EBX=0x0;  
  RMI.EAX=longevity;
  RMI.EDX=swap(socket);
  callrealint(0x7a);
  socketid=(word)swap(RMI.EDX);
  
  return (byte)(RMI.EAX);
}

void closesocket(word socket)
{ // Closes an open socket.. Do not forget to call this routine!!
  clearrmi();
  RMI.EBX=0x1;
  RMI.EDX=swap(socket);
  callrealint(0x7a);
}

int initipx(void)
{ // Does what it is called... (8
  int q;

  clearrmi();
  RMI.EAX=0x7A00;
  callrealint(0x2f);
  if ((RMI.EAX&255)!=255) return(0);

  getlocaladdress();
  for (q=0;q<6;q++) 
   {
	   nodes[0].node[q]=localadd->node.node[q]; // HOME
	   nodes[MAXNODES].node[q]=0xff; // BROADCAST
   }
  return(1);
}

struct nodeaddr BROADCAST()
{
	return nodes[MAXNODES];
}

void getlocaladdress(void)
{
  if (localadd==NULL)
  	{
	  localadd=(struct localaddr *)MK_FP(getlowmem(sizeof(struct localaddr)/16+1),0);
	  localaddl=lsel;
	};
  clearrmi();
  RMI.EBX=9;
  RMI.ES=lseg;
  RMI.ESI=0;
  callrealint(0x7a);
}

void listenforpacket(word packetseg)
{
  clearrmi();
  RMI.ESI=0;          // Offset to ECB is 0 when you allocate lowmem...
  RMI.ES=packetseg;
  RMI.EBX=4;
  RMI.EAX=0;
  callrealint(0x7a);
  if (RMI.EAX&255) error("Listenforpacket: failed!\n");
}

void cancelpacket(word packetseg)
{
  clearrmi();
  RMI.ESI=0;          // Offset to ECB is 0 when you allocate lowmem...
  RMI.ES=packetseg;
  RMI.EBX=6;
  RMI.EAX=0;
  callrealint(0x7a);
  if (RMI.EAX&255) error("Cancelpacket: failed!\n");
}

void initreceivepacket(struct packet *packetet,word lseg)
{
  _fmemset(packetet,0,sizeof(struct packet));
  packetet->ecb.inuse=0x1d;
  packetet->ecb.socket=(word)swap(socketid);
  packetet->ecb.fragcount=1;
  packetet->ecb.fragdata[0]=sizeof(struct ecbtype);
  // fragdata[0] is the offset to the ipx header.
  // Since ipx comes after ecb it's sizeof(ecb) bytes
  // after the allocated memory...

  packetet->ecb.fragdata[1]=lseg;
  // lseg contains the latest allocated lowmem segment.
  
  packetet->ecb.fragsize=sizeof(struct packet)-sizeof(struct ecbtype);
}                     
						   
void initsendpacket(struct packet *packetet,word lseg)
{
  _fmemset(packetet,0,sizeof(struct packet));
  
  packetet->ecb.socket=(word)swap(socketid);
  packetet->ecb.fragcount=1;
  packetet->ecb.fragdata[0]=sizeof(struct ecbtype);
  packetet->ecb.fragdata[1]=lseg;

  packetet->ecb.immedaddr[0]=0xff;
  packetet->ecb.immedaddr[1]=0xff;
  packetet->ecb.immedaddr[2]=0xff;
  packetet->ecb.immedaddr[3]=0xff;
  packetet->ecb.immedaddr[4]=0xff;
  packetet->ecb.immedaddr[5]=0xff;
  packetet->ipx.dest.node.node[0]=0xff;
  packetet->ipx.dest.node.node[1]=0xff;
  packetet->ipx.dest.node.node[2]=0xff;
  packetet->ipx.dest.node.node[3]=0xff;
  packetet->ipx.dest.node.node[4]=0xff;
  packetet->ipx.dest.node.node[5]=0xff;

  packetet->ipx.check=0xffff;
  packetet->ipx.ptype=0x0;
  packetet->ipx.tc=0x0;
  packetet->ipx.dest.net[0]=localadd->network[0];
  packetet->ipx.dest.net[1]=localadd->network[1];
  packetet->ipx.dest.net[2]=localadd->network[2];
  packetet->ipx.dest.net[3]=localadd->network[3];
  packetet->ipx.dest.socket=(word)swap(socketid);
  packetet->ipx.src.net[0]=localadd->network[0];
  packetet->ipx.src.net[1]=localadd->network[1];
  packetet->ipx.src.net[2]=localadd->network[2];
  packetet->ipx.src.net[3]=localadd->network[3];
  packetet->ipx.src.node.node[0]=localadd->node.node[0];
  packetet->ipx.src.node.node[1]=localadd->node.node[1];
  packetet->ipx.src.node.node[2]=localadd->node.node[2];
  packetet->ipx.src.node.node[3]=localadd->node.node[3];
  packetet->ipx.src.node.node[4]=localadd->node.node[4];
  packetet->ipx.src.node.node[5]=localadd->node.node[5];
  packetet->ipx.src.socket=(word)swap(socketid);
}

void sendpacket(word ecbseg,struct nodeaddr destnode, struct packet *packetet,word len)
{ // Sends a packet...
  // ecbseg is the senders segment...
  int q;
  clearrmi();

  for (q=0;q<6;q++)
   packetet->ecb.immedaddr[q]=packetet->ipx.dest.node.node[q]=destnode.node[q];

  if (len>MAXDATASIZE) len=MAXDATASIZE;
  packetet->ecb.fragsize=(word)(sizeof(struct ipxheader)+len);
      
  RMI.ESI=0;
  RMI.ES=ecbseg;
  RMI.EBX=3;
  callrealint(0x7a);

  if (RMI.EAX&255) error("sendpacket: sending failed (%x)\ncompletioncode: %d\ndestination: 0x%x%x%x%x%x%x\nfragsize: %d\nDatalen: %d\n",
  					RMI.EAX,
  					packetet->ecb.complete,
  					packetet->ecb.immedaddr[0],
  					packetet->ecb.immedaddr[1],
  					packetet->ecb.immedaddr[2],
  					packetet->ecb.immedaddr[3],
  					packetet->ecb.immedaddr[4],
  					packetet->ecb.immedaddr[5],
  					packetet->ecb.fragsize,
  					len);
   
}

void char2hex(char *str,char ch) 
{	char hex[]="0123456789ABCDEF";
	str[0]=hex[ch>>4];
	str[1]=hex[ch&0xf];
}

char *decode_localaddress(char *str,struct localaddr *la)
{ int a;

  for (a=0;a<4;a++)
   char2hex(str+(a*2),la->network[a]);
  str[8]=':';

  for (a=0;a<6;a++)
   char2hex(str+(a*2)+9,la->node.node[a]);
  str[21]=0;
  return(str);
}

char *decode_address(char *str,struct netaddress *la)
{ int a;
  for (a=0;a<6;a++)
   char2hex(str+(a*2),la->node.node[a]);
  str[12]=0;
  return(str);
}
#endif
