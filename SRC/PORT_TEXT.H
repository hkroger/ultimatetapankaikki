#ifndef ULTIMATETAPANKAIKKI_PORT_TEXT_H
#define ULTIMATETAPANKAIKKI_PORT_TEXT_H

#include <cstdint>

namespace tk_port
{


struct Font {
    unsigned int glyph_width;
    unsigned int glyph_height;
    bool glyph_nonempty[256];  // Whether the glyph is nonempty
    int glyph_left_zero[256];  // X position of first non-zero pixel
    int glyph_right_zero[256]; // X position of last non-zero pixel
    int glyph_left_one[256];   // X position of first pixel < 2
    int glyph_right_one[256];  // X position of last pixel < 2
    uint8_t *glyph_data[256];  // Bitmaps of size glyph_width * glyph_height
};

enum TextDrawMode {
    DoNotDrawText,
    DrawGradientText,
    DrawBitmapText,
};

int draw_text( Font *font, int x0, int y0, const char *str, int cq, TextDrawMode mode );

void load_png_font( Font *font, const char *png_name );

}
#endif //ULTIMATETAPANKAIKKI_PORT_TEXT_H
