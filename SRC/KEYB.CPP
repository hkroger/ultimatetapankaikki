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
    Input input;
    uint32_t pressed_time;
    int repeats;
};

std::set<Input> pressed_keys;
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

bool state( const Input& input )
{
    return pressed_keys.find( input ) != pressed_keys.end();
}

void clear( const Input& input)
{
    pressed_keys.erase( input );
}

void clear_stack()
{
    pressed_keys.clear();
}

Input get_last_key()
{
    return state( last_pressed_key.input ) ?
        last_pressed_key.input :
        NOTHING;
}

Input wait_for_keypress()
{
    Input input;
    while ( ( input = get_last_key()) == NOTHING )
    {
        event_tick();
    }
    clear( input );
    return input;
}

void pressed( const Input& input, const Uint32 timestamp )
{
    pressed_keys.insert( input );

    last_pressed_key.input = input;
    last_pressed_key.pressed_time = timestamp;
    last_pressed_key.repeats = 0;
}

void released( const Input& input )
{
    clear( input );
    if ( input == last_pressed_key.input )
    {
        last_pressed_key.input = NOTHING;
    }
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

            pressed( e->key.keysym.scancode, e->key.timestamp );

            break;
        }

        case SDL_KEYUP:
        {
            released( e->key.keysym.scancode );

            break;
        }

        case SDL_USEREVENT:
        {
            if (e->user.code != KEY_REPEAT_TIMER_CODE)
                break;

            if (last_pressed_key.input == NOTHING)
                break;

            uint32_t elapsed = e->user.timestamp - last_pressed_key.pressed_time;
            if ( elapsed >= SYMBOL_KEY_REPEAT_START +
                 last_pressed_key.repeats * SYMBOL_KEY_REPEAT_INTERVAL )
            {
                pressed_keys.insert( last_pressed_key.input );
                last_pressed_key.repeats++;
            }

            break;
        }
    }
}

}
}
