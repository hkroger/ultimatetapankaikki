#include "GLOBVAR.H"
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <ERROR/ERROR.H>

static int get_font_chr(char c)
{
    return tolower(c);
}


void pixeli( int x, int y, char col )
{
    virbuff[( y * 320 )  + x] = col;
}

void loadfonts( const char *name )
{
    FILE *fnt;
    int a;
    fnt = fopen( name, "rb" );
    if ( !fnt ) error( "\nFile '%s' not found\n", name );
    f_x_size[FONT_NUM] = fgetc( fnt );
    f_y_size[FONT_NUM] = fgetc( fnt );
    thefonts[FONT_NUM] = (char *) malloc( f_y_size[FONT_NUM] * f_x_size[FONT_NUM] * 256 );
    fseek( fnt, 256 * 2, 0 );
    for ( a = 0; a < f_x_size[FONT_NUM] * f_y_size[FONT_NUM] * 256; a++ )
        thefonts[FONT_NUM][a] = (char) fgetc( fnt );
    fclose( fnt );
}

void writefonts( int x, int y, const char *str, int cq )
{
    int a, c, d, xpos1, xpos2, col, textcnt, fsize = f_x_size[FONT_NUM]*f_y_size[FONT_NUM];
    int x1, x2, px, py;
    textcnt = 0;
    for ( a = 0; str[a]!= 0; a ++  )
    {
        int chr = get_font_chr(str[a]);
        c = 0;
        col = 0;
        while ( col < 2 && c < f_x_size[FONT_NUM] )
        {
            // check from left
            for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
                if ( thefonts[FONT_NUM][chr * fsize + d * f_x_size[FONT_NUM] + (c)] > 1 ) col = 2;
            if ( col < 2 ) c++;
        }
        xpos1 = c;
        c = f_x_size[FONT_NUM];
        col = 0;
        while ( col < 2 && c > 0 )
        {
            // check from right
            for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
                if ( thefonts[FONT_NUM][chr * fsize + d * f_x_size[FONT_NUM] + (c - 1)] > 1 ) col = 2;
            if ( col < 2 ) c--;
        }
        xpos2 = c;
        if ( xpos1 > xpos2 )
        {
            xpos1 = 0;xpos2 = f_x_size[FONT_NUM] / 2;
        }
        x1 = xpos1 - 1;if ( x1 < 0 ) x1 = 0;
        x2 = xpos2 + 1;if ( x2 > f_x_size[FONT_NUM] ) x2 = f_x_size[FONT_NUM];
        for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
            for ( c = x1; c < x2; c++ )
            {
                px = (c - xpos1) + x + (textcnt);
                py = d + y;
                col = thefonts[FONT_NUM][chr * fsize + (d * f_x_size[FONT_NUM]) + c];
                if ( px >= 0 && px < 320 && py >= 0 && py < scr_y_size && col != 0 )
                {
                    if ( cq != 0 && col != 1 ) pixeli( px, py, (char) (cq + d));
                    else pixeli( px, py, 0 );
                }
            }
        textcnt += xpos2 - xpos1 + 1;
    }
};

void writefonts2( int x, int y, const char *str, int cq )
{
    int a, c, d, xpos1, xpos2, col, textcnt, fsize = f_x_size[FONT_NUM] * f_y_size[FONT_NUM];
    textcnt = 0;
    for ( a = 0; str[a] != 0; a++ )
    {
        int chr = get_font_chr(str[a]);
        c = 0;
        col = 0;
        while (col == 0 && c < f_x_size[FONT_NUM])
        {
            // check from left
            for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
                if ( thefonts[FONT_NUM][chr * fsize + d * f_x_size[FONT_NUM] + (c)] != 0 ) col = 1;
            if ( col == 0 ) c++;
        }
        xpos1 = c;
        c = f_x_size[FONT_NUM];
        col = 0;
        while (col == 0 && c > 0)
        {
            // check from right
            for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
                if ( thefonts[FONT_NUM][chr * fsize + d * f_x_size[FONT_NUM] + (c - 1)] != 0 ) col = 1;
            if ( col == 0 ) c--;
        }
        xpos2 = c;
        if ( xpos1 > xpos2 )
        {
            xpos1 = 0;
            xpos2 = f_x_size[FONT_NUM] / 2;
        }
        for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
            for ( c = xpos1; c < xpos2; c++ )
                if ( thefonts[FONT_NUM][chr * fsize + (d * f_x_size[FONT_NUM]) + c] != 0 )
                {
                    if ( cq != 0 )
                    {
                        pixeli((c - xpos1) + x + (textcnt), d + y,
                               thefonts[FONT_NUM][chr * fsize + (d * f_x_size[FONT_NUM]) + c] );
                    }
                    else
                    {
                        pixeli((c - xpos1) + x + (textcnt), d + y, 0 );
                    }
                }
        textcnt += xpos2 - xpos1 + 1;
    }
};

int str_length( const char *str )
{
    int length = 0;
    int a, c, d, col, xpos1, xpos2;
    for ( a = 0; str[a] != 0; a++ )
    {
        int chr = get_font_chr(str[a]);
        c = 0;
        col = 0;
        while (col < 2 && c < f_x_size[FONT_NUM])
        {
            // check from left
            for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
                if ( thefonts[FONT_NUM][chr * (f_x_size[FONT_NUM] * f_y_size[FONT_NUM]) + d * f_x_size[FONT_NUM] +
                                        (c)] > 1 )
                    col = 2;
            if ( col < 2 ) c++;
        };
        xpos1 = c;
        c = f_x_size[FONT_NUM];
        col = 0;
        while (col < 2 && c > 0)
        {
            // check from right
            for ( d = 0; d < f_y_size[FONT_NUM]; d++ )
                if ( thefonts[FONT_NUM][chr * (f_x_size[FONT_NUM] * f_y_size[FONT_NUM]) + d * f_x_size[FONT_NUM] +
                                        (c - 1)] > 1 )
                    col = 2;
            if ( col < 2 ) c--;
        };
        xpos2 = c;
        if ( xpos1 > xpos2 )
        {
            xpos1 = 0;
            xpos2 = f_x_size[FONT_NUM] / 2;
        }
        length += xpos2 - xpos1 + 1;
    }
    return (length);
}

void readline( int x, int y, int len, char *str, char *screen )
{
    int done = 0, a, dy;
    char *bg;
    int key;
    bg = (char *) malloc( 320 * f_y_size[FONT_NUM] );
    memcpy( bg, screen + x + (y * 320), 320 * f_y_size[FONT_NUM] );
    while (!done)
    {
        writefonts2( x, y, str, 1 );
        for ( dy = 0; dy < f_y_size[FONT_NUM]; dy++ )
            memcpy( screen + x + ((dy + y) * 320), virbuff + x + ((dy + y) * 320), len * f_x_size[FONT_NUM] );
        key = k::getch();
        if ( key != 27 && key != 8 && key != 13 )
            if ( strlen( str ) < (len - 1))
            {
                str[strlen( str ) + 1] = 0;
                str[strlen( str )] = (char) key;
            }
        if ( key == 27 )
        {
            str[0] = 0;
            done = 1;
        }
        if ( key == 13 )
        {
            done = 1;
        }
        if ( key == 8 )
        {
            if ( strlen( str ) > 0 )
                str[strlen( str ) - 1] = 0;
            a = str_length( str ) + x;
            for ( dy = 0; dy < f_y_size[FONT_NUM]; dy++ )
                memcpy( virbuff + a + ((dy + y) * 320), bg + (dy * 320) + str_length( str ), f_x_size[FONT_NUM] );
        }
    }
}
