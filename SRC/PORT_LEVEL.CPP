#include "DEFINES.H"
#include "TYPES.H"
#include "GLOBVAR.H"

/**
 * Get the level block at level coordinate px,py. This is _not_ the tile coordinate!
 * Returns null if out of bounds.
 */
BLOCK* get_block_at_coord( int px, int py ) {
    int tx = px / 20;
    int ty = py / 20;
    if(tx < 0 || tx >= level_x_size) return nullptr;
    if(ty < 0 || ty >= level_y_size) return nullptr;
    return &level[ty * level_x_size + tx];
}

/**
 * Get the tile type at coordinate px / py. This is _not_ the tile coordinate!
 * This may also return OUTOFBOUNDS if the coordinate is out of bounds.
 */
int get_block_type_at_coord( int px, int py ) {
    BLOCK *b = get_block_at_coord(px, py);
    return (b != nullptr ? b->type : OUTOFBOUNDS);
}
