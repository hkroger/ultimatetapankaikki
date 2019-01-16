#ifndef ULTIMATETAPANKAIKKI_PORT_IMAGE_H
#define ULTIMATETAPANKAIKKI_PORT_IMAGE_H
namespace tk_port
{
    bool load_png( const char *png_name, char *kuva, char *paletti );

    bool load_efp_replacement_png( const char *efp_name, char *kuva, char *paletti );
}
#endif //ULTIMATETAPANKAIKKI_PORT_IMAGE_H
