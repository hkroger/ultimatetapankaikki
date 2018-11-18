// IPX EXAMPLE FOR WATCOM C/C++ (10.0a) UNDER DOS4GW PMODE DPMI.
// Written by Daniel Jeppson d94dj@efd.lth.se 960511 03:28 AM (8

// You are hearby granted the right to use this code freely but
// I would appreciate a credit or greet if you find it useful...

// Please see the readme.txt and chat.c for more information...

#ifndef __INT_H_
#define __INT_H_

#ifdef  __cplusplus
extern "C" {
#endif

unsigned short getlowmem(unsigned short size);
void freelowmem(unsigned short sel);

#ifdef  __cplusplus
}
#endif

#ifdef  __cplusplus
extern "C" {
#endif

extern unsigned short lseg;
extern unsigned short lsel;

#ifdef  __cplusplus
}
#endif

#pragma aux getlowmem "_*" parm [EBX] caller;
#pragma aux freelowmem "_*" parm [EDX] caller;

#endif
