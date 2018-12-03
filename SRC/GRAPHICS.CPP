#include "GRAPHICS.H"
#include "PORT.H"

void setpal(char palette[768], int brightness)
{
    tk_port_set_palette(palette, brightness);
    // Since setpal() is expected to update VGA registers and thus refresh the screen,
    // should be an okay place to tick (which also does present).
    tk_port_event_tick();
}
