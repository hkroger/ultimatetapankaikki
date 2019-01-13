#ifndef __TK__KEYB__
#define __TK__KEYB__

#include <SDL.h>
#include "INPUT.H"

namespace tk_port
{
namespace keyb
{
    const input::Input ESC = SDL_SCANCODE_ESCAPE;
    const input::Input ARROW_UP = SDL_SCANCODE_UP;
    const input::Input ARROW_DOWN = SDL_SCANCODE_DOWN;
    const input::Input ARROW_LEFT = SDL_SCANCODE_LEFT;
    const input::Input ARROW_RIGHT = SDL_SCANCODE_RIGHT;
    const input::Input ENTER = SDL_SCANCODE_RETURN;
    const input::Input BACKSPACE = SDL_SCANCODE_BACKSPACE;
    const input::Input LEFT_SHIFT = SDL_SCANCODE_LSHIFT;
    const input::Input GRAVE = SDL_SCANCODE_GRAVE;  // Left of 1
    const input::Input MINUS = SDL_SCANCODE_MINUS;  // Right of 0
    const input::Input SPACE = SDL_SCANCODE_SPACE;
    const input::Input CHR_1 = SDL_SCANCODE_1;
    const input::Input CHR_2 = SDL_SCANCODE_2;
    const input::Input CHR_3 = SDL_SCANCODE_3;
    const input::Input CHR_4 = SDL_SCANCODE_4;
    const input::Input CHR_5 = SDL_SCANCODE_5;
    const input::Input CHR_6 = SDL_SCANCODE_6;
    const input::Input CHR_7 = SDL_SCANCODE_7;
    const input::Input CHR_8 = SDL_SCANCODE_8;
    const input::Input CHR_9 = SDL_SCANCODE_9;
    const input::Input CHR_0 = SDL_SCANCODE_0;
    const input::Input CHR_Y = SDL_SCANCODE_Y;
    const input::Input F5 = SDL_SCANCODE_F5;
    const input::Input F12 = SDL_SCANCODE_F12;

    // Return a character corresponding the given scancode
    char scancode_to_char( SDL_Scancode scancode );

    void handle_event( const SDL_Event * );
}
}

namespace k = tk_port::keyb;

#endif
