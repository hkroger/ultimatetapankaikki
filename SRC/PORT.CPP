#define _PORT_IMPL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory>
#include <vector>
#include <SDL.h>
#include "PORT.H"
#include "GLOBVAR.H"
#include "ERROR/ERROR.H"
#include "CONTROLLER.H"
#include "INPUT.H"

extern int scr_y_size;

namespace tk_port
{
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *surface = NULL;
SDL_Palette *palette = NULL;
SDL_Rect render_dest_rect;
bool window_resized;
bool quit_flag = false;
uint32_t debug = 0;
uint64_t timer_zero;
bool full_screen = false;
std::vector< std::unique_ptr<Controller> > controllers;
const unsigned int max_number_of_controllers = 2;

#define TK_PORT_NSEC_PER_SEC 1000000000L
#define TK_PORT_NSEC_PER_MSEC 1000000L
#define TK_PORT_GRAPHICS_SCALE 3

#ifdef TK_PORT_MAC
#include <mach/mach_time.h>
mach_timebase_info_data_t timer_timebase_info;
#endif

const unsigned int timer_interval_ms = 1;
unsigned int sub_framecount = 0;
unsigned int last_frame_tick = 0;

Uint32 update_framecount( Uint32 interval, void *param )
{
    // Set the target interval to be used. This technically allows
    // for dynamic speed changes during the game by mutating the `target_frames`
    // global...
    const unsigned int frame_interval_ms = 1000 / target_frames;
    const unsigned int now = SDL_GetTicks();
    sub_framecount += now - last_frame_tick;
    last_frame_tick = now;

    while ( sub_framecount >= frame_interval_ms )
    {
        sub_framecount -= frame_interval_ms;
        framecount++;
    }

    return interval;
}

static void read_tk_port_debug()
{
    const char *tk_port_debug_s = getenv( "TK_PORT_DEBUG" );
    if ( tk_port_debug_s == NULL )
    {
        return;
    }
    debug = strtoul( tk_port_debug_s, NULL, 10 );
    if ( debug != 0 )
    {
        SDL_Log( "tk_port_debug = %d (%08x)", debug, debug );
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
    if ( surface )
    {
        SDL_FreeSurface(surface);
    }

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
    set_palette( screen, 0 );
}

inline void refresh_rendering()
{
    get_render_rect( &render_dest_rect );
}

int init_graphics()
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

    // Set screen clearing color
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, SDL_ALPHA_OPAQUE );

    refresh_rendering();

    window_resized = false;

    const int s_ret = create_surface();
    if ( s_ret ) return s_ret;

    palette = SDL_AllocPalette( 256 );

    init_screen();

    return 0;
}

void init_time()
{
    // Set up the timer to update the framecount global.
    last_frame_tick = SDL_GetTicks();
    SDL_AddTimer( timer_interval_ms, update_framecount, NULL );

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

void change_resolution( const unsigned int y )
{
    scr_y_size = y;

    refresh_rendering();

    const int s_ret = create_surface();
    if ( s_ret ) error( "Failed to create new surface\n" );

    init_screen();
}

void init_controllers( void )
{
    for ( int i = 0; i < SDL_NumJoysticks(); ++i )
    {
        if ( SDL_IsGameController( i ) )
        {
            controllers.emplace_back( new Controller(i) );
            if ( controllers.size() == max_number_of_controllers )
                break;
        }
    }
}

int init()
{
    int ret;
    read_tk_port_debug();
    if ( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER ) != 0 )
    {
        SDL_Log( "Error initializing SDL:  %s", SDL_GetError());
        return 1;
    }
    if((ret = init_graphics()) != 0) {
        return ret;
    }
    init_time();
    init_controllers();
    event_tick();  // Tick once so we get the window up at this point
    return 0;
}

void deinit()
{
    // Uninitialize SDL controllers before main SDL
    controllers.clear();

    if ( surface ) SDL_FreeSurface( surface );
    if ( palette ) SDL_FreePalette( palette );
    if ( renderer ) SDL_DestroyRenderer( renderer );
    if ( window ) SDL_DestroyWindow( window );

    SDL_Quit();
}

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

void present_screen( void )
{
    if ( window_resized )
    {
        refresh_rendering();
        window_resized = false;
    }

    SDL_RenderClear( renderer );

    SDL_Texture *surface_texture = SDL_CreateTextureFromSurface( renderer, surface );
    SDL_RenderCopy( renderer, surface_texture, NULL, &render_dest_rect );
    SDL_RenderPresent( renderer );
    SDL_DestroyTexture( surface_texture );
}

int clamp( int val, int min, int max )
{
    if ( val < min ) return min;
    if ( val > max ) return max;
    return val;
}

void frame_rate_limiter( const unsigned int max_fps )
{
    static unsigned int frame_start_time = SDL_GetTicks();

    const unsigned int max_delay = 1000 / max_fps;
    const unsigned int since_last_frame = SDL_GetTicks() - frame_start_time;
    const unsigned int delay = clamp( max_delay - since_last_frame, 0, max_delay );

    if ( delay > 0 )
    {
        SDL_Delay( delay );
    }

    frame_start_time = SDL_GetTicks();
}

void handle_controller_input( void )
{
    for ( unsigned int i = 0; i < controllers.size(); ++i )
    {
        const auto& controller = controllers[i];

        if ( !controller->isConnected() )
        {
            // Logic could be improved so that disconnected controllers are reconnected. For now
            // just ignore such controllers until end of the session.
            continue;
        }

        const auto set_key_if = [&controller]( const SDL_GameControllerButton& controller_button, const ControllerInput& button )
        {
            if ( controller->isButtonPressed( controller_button ) )
            {
                keyb::pressed( button, SDL_GetTicks() );
            }
            else if (controller->isButtonReleased( controller_button ) )
            {
                keyb::released( button );
            }
        };

        for ( const auto& key_pair : i == 0 ? s_key_map_p1 : s_key_map_p2 )
        {
            set_key_if( key_pair.controller, key_pair.input );
        }
    }
}

void event_tick( void )
{
    // Controllers emulate keyboard presses so any controller action is actually handled in the
    // keyboard loop below
    handle_controller_input();

    SDL_Event e;
    while (SDL_PollEvent( &e ))
    {
        tk_port::keyb::handle_event(&e);

        switch (e.type)
        {
            case SDL_QUIT:
                if ( quit_flag )
                {
                    // Exit with force if app didn't quit with first press. App doesn't
                    // poll for quit_flag in many places.
                    exit( 1 );
                }
                quit_flag = true;
                break;

            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED)
                {
                    window_resized = true;
                }
                break;
        }
    }
    present_screen();

    // To avoid running in busy loop taking all the CPU time,
    // limit the speed to nice multiple of target frame rate
    frame_rate_limiter( target_frames * 4 );
}

void save_screenshot( const char* path )
{
    SDL_SaveBMP( surface, path );
}

void set_palette( char palette_entries[768], int brightness )
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

void sleep( const int msec )
{
    event_tick();
    SDL_Delay( msec );
}

/**
 * A clock that returns wallclock values in "vintage time", which are arbitrary ticks.
 * @return integer describing the current time.
 */
static uint64_t nanoclock()
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

int vintage_clock()
{
    uint64_t nsec_time = nanoclock();
    return (int) (nsec_time * VINTAGE_CLOCKS_PER_SEC / TK_PORT_NSEC_PER_SEC);
}

void toggle_fullscreen()
{
    full_screen = !full_screen;

    // Borderless is used as it is very quick compared to real fullscreen. Latter
    // has very slow and ugly transition (away from fullscreen) at least on some
    // Windows 10 setups
    SDL_SetWindowFullscreen( window, full_screen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0 );
    window_resized = true;
}

/**
 * Return the wallclock time vintage_clock() is derived off.
 * @return
 */
double debug_wallclock()
{
    uint64_t nsec_time = nanoclock();
    return nsec_time / (double) TK_PORT_NSEC_PER_SEC;
}
}
