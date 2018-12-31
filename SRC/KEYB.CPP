#include <set>
#include <cstdint>
#include <SDL.h>
#include "KEYB.H"

namespace tk_port
{
namespace keyb
{

const uint32_t KEY_REPEAT_TIMER_CODE = 43615681;  // Random number heh heh
const uint32_t SYMBOL_KEY_REPEAT_START = 500;     // ms
const uint32_t SYMBOL_KEY_REPEAT_INTERVAL = 25;   // ms

struct key_repeat
{
    SDL_Scancode scancode;
    uint32_t pressed_time;
    int repeats;
};

std::set<SDL_Scancode> pressed_keys;
key_repeat last_pressed_key = { NOTHING, 0, 0 };

char scancode_to_char( SDL_Scancode scancode )
{
    return SDL_GetKeyFromScancode( scancode ) & 0xFF;
}

uint32_t timer_callback( Uint32 interval, void * )
{
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = KEY_REPEAT_TIMER_CODE;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return interval;
}

void init( int )
{
    SDL_AddTimer( 5, timer_callback, NULL );
    clear_stack();
}

void deinit() {}

bool state( SDL_Scancode scancode )
{
    return pressed_keys.find( scancode ) != pressed_keys.end();
}

void clear( SDL_Scancode scancode )
{
    pressed_keys.erase( scancode );
}

void clear_stack()
{
    pressed_keys.clear();
}

SDL_Scancode get_last_key()
{
    return state( last_pressed_key.scancode ) ?
        last_pressed_key.scancode :
        NOTHING;
}

SDL_Scancode wait_for_keypress()
{
    SDL_Scancode scancode;
    while ( (scancode = get_last_key()) == NOTHING )
    {
        event_tick();
    }
    clear( scancode );
    return scancode;
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

            pressed_keys.insert( e->key.keysym.scancode );

            last_pressed_key.scancode = e->key.keysym.scancode;
            last_pressed_key.pressed_time = e->key.timestamp;
            last_pressed_key.repeats = 0;
            break;
        }

        case SDL_KEYUP:
        {
            clear( e->key.keysym.scancode );
            if ( e->key.keysym.scancode == last_pressed_key.scancode ) {
                last_pressed_key.scancode = NOTHING;
            }
            break;
        }

        case SDL_USEREVENT:
        {
            if (e->user.code != KEY_REPEAT_TIMER_CODE)
                break;

            if (last_pressed_key.scancode == NOTHING)
                break;

            uint32_t elapsed = e->user.timestamp - last_pressed_key.pressed_time;
            if ( elapsed >= SYMBOL_KEY_REPEAT_START +
                 last_pressed_key.repeats * SYMBOL_KEY_REPEAT_INTERVAL )
            {
                pressed_keys.insert( last_pressed_key.scancode );
                last_pressed_key.repeats++;
            }

            break;
        }
    }
}

}
}
