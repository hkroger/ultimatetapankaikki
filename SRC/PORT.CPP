#define _PORT_IMPL

#include <SDL.h>
#include "PORT.H"
#include "GLOBVAR.H"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Palette *palette;
bool tk_port_quit_flag = 0;

#ifdef TK_PORT_POSIX
static struct timespec timer_zero;
#endif
#ifdef TK_PORT_MAC

#include <mach/mach_time.h>

static mach_timebase_info_data_t timer_timebase_info;
static uint64_t timer_zero = 0;
#endif


static void tk_port_screen_test();

static Uint32 tk_port_update_framecount( Uint32 interval, void *param )
{
	framecount++;
	// Return the next interval to be used. This technically allows
	// for dynamic speed changes during the game by mutating the `target_frames`
	// global...
	return target_frames;
}

int tk_port_init( void )
{
	if ( SDL_Init(SDL_INIT_EVERYTHING) != 0 )
	{
		SDL_Log( "Error initializing SDL:  %s", SDL_GetError());
		return 1;
	}

	window = SDL_CreateWindow(
			"TK",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 600,
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
	surface = SDL_CreateRGBSurface( 0, 320, 200, 8, 0, 0, 0, 0 );
	if ( surface == NULL )
	{
		SDL_Log( "SDL_CreateRGBSurfaceWithFormat() failed: %s", SDL_GetError());
		return 4;
	}
	if ( surface->pitch != 320 )
	{
		// TODO: Support this mode maybe?
		SDL_Log( "Was unable to get a surface with the correct linear pitch 320" );
		return 5;
	}
	palette = SDL_AllocPalette( 256 );

	// Now that we have a linear 320x200x8 surface, assign it as our "VGA 0xA0000"
	// all other code can write into.
	screen = (char *) surface->pixels;

	tk_port_screen_test();
	tk_port_event_tick();  // Tick once so we get the window up at this point

	// Set up the timer to update the framecount global.
	// According to https://github.com/OpenEmu/VICE-Core/blob/ff96ffd72cb223216a5705d3824f1b9385008ea9/vice/src/arch/msdos/vsync.c#L223-L224
	// MIDASsetTimerCallbacks() takes its rate parameter as kHz; the original call in
	// do_all() in GAME.CPP is MIDASsetTimerCallbacks( target_frames*1000 , ...),
	// so presumably target_frames is in hz.
	SDL_AddTimer( target_frames, tk_port_update_framecount, NULL );

#ifdef TK_PORT_POSIX
	clock_gettime(CLOCK_MONOTONIC, &timer_zero);
#endif
#ifdef TK_PORT_MAC
	mach_timebase_info( &timer_timebase_info );
	timer_zero = mach_absolute_time();
#endif
	return 0;
}

static void tk_port_screen_test()
{
	for ( int i = 0; i < 256; i++ )
	{
		palette->colors[i].r = palette->colors[i].g = palette->colors[i].b = (Uint8) i;
	}

	SDL_SetSurfacePalette( surface, palette );

	for ( int y = 0; y < 200; y++ )
	{
		for ( int x = 0; x < 320; x++ )
		{
			screen[y * 320 + x] = (char) ((x & 0xFF) ^ (y & 0xFF));
		}
	}
}

void tk_port_exit( void )
{
	SDL_Quit();
}

static uint8_t tk_port_remap_key( SDL_Keycode keycode, SDL_Scancode scancode )
{
	// TODO: add all special characters here
	switch(scancode) {
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
	switch(keycode) {
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
				if ( down && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE && e.key.keysym.mod & KMOD_LSHIFT)
				{
					tk_port_quit_flag = true;
					exit(42);
				}
				uint8_t tk_key = tk_port_remap_key( e.key.keysym.sym, e.key.keysym.scancode );
				k.state[tk_key] = down;
				if ( down )
				{
					//printf("key down: %04x (tk = %d)\n", e.key.keysym.scancode, tk_key);
					k._last_getch = tk_key;
					k._kbhit_flag = true;
				}
			}
				break;
		}
	}
	tk_port_present_screen();
}

void tk_port_present_screen( void )
{
	SDL_Texture *surface_texture = SDL_CreateTextureFromSurface( renderer, surface );
	SDL_RenderCopy( renderer, surface_texture, NULL, NULL );
	SDL_RenderPresent( renderer );
	SDL_DestroyTexture( surface_texture );
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
		// The left-shifts below scale the assumedly 0..63 VGA palette
		// (ref. http://joco.homeserver.hu/vgalessons/lesson6.html)
		// up to the 0..255 expected by SDL.
		#define DARKEN(c) clamp(((int)(c) - 255 + brightness), 0, 255)
		palette->colors[i].r = clamp(DARKEN(*(palette_ptr++)) << 2, 0, 255 );
		palette->colors[i].g = clamp(DARKEN(*(palette_ptr++)) << 2, 0, 255 );
		palette->colors[i].b = clamp(DARKEN(*(palette_ptr++)) << 2, 0, 255 );
	}
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
#define TK_PORT_NSEC_PER_SEC 1000000000L

static uint64_t tk_port_nanoclock()
{
#if defined(TK_PORT_MAC)
	uint64_t now = mach_absolute_time();
	return (now - timer_zero) * timer_timebase_info.numer / timer_timebase_info.denom;
#elif defined(TK_PORT_POSIX)
	// TODO: This code is untested!
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC, &now);
	return ( now.tv_sec - timer_zero.tv_sec ) * TK_PORT_NSEC_PER_SEC + ( now.tv_nsec - timer_zero.tv_nsec );
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
