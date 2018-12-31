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

int _kbhit_flag;
int _last_getch;

struct key_repeat
{
    SDL_Scancode scancode;
    uint32_t pressed_time;
    int repeats;
};

std::set<SDL_Scancode> pressed_keys;
key_repeat last_pressed_key = { SDL_SCANCODE_UNKNOWN, 0, 0 };

uint8_t remap_key( SDL_Keycode keycode )
{
    return keycode & 0xFF;
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
    _kbhit_flag = 0;
    _last_getch = 0;
}

SDL_Scancode get_last_key()
{
    return state( last_pressed_key.scancode ) ?
        last_pressed_key.scancode :
        SDL_SCANCODE_UNKNOWN;
}

int kbhit()
{
    if ( _kbhit_flag )
    {
        _kbhit_flag = 0;
        return 1;
    }
    return 0;
}

int getch()
{
    while (!kbhit())
    {
        event_tick();
    }
    return _last_getch;
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

            const uint8_t tk_key = remap_key( e->key.keysym.sym );
            if ( debug )
            {
                printf( "key down: %04x (tk = %d)\n", e->key.keysym.scancode, tk_key );
            }
            _last_getch = tk_key;
            _kbhit_flag = true;
            break;
        }

        case SDL_KEYUP:
        {
            clear( e->key.keysym.scancode );
            if ( e->key.keysym.scancode == last_pressed_key.scancode ) {
                last_pressed_key.scancode = SDL_SCANCODE_UNKNOWN;
            }
            break;
        }

        case SDL_USEREVENT:
        {
            if (e->user.code != KEY_REPEAT_TIMER_CODE)
                break;

            if (last_pressed_key.scancode == SDL_SCANCODE_UNKNOWN)
                break;

            uint32_t elapsed = e->user.timestamp - last_pressed_key.pressed_time;
            if ( elapsed >= SYMBOL_KEY_REPEAT_START +
                 last_pressed_key.repeats * SYMBOL_KEY_REPEAT_INTERVAL )
            {
                pressed_keys.insert( last_pressed_key.scancode );
                _last_getch = remap_key( SDL_GetKeyFromScancode( last_pressed_key.scancode ) );
                _kbhit_flag = true;
                last_pressed_key.repeats++;
            }

            break;
        }
    }
}

}
}
