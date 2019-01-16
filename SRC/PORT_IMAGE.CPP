#include <cstring>
#include <cstdlib>
#include <SDL_surface.h>
#include <SDL_image.h>
#include <ERROR/ERROR.H>

namespace tk_port
{
/**
 * Replace the extension in the path `name` with `.png`.
 *
 * The return value is a static buffer, so the caller does not need
 * to bother with memory management.  This function is not reentrant.
 *
 * @param name Path to (probably) an EFP name
 * @return PNG path
 */
static char *replace_extension_with_png( const char *name )
{
    static char png_name[80];
    strncpy( png_name, name, 80 );
    size_t len = strlen( png_name );
    if ( len >= 3 )
    {
        png_name[len - 3] = 'p';
        png_name[len - 2] = 'n';
        png_name[len - 1] = 'g';
    }
    return png_name;
}

/**
 * Load the image data and/or the palette data from a PNG.
 * @param png_name PNG path.
 * @param kuva Optional pointer to receive the image data.
 *             Must be large enough to accommodate it; no bounds checking is or can be done.
 * @param paletti Optional pointer to a 768-byte char array to receive the palette data.
 *                The PNG's palette, ranging from 0..255 is rescaled down to 0..63 to match
 *                VGA hardware and other assumptions in the code.
 * @return True if the load succeeded, False if not.
 */
bool load_png( const char *png_name, char *kuva, char *paletti )
{
    SDL_RWops *rwop = SDL_RWFromFile( png_name, "rb" );
    if ( !rwop )
    {
        // assume the file just didn't exist
        return false;
    }
    SDL_Surface *image = IMG_LoadPNG_RW( rwop );
    SDL_FreeRW( rwop );
    if ( !image )
    {
        error( "load_png(%s): IMG_LoadPNG_RW says %s\n", png_name, IMG_GetError());
        return false;
    }
    const SDL_Palette *palette = image->format->palette;

    if ( !palette )
    {
        SDL_FreeSurface( image );
        error( "load_png(%s): image not palettized", png_name );
        return false;
    }

    if ( kuva != nullptr )  // Interested in image data?
    {
        // some images are not loaded in fully linear mode (notably those with width < 8)
        // so we need to copy scanline by scanline in those cases
        if ( image->pitch != image->w )
        {
            for ( unsigned int y = 0; y < (unsigned int) image->h; y++ )
            {
                size_t in_off = y * image->pitch;
                size_t out_off = y * image->w;
                memcpy( kuva + out_off, kuva + in_off, image->pitch );
            }
        }
        else
        {
            size_t n_bytes = ((size_t) image->w) * ((size_t) image->h);
            memcpy( kuva, image->pixels, n_bytes );
        }
    }

    if ( paletti != nullptr )  // Interested in palette data?
    {
        char *pal_p = paletti;
        for ( int i = 0; i < palette->ncolors; i++ )
        {
            // scale colors down to 0..63
            *pal_p++ = (palette->colors[i].r >> 2);
            *pal_p++ = (palette->colors[i].g >> 2);
            *pal_p++ = (palette->colors[i].b >> 2);
        }
    }
    SDL_FreeSurface( image );
    return true;
}

bool load_efp_replacement_png( const char *efp_name, char *kuva, char *paletti )
{
    const char *png_name = replace_extension_with_png( efp_name );
    return load_png( png_name, kuva, paletti );
}

}