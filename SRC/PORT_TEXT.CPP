#include <ERROR/ERROR.H>
#include "PORT_IMAGE.H"
#include "PORT_TEXT.H"
#include "GLOBVAR.H"
#include <cctype>

#define INTER_CHARACTER_SPACING 2
#define SPACE_SPACING 1

namespace tk_port
{

static uint8_t get_font_chr( char c )
{
    return (uint8_t) tolower( c );
}

static void plot_pixel( int x, int y, char col )
{
    virbuff[(y * 320) + x] = col;
}


/**
 * Load a font from a PNG file into the font object provided.
 *
 * @param font Font object to populate.
 * @param png_name The PNG filename.
 */
void load_png_font( Font *font, const char *png_name )
{
    char buf[300 * 300] = {0};  // None of the shipped fonts are larger than this...
    int img_width, img_height;
    if ( !tk_port::load_png( png_name, buf, nullptr, &img_width, &img_height ))
    {
        error( "\nloading font %s: unable to load image\n", png_name );
    }
    unsigned int glyph_w = font->glyph_width = (unsigned int) ((img_width / 16) - 1);
    unsigned int glyph_h = font->glyph_height = (unsigned int) ((img_height / 16) - 1);

    for ( int glyph_index = 0; glyph_index < 256; glyph_index++ )
    {
        auto grid_y = (unsigned int) (glyph_index / 16);
        auto grid_x = (unsigned int) (glyph_index % 16);

        uint8_t *glyph_data = font->glyph_data[glyph_index] = (uint8_t *) malloc(
            font->glyph_width * font->glyph_height );
        if ( !glyph_data )
        {
            error( "\nloading font %s: unable to allocate memory for glyph %d\n", png_name, glyph_index );
        }
        for ( unsigned int y = 0; y < glyph_h; y++ )
        {
            unsigned int src_off = (grid_y * (glyph_h + 1) + y) * img_width + (grid_x * (glyph_w + 1));
            memcpy( glyph_data + y * glyph_w, buf + src_off, glyph_w );
        }
        bool empty = true;
        int left1 = glyph_w, right1 = 0;
        for ( int x = 0; x < (int) glyph_w; x++ )
        {
            for ( int y = 0; y < (int) glyph_h; y++ )
            {
                int ix = glyph_w - 1 - x;
                uint8_t *scanline = glyph_data + y * glyph_w;
                if ( scanline[x] ) empty = false;
                if ( scanline[x] > 1 && x > right1 ) right1 = x;
                if ( scanline[ix] > 1 && ix < left1 ) left1 = ix;
            }
        }
        font->glyph_nonempty[glyph_index] = !empty;
        font->glyph_left_one[glyph_index] = left1;
        font->glyph_right_one[glyph_index] = right1;
    }
}

/**
 * Get the metrics for a given glyph in the font.
 *
 * This has special casing (from the original code) for null glyphs such as undrawables
 * and actual bona-fide spaces; they are always drawn as half of the maximum glyph width.
 *
 *
 * @param font Font object.
 * @param chr Glyph index.
 * @param left Pointer to hold the leftmost pixel column to draw.
 * @param right Pointer to hold the rightmost pixel column to draw.
 * @return True if there is something to draw at all.
 */
static bool get_glyph_metrics( Font *font, uint8_t chr, int *left, int *right )
{
    if ( !font->glyph_nonempty[chr] )
    {
        *left = 0;
        *right = font->glyph_width / 2;
        return false;
    }
    *left = font->glyph_left_one[chr];
    *right = font->glyph_right_one[chr];
    return true;
}

/**
 * Internal workhorse function to draw or measure text onto the virbuff buffer.
 *
 * @param font Font object.
 * @param x0 Start X position.
 * @param y0 Start Y position.
 * @param str ASCII string to write.
 * @param cq Color modifier magic number.
 *           * In either draw mode, if this is 0, only the outlines of the glyph are drawn in color 0.
 *           * In Gradient mode, this is added to all plotted pixel values > 1.  Since palette entry 1
 *             is used as the outline color for the outlined font (8x8b), this leaves the outlines opaque.
 *           * In Bitmap mode, any other value than zero is ignored.
 * @param mode Draw mode.
 *             * DoNotDrawText: Don't actually touch `screen`, just measure the width of the text written.
 *             * DrawGradientText: Draw text in gradient mode, modulating the color values by the in-glyph Y
 *                                 coordinate and the cq value.
 *             * DrawBitmapText: Use the font's pixel values as-is, drawing bitmap text.
 * @return The final X coordinate.  This can be used in conjuction with DoNotDrawText and initial x0 = 0 to
 *         measure the width of text drawn.
 */
int draw_text( Font *font, int x0, int y0, const char *str, int cq, TextDrawMode mode )
{
    for ( int index = 0; str[index] != 0; index++ )
    {
        uint8_t chr = get_font_chr( str[index] );
        int xpos1, xpos2;
        bool must_draw = get_glyph_metrics( font, chr, &xpos1, &xpos2 );
        if ( !must_draw )
        {
            x0 += xpos2 - xpos1 + SPACE_SPACING;
            continue;
        }
        if ( mode != DoNotDrawText )
        {
            int x1 = xpos1 - 1;
            if ( x1 < 0 ) x1 = 0;
            int x2 = xpos2 + 2;
            if ( x2 > (int) font->glyph_width ) x2 = font->glyph_width;
            for ( unsigned int y = 0; y < font->glyph_height; y++ )
            {
                int py = y + y0;
                if ( py >= scr_y_size ) break;  // nothing to draw beyond this point
                for ( int x = x1; x < x2; x++ )
                {
                    int px = (x - xpos1) + x0;
                    if ( px >= 320 ) break;  // nothing to draw beyond this point

                    auto col = font->glyph_data[chr][(y * font->glyph_width) + x];
                    if ( px >= 0 && px < 320 && py >= 0 && py < scr_y_size && col != 0 )
                    {
                        if ( mode == DrawGradientText )
                        {
                            if ( cq != 0 && col != 1 )
                            {
                                plot_pixel( px, py, (uint8_t) (cq + y));
                            }
                            else
                            {
                                plot_pixel( px, py, 0 );
                            }
                        }
                        else if ( mode == DrawBitmapText )
                        {
                            if ( cq != 0 )
                            {
                                plot_pixel( px, py, col );
                            }
                            else
                            {
                                plot_pixel( px, py, 0 );
                            }

                        }
                    }
                }
            }
        }
        x0 += xpos2 - xpos1 + INTER_CHARACTER_SPACING;
    }
    return x0;
};

}
