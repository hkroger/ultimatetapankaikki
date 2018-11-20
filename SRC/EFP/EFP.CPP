#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "EFP.H"
#include "../ERROR/ERROR.H"

void load_efp( const char *name, char *kuva, int offs )
{

	char header[7];
	unsigned int p;
	unsigned short w, h;

	short tavu, tavu2;
	FILE *efp = fopen( name, "rb" );

	if ( !efp )
	{
		error( "File %s is missing\n", name );
	}
	fseek( efp, 0, SEEK_END );
	unsigned int file_len = ftell( efp );
	fseek( efp, offs, SEEK_SET );
	uint8_t *file_data = (uint8_t *) calloc(file_len - offs, sizeof( uint8_t ));
	if ( !file_data )
	{
		error( "load_efp(): unable to allocate %d bytes to read %s", file_len - offs, name );
	}
	fread( file_data, sizeof( uint8_t ), file_len - offs, efp );
	uint8_t *data_p = file_data;

	if ( strncmp((const char *) data_p, "EF pic", 6 ))
	{
		error( "File is not an EF pic!\n" );
	}

	data_p += 6;
	w = ((unsigned short *) data_p)[0];
	h = ((unsigned short *) data_p)[1];
	data_p += 4;

	for ( p = 0; p < w * h; )
	{
		tavu = *data_p++;
		if ( tavu > 192 )
		{
			tavu2 = *data_p++;
			memset( kuva + p, tavu2, tavu - 192 );
			p += tavu - 192;
		} else
		{
			kuva[p++] = (char) tavu;
		}
	}
	fclose( efp );
	free( file_data );

}


void load_efp_pal( const char *name, char *paletti )
{
	FILE *efp = fopen( name, "rb" );
	fseek( efp, -768, SEEK_END );
	fread( paletti, sizeof(char), 768, efp );
	fclose( efp );
}

