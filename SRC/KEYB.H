#ifndef __TK__KEYB__
#define __TK__KEYB__

#include <SDL.h>
#include "PORT.H"

namespace tk_port
{
namespace keyb
{
    const SDL_Scancode NOTHING = SDL_SCANCODE_UNKNOWN;
    const SDL_Scancode ESC = SDL_SCANCODE_ESCAPE;
    const SDL_Scancode ARROW_UP = SDL_SCANCODE_UP;
    const SDL_Scancode ARROW_DOWN = SDL_SCANCODE_DOWN;
    const SDL_Scancode ARROW_LEFT = SDL_SCANCODE_LEFT;
    const SDL_Scancode ARROW_RIGHT = SDL_SCANCODE_RIGHT;
    const SDL_Scancode ENTER = SDL_SCANCODE_RETURN;
    const SDL_Scancode BACKSPACE = SDL_SCANCODE_BACKSPACE;
    const SDL_Scancode LEFT_SHIFT = SDL_SCANCODE_LSHIFT;
    const SDL_Scancode GRAVE = SDL_SCANCODE_GRAVE;  // Left of 1
    const SDL_Scancode MINUS = SDL_SCANCODE_MINUS;  // Right of 0
    const SDL_Scancode SPACE = SDL_SCANCODE_SPACE;
    const SDL_Scancode CHR_1 = SDL_SCANCODE_1;
    const SDL_Scancode CHR_2 = SDL_SCANCODE_2;
    const SDL_Scancode CHR_3 = SDL_SCANCODE_3;
    const SDL_Scancode CHR_4 = SDL_SCANCODE_4;
    const SDL_Scancode CHR_5 = SDL_SCANCODE_5;
    const SDL_Scancode CHR_6 = SDL_SCANCODE_6;
    const SDL_Scancode CHR_7 = SDL_SCANCODE_7;
    const SDL_Scancode CHR_8 = SDL_SCANCODE_8;
    const SDL_Scancode CHR_9 = SDL_SCANCODE_9;
    const SDL_Scancode CHR_0 = SDL_SCANCODE_0;
    const SDL_Scancode CHR_Y = SDL_SCANCODE_Y;
    const SDL_Scancode F5 = SDL_SCANCODE_F5;
    const SDL_Scancode F12 = SDL_SCANCODE_F12;

    void init( int );
    void deinit();
    bool state( SDL_Scancode );
    void clear( SDL_Scancode );
    void clear_stack();

    // Return scancode of the key that was pressed last, or NOTHING if
    // no key is currently pressed.
    SDL_Scancode get_last_key();

    // Wait until a key is pressed and return its scancode
    SDL_Scancode wait_for_keypress();

    // Return a character corresponding the given scancode
    char scancode_to_char( SDL_Scancode scancode );

    void handle_event( const SDL_Event * );
}
}

namespace k = tk_port::keyb;

#endif
