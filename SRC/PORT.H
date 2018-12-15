#ifndef _PORT_H
#define _PORT_H

#include <cstdint>

// openwatcom (https://github.com/open-watcom/owp4v1copy/blob/master/bld/hdr/watcom/time.mh#L94)
// defines it as 1000.
#define VINTAGE_CLOCKS_PER_SEC 22

#if defined (__APPLE__) && defined (__MACH__)
#define TK_PORT_MAC
#define TK_PORT_POSIXISH
#elif defined (__unix__)
#define TK_PORT_POSIX
#define TK_PORT_POSIXISH
#elif defined (_WINDOWS)
#define TK_PORT_MSVC
#endif

int tk_port_init( void );

void tk_port_exit( void );

void tk_port_event_tick( void );

void tk_port_present_screen( void );

void tk_port_save_screenshot( const char* path );

void tk_port_set_palette( char palette_entries[768], int brightness );

void tk_port_sleep(int msec);

int tk_port_vintage_clock();

double tk_port_debug_wallclock();

extern bool tk_port_quit_flag;
extern uint32_t tk_port_debug;


#ifndef _PORT_IMPL
// Override `clock` everywhere with our vintage clock
#define clock tk_port_vintage_clock
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC VINTAGE_CLOCKS_PER_SEC
#endif

#endif
