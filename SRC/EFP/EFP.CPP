#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include "EFP.H"
#include "../ERROR/ERROR.H"
#include "PORT_IMAGE.H"

void load_efp( const char *name, char *kuva, int offs )
{
    if ( !tk_port::load_efp_replacement_png( name, kuva, nullptr ))
    {
        error("Unable to load PNG replacement for %s\n", name);
    }
}


void load_efp_pal( const char *name, char *paletti )
{
    if ( !tk_port::load_efp_replacement_png( name, nullptr, paletti ))
    {
        error("Unable to load palette from PNG replacement for %s\n", name);
    }
}
