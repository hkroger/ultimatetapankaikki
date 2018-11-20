#ifndef _PORT_H
#define _PORT_H

// h/t https://stackoverflow.com/a/11351171/51685
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#define TK_PORT_POSIXISH
#else
#endif

int tk_port_init( void );

void tk_port_exit( void );

void tk_port_event_tick( void );

void tk_port_present_screen( void );

#endif
