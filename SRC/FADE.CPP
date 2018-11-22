#include "GLOBVAR.H"
#include "GRAPHICS.H"
#include <cmath>
#include <cstdlib>
#include <cstring>

#define max( a, b ) ((a) > (b) ? (a) : (b))

int sine[256];
void Draw_Phase( int phase, char *image, char *pal )
{
    int x, y, yfix, xfix, yg, l;
    l = ( phase*30 )  >> 8;
    for ( y = 0, yg = 0; y < scr_y_size; y ++  )
    {
        yfix = 320*( ( l*sine[( y + phase ) &0xff] )  >> 8 );
        if ( yfix + yg < 0||yfix + yg >= 320*scr_y_size ) memset( screen + yg, 0, 320 );
        else for ( x = 0; x < 320; x ++  )
        {
            xfix = x + ( ( l*sine[( x + phase ) &0xff] )  >> 8 );
            if ( xfix > 0&&xfix < 320 ) screen[yg + x] = image[yg + xfix + yfix];
            else screen[yg + x] = 0;
        }
        yg += 320;
    }
    setpal( pal, max( 256 - phase, 0 )  );
}

#define FADE_SPEED 10
void fadein( char *image, char *pal )
{
    int old_c = 0, alku;
    setpal( pal, 0 );
    alku = framecount;
    while( alku + ( 256 / FADE_SPEED )  > framecount )
    {
        while ( framecount == old_c );
        old_c = framecount;
        Draw_Phase( 256 - ( framecount - alku ) *FADE_SPEED, image, pal );
    }
    setpal( pal, 256 );
    memcpy( screen, image, 320*scr_y_size );
    tk_port_event_tick();
}

void fadeout( char *image, char *pal )
{
    int alku, old_c;
    alku = old_c = framecount;
    while( alku + ( 256 / FADE_SPEED )  > framecount )
    {
        while ( framecount == old_c );
        old_c = framecount;
        Draw_Phase( ( framecount - alku ) *FADE_SPEED, image, pal );
    }
    setpal( pal, 0 );
    memset( screen, 0, 320*scr_y_size );
    tk_port_event_tick();
}

void CalCuLaTE_Sine_Table_AND_Be_Happy()
{
    int a;
    for ( a = 0; a < 256; a ++  )
    sine[a] = 255*( sin( ( float ) a / 256*pi*2 )  );
}
