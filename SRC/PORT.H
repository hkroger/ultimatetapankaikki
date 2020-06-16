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

#if defined (__EMSCRIPTEN__)
#define TK_PORT_EMCC
#include <emscripten.h>
#endif

namespace tk_port
{
int init( void );

void deinit( void );

void event_tick( void );

void change_resolution( const unsigned int y );

void save_screenshot( const char* path );

void set_palette( char palette_entries[768], int brightness );

void sleep( const int msec );

int vintage_clock( void );

void toggle_fullscreen( void );

extern bool quit_flag;
extern uint32_t debug;
}

#ifndef _PORT_IMPL
#define vclock tk_port::vintage_clock
#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC VINTAGE_CLOCKS_PER_SEC
#endif

#endif
