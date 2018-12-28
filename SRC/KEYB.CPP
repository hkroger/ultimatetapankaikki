#include <SDL.h>
#include <cstdint>
#include "KEYB.H"

namespace tk_port
{
namespace keyb
{

char state[256];
int _kbhit_flag;
int _last_getch;

uint8_t remap_key( SDL_Keycode keycode )
{
    return keycode & 0xFF;
}

uint8_t get_scancode_index( SDL_Keycode keycode, SDL_Scancode scancode )
{
    switch (scancode)
    {
        case SDL_SCANCODE_LCTRL:
            return 29;
        case SDL_SCANCODE_RCTRL:
            return 90;
        case SDL_SCANCODE_LSHIFT:
            return 42;
        case SDL_SCANCODE_RSHIFT:
            return 54;
        case SDL_SCANCODE_CAPSLOCK:
            return 58;
        case SDL_SCANCODE_LALT:
            return 56;
        case SDL_SCANCODE_RALT:
            return 92;
        default:
            break;
    }

    if ( keycode >= 'a' && keycode <= 'z' )
    {
        return keycode & 0xDF;
    }

    switch (keycode)
    {
        case SDLK_ESCAPE:
            return 1;
        case SDLK_TAB:
            return 15;
        case SDLK_SPACE:
            return 57;
        case SDLK_PAGEDOWN:
            return 100;
        case SDLK_PAGEUP:
            return 95;
        case SDLK_HOME:
            return 93;
        case SDLK_END:
            return 98;
        case SDLK_INSERT:
            return 101;
        case SDLK_DELETE:
            return 102;
        case SDLK_UP:
            return 94;
        case SDLK_DOWN:
            return 99;
        case SDLK_LEFT:
            return 96;
        case SDLK_RIGHT:
            return 97;
        case SDLK_RETURN:
        case SDLK_KP_ENTER:
        case SDLK_RETURN2:
            return 28;
        default:
            return keycode & 0xFF;
    }
}

void init( int )
{
    clear_stack();
}

void deinit() {}

void clear_stack()
{
    for ( int i = 0; i < 256; i++ )
    {
        state[i] = 0;
    }
    _kbhit_flag = 0;
    _last_getch = 0;
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
    if (e->type != SDL_KEYDOWN && e->type != SDL_KEYUP)
        return;

    // We handle repeating separately
    if (e->key.repeat)
        return;

    bool down = (e->key.type == SDL_KEYDOWN);

    // Shift+ESC hard-kills the game, for development.
    if ( down && e->key.keysym.scancode == SDL_SCANCODE_ESCAPE && e->key.keysym.mod & KMOD_LSHIFT )
    {
        quit_flag = true;
        exit( 42 );
    }

    state[get_scancode_index( e->key.keysym.sym, e->key.keysym.scancode )] = down;

    if ( down )
    {
        const uint8_t tk_key = remap_key( e->key.keysym.sym );

        if ( debug )
        {
            printf( "key down: %04x (tk = %d)\n", e->key.keysym.scancode, tk_key );
        }
        _last_getch = tk_key;
        _kbhit_flag = true;
    }
}

}
}
