#ifndef __TK__SOUND__
#define __TK__SOUND__

#include <cstdint>

#define TRUE 1
#define FALSE 0

enum MIDASpanning
{
    MIDAS_PAN_LEFT = -64,
    MIDAS_PAN_MIDDLE = 0,
    MIDAS_PAN_RIGHT = 64,
    MIDAS_PAN_SURROUND = 0x80
};


enum MIDASchannels
{
    MIDAS_CHANNEL_AUTO = 0xFFFF,
    MIDAS_ILLEGAL_CHANNEL = 0xFFFF
};

struct _MIDASsample;
struct _MIDASmodule;
typedef struct _MIDASsample *MIDASsample;
typedef struct _MIDASmodule *MIDASmodule;
typedef int MIDASmodulePlayHandle;
typedef int MIDASsamplePlayHandle;

int MIDASstartup( void );
int MIDASinit( void );
int MIDASsetTimerCallbacks( uint16_t rate, int displaySync, void (*preVR)(), void (*immVR)(), void (*inVR)());
int MIDASopenChannels( int numChannels );
int MIDASallocAutoEffectChannels( unsigned numChannels );
int MIDASclose( void );
MIDASsample MIDASloadWaveSample( const char *fileName, int loopSample );
MIDASmodule MIDASloadModule( const char *fileName );
MIDASsamplePlayHandle MIDASplaySample( MIDASsample sample, unsigned channel, int priority, unsigned rate, unsigned volume, int panning );
MIDASmodulePlayHandle MIDASplayModule( MIDASmodule module, int loopSong );
int MIDASstopModule( MIDASmodulePlayHandle playHandle );
int MIDASsetMusicVolume( MIDASmodulePlayHandle playHandle, unsigned volume );

#endif
