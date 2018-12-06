#include "MISCFUNC_MSVC.h"
#include "PORT.H"
#include "MISCFUNC.h"

#include "windows.h"

void list_files_msvc( int *no, vile *filelist, const char *wildcard, int type )
{
    WIN32_FIND_DATA findData;
    HANDLE handle = FindFirstFile( wildcard, &findData );

    if ( handle != INVALID_HANDLE_VALUE )
    {
        do {
            if ( strcmp( ".", findData.cFileName ) &&
                strcmp( "..", findData.cFileName ) )
            {
                const bool isDirectory = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

                if ( ( type == _A_SUBDIR && isDirectory ) ||
                    ( type == _A_NORMAL && !isDirectory ) )
                {
                    strncpy( filelist[*no].name, findData.cFileName, 12 );
                    (*no)++;
                }
            }
        } while ( FindNextFile( handle, &findData ) );
        FindClose( handle );
    }
}
