#define IPXDEBUG

#include "int.h"    // My asm header file..
#include <stdio.h>
#include <i86.h>
#include <conio.h>
#include <string.h>
#include <iostream.h>

#include "ipx.c"    // More ipx code..

#define SERVER 1
#define CLIENT 0
#define SOCKET 0x869c  // DOOM Socket
#define RECEIVERS 10

#define IPX_END 0
#define IPX_SERVERSEARCH 1
#define IPX_TEXTMESSAGE 2

int mode;
char str[255];

struct packet far *send,far *rec[RECEIVERS];
unsigned short sendl,sendseg,recl[RECEIVERS],recseg[RECEIVERS];

void init_memory()
{ int a;
  send=(struct packet far *)MK_FP(getlowmem(sizeof(struct packet)/16+1),0);
  sendl=lsel;
  sendseg=lseg;
  initsendpacket(send,sendseg);

  for (a=0;a<RECEIVERS;a++)
  {		rec[a]=(struct packet far *)MK_FP(getlowmem(sizeof(struct packet)/16+1),0);
		recl[a]=lsel;
		recseg[a]=lseg;
		initreceivepacket(rec[a],recseg[a]);
  }

}

// Ei pist„ nollaa stringin loppuun!!!
char2hex(char *str,char ch) 
{	char hex[]="0123456789ABCDEF";
	str[0]=hex[ch>>4];
	str[1]=hex[ch&0xf];
}

char *decode_localaddress(char *str,struct localaddr far *la)
{ int a;

  for (a=0;a<4;a++)
   char2hex(str+(a*2),la->network[a]);
  str[8]=':';

  for (a=0;a<6;a++)
   char2hex(str+(a*2)+9,la->node[a]);
  str[21]=0;
  return(str);
}

char *decode_address(char *str,struct netaddress far *la)
{ int a;
  for (a=0;a<6;a++)
   char2hex(str+(a*2),la->node[a]);
  str[12]=0;
  return(str);
}

askserver()
{
	send->data[0]=IPX_SERVERSEARCH;
	send->data[1]=IPX_END;
	sendpacket(sendseg,MAXNODES,send,10);
}

sendtext(char *string)
{	int a,l;

	l=strlen(string);
 	send->data[0]=IPX_TEXTMESSAGE;
	for (a=0;a<l;a++)
	 send->data[a+1]=string[a];
	send->data[l+1]=IPX_END;
	sendpacket(sendseg,MAXNODES,send,l+2);
}

clientloop()
{
	askserver();
	sendtext("Allu on mulukku");
	sendtext("Allu on mulukku2");
	sendtext("Allu on mulukku5");
//	waitserver();
}

serverloop()
{	int a;
	int quit;
	int data,ofs;

	quit=0;
	while (kbhit()) getch();
	while (!quit)
	for (a=0;a<RECEIVERS;a++)
	if (rec[a]->ecb.inuse==0)
	{
		data=-1;
		ofs=0;
		while(data!=IPX_END)  
		{ 
			data=rec[a]->data[ofs++];
			switch (data)
			{
			case IPX_SERVERSEARCH:
				 printf("Search request received from %s\n",decode_address(str,&(rec[a]->ipx.src)));
				break;
			case IPX_TEXTMESSAGE:
				 while ((data=rec[a]->data[ofs++])!=0)
				  putch(data);
				 printf("\n");
				 break;
			case IPX_END:
			break;
			default:printf("unknown command %d\n",data,data,data);
			};

		}
		listenforpacket(recseg[a]);
	}
	else if (kbhit()) quit=1;
}


main(int argc, char *argv[])
{
  argv=argv;
  if (argc>1) mode=SERVER;
  	     else mode=CLIENT;

  initipx(); // initialize the ipx routines.

  if (!opensocket(0,SOCKET))
  { 	   
    init_memory();
    printf("SocketID = %x\n",(dword)socketid);
	getlocaladdress();
	printf("Local address: %s\n",decode_localaddress(str,localadd));

	if (mode==SERVER) serverloop();
				 else clientloop();

    // Closing down IPX...
    printf("Done.\n");
    closesocket(socketid);
  }
    else
  {
    printf("Unable to open socket (%x)!",(dword)SOCKET);
    exit(0);
  }
}
