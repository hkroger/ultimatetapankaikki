#define _PORT_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include "PORT.H"
#include "GLOBVAR.H"
#include "ERROR/ERROR.H"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Palette *palette;
static SDL_Rect render_dest_rect;
static bool window_resized;
bool tk_port_quit_flag = false;
uint32_t tk_port_debug = 0;
extern int scr_y_size;

static uint64_t timer_zero;
#define TK_PORT_NSEC_PER_SEC 1000000000L
#define TK_PORT_NSEC_PER_MSEC 1000000L
#define TK_PORT_GRAPHICS_SCALE 3

#ifdef TK_PORT_MAC
#include <mach/mach_time.h>
static mach_timebase_info_data_t timer_timebase_info;
#endif

static Uint32 tk_port_update_framecount( Uint32 interval, void *param )
{
    framecount++;
    // Return the next interval to be used. This technically allows
    // for dynamic speed changes during the game by mutating the `target_frames`
    // global...
    return 1000 / target_frames;
}


static void read_tk_port_debug()
{
    const char *tk_port_debug_s = getenv( "TK_PORT_DEBUG" );
    if ( tk_port_debug_s == NULL )
    {
        return;
    }
    tk_port_debug = strtoul( tk_port_debug_s, NULL, 10 );
    if ( tk_port_debug != 0 )
    {
        SDL_Log( "tk_port_debug = %d (%08x)", tk_port_debug, tk_port_debug );
    }
}

static void get_render_rect( SDL_Rect *rect )
{
    int window_w, window_h;
    const double aspect = 320.0 / scr_y_size;

    SDL_GetRendererOutputSize( renderer, &window_w, &window_h );
    if ( 1.0 * window_w / window_h <= aspect )
    {
        int h = window_w / aspect;
        rect->x = 0;
        rect->y = (window_h - h) / 2;
        rect->w = window_w;
        rect->h = h;
    }
    else
    {
        int w = window_h * aspect;
        rect->x = (window_w - w) / 2;
        rect->y = 0;
        rect->w = w;
        rect->h = window_h;
    }
}

static int create_surface()
{
    surface = SDL_CreateRGBSurface( 0, 320, scr_y_size, 8, 0, 0, 0, 0 );

    if ( !surface )
    {
        SDL_Log( "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError() );
        return 4;
    }

    if ( surface->pitch != 320 )
    {
        SDL_Log( "Was unable to get a surface with the correct linear pitch 320" );
        return 5;
    }

    return 0;
}

static void init_screen()
{
    // Now that we have a linear 320x240x8 or 320x200x8 surface, assign it as our "VGA 0xA0000"
    // all other code can write into.
    screen = (char *) surface->pixels;
    memset( screen, 0, 320 * scr_y_size );

    // I mean, the first 768 bytes of `screen` are zero...
    tk_port_set_palette( screen, 0 );
}

static int tk_port_init_graphics()
{
    window = SDL_CreateWindow(
        "TK",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        320 * TK_PORT_GRAPHICS_SCALE,
        240 * TK_PORT_GRAPHICS_SCALE,
        SDL_WINDOW_RESIZABLE
    );
    if ( !window )
    {
        SDL_Log( "Error initializing SDL window:  %s", SDL_GetError());
        return 2;
    }
    renderer = SDL_CreateRenderer( window, -1, 0 );
    if ( !renderer )
    {
        SDL_Log( "Error initializing SDL renderer:  %s", SDL_GetError());
        return 3;
    }
    get_render_rect(&render_dest_rect);
    window_resized = false;

    const int s_ret = create_surface();
    if ( s_ret ) return s_ret;

    palette = SDL_AllocPalette( 256 );

    init_screen();

    return 0;
}

static void tk_port_init_time()
{
    // Set up the timer to update the framecount global.
    SDL_AddTimer( 1000 / target_frames, tk_port_update_framecount, NULL );

#ifdef TK_PORT_POSIX
    struct timespec tv;
    clock_gettime(CLOCK_MONOTONIC, &tv);
    timer_zero = tv.tv_sec * TK_PORT_NSEC_PER_SEC + tv.tv_nsec;
#endif
#ifdef TK_PORT_MAC
    mach_timebase_info( &timer_timebase_info );
    timer_zero = mach_absolute_time();
#endif
}

void tk_port_resolution_changed( void )
{
    SDL_RenderClear( renderer );
    get_render_rect( &render_dest_rect );

    SDL_FreeSurface( surface );

    const int s_ret = create_surface();
    if (s_ret) error( "Failed to create new surface" );

    init_screen();
}

int tk_port_init()
{
    int ret;
    read_tk_port_debug();
    if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
    {
        SDL_Log( "Error initializing SDL:  %s", SDL_GetError());
        return 1;
    }
    if((ret = tk_port_init_graphics()) != 0) {
        return ret;
    }
    tk_port_init_time();
    tk_port_event_tick();  // Tick once so we get the window up at this point
    return 0;
}

void tk_port_exit()
{
    if ( surface ) SDL_FreeSurface( surface );
    if ( palette ) SDL_FreePalette( palette );
    if ( renderer ) SDL_DestroyRenderer( renderer );
    if ( window ) SDL_DestroyWindow( window );

    SDL_Quit();
}

static uint8_t tk_port_remap_key( SDL_Keycode keycode )
{
    return keycode & 0xFF;
}

static uint8_t tk_port_get_scancode_index( SDL_Keycode keycode, SDL_Scancode scancode )
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

void tk_port_event_tick( void )
{
    SDL_Event e;
    while (SDL_PollEvent( &e ))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                tk_port_quit_flag = true;
                break;
            case SDL_KEYUP:
            case SDL_KEYDOWN:
            {
                bool down = (e.key.type == SDL_KEYDOWN);

                // Shift+ESC hard-kills the game, for development.
                if ( down && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && e.key.keysym.mod & KMOD_LSHIFT )
                {
                    tk_port_quit_flag = true;
                    exit( 42 );
                }

                k.state[tk_port_get_scancode_index( e.key.keysym.sym, e.key.keysym.scancode )] = down;

                if ( down )
                {
                    const uint8_t tk_key = tk_port_remap_key( e.key.keysym.sym );

                    if ( tk_port_debug )
                    {
                        printf( "key down: %04x (tk = %d)\n", e.key.keysym.scancode, tk_key );
                    }
                    k._last_getch = tk_key;
                    k._kbhit_flag = true;
                }
            }
                break;

            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    window_resized = true;
                }
                break;
        }
    }
    tk_port_present_screen();
}

void tk_port_present_screen( void )
{
    if ( window_resized )
    {
        SDL_RenderClear( renderer );
        get_render_rect( &render_dest_rect );
        window_resized = false;
    }

    SDL_Texture *surface_texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_RenderCopy( renderer, surface_texture, NULL, &render_dest_rect );
    SDL_RenderPresent( renderer );
    SDL_DestroyTexture( surface_texture );
}

void tk_port_save_screenshot( const char* path )
{
    SDL_SaveBMP( surface, path );
}

static int clamp( int val, int min, int max )
{
    if ( val < min ) return min;
    if ( val > max ) return max;
    return val;
}

void tk_port_set_palette( char palette_entries[768], int brightness )
{
    char *palette_ptr = palette_entries;

    for ( int i = 0; i < 256; i++ )
    {
        // We didn't have the implementation of setpal(), so this
        // "algorithm" was disassembled from GAME.EXE.
#define DARKEN( c ) clamp((int)(c * brightness) >> 8, 0, 63)

        // The left-shifts below scale the assumedly 0..63 VGA palette
        // (ref. http://joco.homeserver.hu/vgalessons/lesson6.html)
        // up to the 0..255 expected by SDL.
        palette->colors[i].r = (uint8_t) clamp( DARKEN( *(palette_ptr++)) << 2, 0, 255 );
        palette->colors[i].g = (uint8_t) clamp( DARKEN( *(palette_ptr++)) << 2, 0, 255 );
        palette->colors[i].b = (uint8_t) clamp( DARKEN( *(palette_ptr++)) << 2, 0, 255 );
    }
    SDL_SetSurfacePalette( surface, palette );
}

void tk_port_sleep( int msec )
{
    tk_port_event_tick();
    SDL_Delay( msec );
}

/**
 * A clock that returns wallclock values in "vintage time", which are arbitrary ticks.
 * @return integer describing the current time.
 */
static uint64_t tk_port_nanoclock()
{
#if defined(TK_PORT_MAC)
    uint64_t now = mach_absolute_time();
    return (now - timer_zero) * timer_timebase_info.numer / timer_timebase_info.denom;
#elif defined(TK_PORT_POSIX)
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * TK_PORT_NSEC_PER_SEC + now.tv_nsec) - timer_zero;
#elif defined(TK_PORT_MSVC)
    // Not very accurate and wraps over if running for ~49 days but works
    return static_cast<uint64_t>(SDL_GetTicks()) * TK_PORT_NSEC_PER_MSEC;
#else
#error No nanoclock
#endif
}

int tk_port_vintage_clock()
{
    uint64_t nsec_time = tk_port_nanoclock();
    return (int) (nsec_time * VINTAGE_CLOCKS_PER_SEC / TK_PORT_NSEC_PER_SEC);
}

/**
 * Return the wallclock time tk_port_vintage_clock() is derived off.
 * @return
 */
double tk_port_debug_wallclock()
{
    uint64_t nsec_time = tk_port_nanoclock();
    return nsec_time / (double) TK_PORT_NSEC_PER_SEC;
}
