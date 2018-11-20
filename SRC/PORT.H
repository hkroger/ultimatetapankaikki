#ifndef _PORT_H
#define _PORT_H

// DJGPP's time.h defines CLOCKS_PER_SEC as 91,
// openwatcom (https://github.com/open-watcom/owp4v1copy/blob/master/bld/hdr/watcom/time.mh#L94)
// defines it as 1000.
// Given how clock() is used in the source otherwise, assuming 1000 seems safer.
// Oh, no, wait, disregard the above. The value below has been eyeballed by comparing the
// menu animations of the actual GAME.EXE running in DOSBox to the recompilation.
#define VINTAGE_CLOCKS_PER_SEC 104

#if defined (__APPLE__) && defined (__MACH__)
#define TK_PORT_MAC
#define TK_PORT_POSIXISH
#endif
#if defined (__unix__)
#define TK_PORT_POSIX
#define TK_PORT_POSIXISH
#endif

int tk_port_init( void );

void tk_port_exit( void );

void tk_port_event_tick( void );

void tk_port_present_screen( void );

void tk_port_set_palette( char palette_entries[768], int brightness );

void tk_port_sleep(int msec);

int tk_port_vintage_clock();

double tk_port_debug_wallclock();

extern bool tk_port_quit_flag;


#ifndef _PORT_IMPL
// Override `clock` everywhere with our vintage clock
#define clock tk_port_vintage_clock
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC VINTAGE_CLOCKS_PER_SEC
#endif

#endif
