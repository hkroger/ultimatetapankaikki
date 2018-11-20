#include <SDL.h>
#include "PORT.H"
#include "GLOBVAR.H"

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Surface *surface;
static SDL_Palette *palette;
static bool quit_flag = 0;


static void tk_port_screen_test();

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
	surface = SDL_CreateRGBSurfaceWithFormat( 0, 320, 200, 8, SDL_PIXELFORMAT_INDEX8 );
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


	while (!quit_flag)
	{
		tk_port_event_tick();
	}
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

void tk_port_event_tick( void )
{
	SDL_Event e;
	while (SDL_PollEvent( &e ))
	{
		if ( e.type == SDL_QUIT )
		{
			quit_flag = true;
		}
		if ( e.type == SDL_KEYDOWN )
		{
			if ( e.key.keysym.scancode == SDL_SCANCODE_ESCAPE )
			{
				quit_flag = true;
			}
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