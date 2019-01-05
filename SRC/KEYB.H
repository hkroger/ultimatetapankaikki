#ifndef __TK__KEYB__
#define __TK__KEYB__

#include <SDL.h>
#include "PORT.H"
#include "INPUT.H"

namespace tk_port
{
namespace keyb
{
    const Input NOTHING = SDL_SCANCODE_UNKNOWN;
    const Input ESC = SDL_SCANCODE_ESCAPE;
    const Input ARROW_UP = SDL_SCANCODE_UP;
    const Input ARROW_DOWN = SDL_SCANCODE_DOWN;
    const Input ARROW_LEFT = SDL_SCANCODE_LEFT;
    const Input ARROW_RIGHT = SDL_SCANCODE_RIGHT;
    const Input ENTER = SDL_SCANCODE_RETURN;
    const Input BACKSPACE = SDL_SCANCODE_BACKSPACE;
    const Input LEFT_SHIFT = SDL_SCANCODE_LSHIFT;
    const Input GRAVE = SDL_SCANCODE_GRAVE;  // Left of 1
    const Input MINUS = SDL_SCANCODE_MINUS;  // Right of 0
    const Input SPACE = SDL_SCANCODE_SPACE;
    const Input CHR_1 = SDL_SCANCODE_1;
    const Input CHR_2 = SDL_SCANCODE_2;
    const Input CHR_3 = SDL_SCANCODE_3;
    const Input CHR_4 = SDL_SCANCODE_4;
    const Input CHR_5 = SDL_SCANCODE_5;
    const Input CHR_6 = SDL_SCANCODE_6;
    const Input CHR_7 = SDL_SCANCODE_7;
    const Input CHR_8 = SDL_SCANCODE_8;
    const Input CHR_9 = SDL_SCANCODE_9;
    const Input CHR_0 = SDL_SCANCODE_0;
    const Input CHR_Y = SDL_SCANCODE_Y;
    const Input F5 = SDL_SCANCODE_F5;
    const Input F12 = SDL_SCANCODE_F12;

    void init( int );
    void deinit();
    bool state( const Input& );
    void clear( const Input& );
    void clear_stack();
    void pressed( const Input& input, const Uint32 timestamp );
    void released( const Input& input );

    // Return key/button that was pressed last, or NOTHING if
    // no key is currently pressed.
    Input get_last_key();

    // Wait until a key/button is pressed and return it
    Input wait_for_keypress();

    // Return a character corresponding the given scancode
    char scancode_to_char( SDL_Scancode scancode );

    void handle_event( const SDL_Event * );
}
}

namespace k = tk_port::keyb;

#endif
