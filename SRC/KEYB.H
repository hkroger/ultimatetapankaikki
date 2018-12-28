#ifndef __TK__KEYB__
#define __TK__KEYB__

#include <SDL.h>
#include "PORT.H"

namespace tk_port
{
namespace keyb
{
    extern char state[256];

    void init( int );
    void deinit();
    void clear_stack();
    int kbhit();
    int getch();
    void handle_event( const SDL_Event * );
}
}

namespace k = tk_port::keyb;

#endif
