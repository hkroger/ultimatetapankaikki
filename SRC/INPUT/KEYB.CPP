#include <cstdint>
#include <SDL.h>
#include "KEYB.H"
#include "PORT.H"

namespace tk_port
{
namespace keyb
{
char scancode_to_char( SDL_Scancode scancode )
{
    return SDL_GetKeyFromScancode( scancode ) & 0xFF;
}

void handle_event( const SDL_Event *e )
{
    switch (e->type)
    {
        case SDL_KEYDOWN:
        {
            // Key repeating is done manually with timer and SDL_USEREVENT
            if (e->key.repeat)
                break;

            // Shift+ESC hard-kills the game, for development.
            if ( e->key.keysym.scancode == SDL_SCANCODE_ESCAPE && e->key.keysym.mod & KMOD_LSHIFT )
            {
                quit_flag = true;
                exit( 42 );
            }

            if ( e->key.keysym.scancode == SDL_SCANCODE_RETURN && e->key.keysym.mod & KMOD_LALT )
            {
                toggle_fullscreen();
                return;
            }

            input::pressed( e->key.keysym.scancode, e->key.timestamp );

            break;
        }

        case SDL_KEYUP:
        {
            input::released( e->key.keysym.scancode );

            break;
        }
        default:
            break;
    }
}
}
}
