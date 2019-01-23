#include "GLOBVAR.H"
#include "PORT_TEXT.H"
#include "PORT_IMAGE.H"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include "ERROR/ERROR.H"
#include "INPUT/INPUT.H"
#include "MISCFUNC.H"


void loadfonts( const char *name )
{
    const char *png_name = tk_port::replace_extension_with_png( data_path( name ).c_str() );
    tk_port::load_png_font( &thefonts[FONT_NUM], png_name );
}

void writefonts( int x, int y, const char *str, int cq )
{
    tk_port::draw_text( &thefonts[FONT_NUM], x, y, str, cq, tk_port::DrawGradientText );
}

void writefonts2( int x, int y, const char *str, int cq )
{
    tk_port::draw_text( &thefonts[FONT_NUM], x, y, str, cq, tk_port::DrawBitmapText );
}

int str_length( const char *str )
{
    return tk_port::draw_text( &thefonts[FONT_NUM], 0, 0, str, 0, tk_port::DoNotDrawText );
}

void readline( int x, int y, int len, char *str, char *screen, bool (*filter_char)( char ))
{
    int done = 0, a, dy;
    char *bg;
    i::Input input;
    tk_port::Font *f = &thefonts[FONT_NUM];
    bg = (char *) malloc( 320 * f->glyph_height );
    memcpy( bg, screen + x + (y * 320), 320 * f->glyph_height );
    while (!done)
    {
        writefonts2( x, y, str, 1 );
        for ( dy = 0; dy < (int)f->glyph_height; dy++ )
            memcpy( screen + x + ((dy + y) * 320), virbuff + x + ((dy + y) * 320), len * f->glyph_width );
        input = i::wait_for_keypress();
        if ( input != k::ESC && input != k::BACKSPACE && input != k::ENTER && input.get_type() == i::Input::Keyboard )
            if ((int) strlen( str ) < (len - 1))
            {
                char key = k::scancode_to_char( input.scancode );
                if ( filter_char( key ))
                {
                    str[strlen( str ) + 1] = 0;
                    str[strlen( str )] = key;
                }
            }
        if ( input == k::ESC )
        {
            str[0] = 0;
            done = 1;
        }
        if ( input == k::ENTER )
        {
            done = 1;
        }
        if ( input == k::BACKSPACE )
        {
            if ( strlen( str ) > 0 )
                str[strlen( str ) - 1] = 0;
            a = str_length( str ) + x;
            for ( dy = 0; dy < (int)f->glyph_height; dy++ )
                memcpy( virbuff + a + ((dy + y) * 320), bg + (dy * 320) + str_length( str ), f->glyph_width );
        }
    }
}
