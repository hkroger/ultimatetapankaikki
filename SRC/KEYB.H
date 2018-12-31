#ifndef __TK__KEYB__
#define __TK__KEYB__

#include <SDL.h>
#include "PORT.H"

namespace tk_port
{
namespace keyb
{
    const int ESC = 1;
    const int ARROW_UP = 94;
    const int ARROW_DOWN = 99;
    const int ARROW_LEFT = 96;
    const int ARROW_RIGHT = 97;
    const int ENTER = 28;
    const int LEFT_SHIFT = 42;
    const int GRAVE = 41;  // Left of 1
    const int CHR_1 = 2;
    const int CHR_2 = 3;
    const int CHR_3 = 4;
    const int CHR_4 = 5;
    const int CHR_5 = 6;
    const int CHR_6 = 7;
    const int CHR_7 = 8;
    const int CHR_8 = 9;
    const int CHR_9 = 10;
    const int CHR_0 = 11;
    const int DASH = 12;  // Right of 0
    const int CHR_M = 77;
    const int CHR_F = 70;
    const int F12 = 69;

    void init( int );
    void deinit();
    bool state( int );
    void clear( int );
    void clear_stack();
    int kbhit();
    int getch();
    void handle_event( const SDL_Event * );
}
}

namespace k = tk_port::keyb;

#endif
