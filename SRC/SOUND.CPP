#include <cstddef>
#include <SDL_mixer.h>
#include <SDL_log.h>
#include "SOUND.H"

struct _MIDASsample {
	Mix_Chunk *mix_chunk;
	char *path;
};

struct _MIDASmodule {
	Mix_Music *mix_music;
};


int MIDASstartup( void )
{
	if ((Mix_Init( MIX_INIT_MOD ) & MIX_INIT_MOD) != MIX_INIT_MOD )
	{
		SDL_Log( "Error initializing SDL_mixer:  %s", Mix_GetError());
		return false;
	}
	if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
	{
		printf( "Error initializing mixer API: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

int MIDASinit( void )
{
	return true;
}

int MIDASsetTimerCallbacks( uint16_t rate, int displaySync, void (*preVR)(), void (*immVR)(), void (*inVR)())
{
	return true;
}

int MIDASopenChannels( int numChannels )
{
	Mix_AllocateChannels( numChannels );
	return true;
}

int MIDASallocAutoEffectChannels( unsigned numChannels )
{
	return true;
}

int MIDASclose( void )
{
	Mix_CloseAudio();
	Mix_Quit();
	return true;
}

MIDASsample MIDASloadWaveSample( const char *fileName, int loopSample )
{
	MIDASsample smp = static_cast<MIDASsample>(calloc( 1, sizeof( _MIDASsample )));
	Mix_Chunk *chunk = Mix_LoadWAV( fileName );
	if ( !chunk )
	{
		SDL_Log( "Error loading sound file %s:  %s", fileName, Mix_GetError());
		return NULL;
	}
	smp->path = strdup( fileName );
	smp->mix_chunk = chunk;
	return smp;
}

MIDASmodule MIDASloadModule( const char *fileName )
{
	MIDASmodule mod = static_cast<MIDASmodule>(calloc( 1, sizeof( _MIDASmodule )));
	Mix_Music *music = Mix_LoadMUS( fileName );
	if ( !music )
	{
		SDL_Log( "Error loading music file %s:  %s", fileName, Mix_GetError());
		return NULL;
	}
	mod->mix_music = music;
	return mod;
}

MIDASsamplePlayHandle
MIDASplaySample( MIDASsample sample, unsigned channel, int priority, unsigned rate, unsigned volume, int panning )
{
	int mix_chan = Mix_PlayChannel( -1, sample->mix_chunk, 0 );
	if ( mix_chan == -1 )
	{
		printf( "Mix_PlayChannel: %s\n", Mix_GetError());
	}
	if ( rate != 22500 )
	{
		// TODO: Support this resampling
		SDL_Log( "sound %s is requested at non-standard rate %d, this is not supported", sample->path, rate );
	}
	int left_vol = panning * -4;  // -64 (leftmost panning) -> 256
	if ( left_vol >= 255 ) left_vol = 255;
	Mix_SetPanning( mix_chan, left_vol, 255 - left_vol );
	int mix_vol = volume * 2;  // original volume is from 0..63, SDL_mixer wants 0..127
	if ( mix_vol >= 127 ) mix_vol = 127;
	Mix_Volume( mix_chan, mix_vol );
	return mix_chan;
}

MIDASmodulePlayHandle MIDASplayModule( MIDASmodule module, int loopSong )
{
	Mix_HaltMusic();
	if ( Mix_PlayMusic( module->mix_music, loopSong ? -1 : 0 ) != 0 )
	{
		SDL_Log( "Error playing music:  %s", Mix_GetError());
		return 0;
	}
	return 1;
}

int MIDASstopModule( MIDASmodulePlayHandle playHandle )
{
	Mix_HaltMusic();
	return true;
}

int MIDASsetMusicVolume( MIDASmodulePlayHandle playHandle, unsigned volume )
{
	Mix_VolumeMusic( volume );  // TODO: is the range of MIDAS' volume 0..127?
	return true;
}
