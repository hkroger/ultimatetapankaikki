#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL_mixer.h>
#include <SDL_log.h>
#include "SOUND.H"

struct _MIDASsample {
    char *path;

    // Original audio
    SDL_AudioSpec spec;
    uint8_t *audio_buf;
    uint32_t audio_len;

    // Last resampled audio
    Mix_Chunk *mix_chunk;
    int mix_rate;
};

struct _MIDASmodule {
    Mix_Music *mix_music;
};

static bool started = false;

int MIDASstartup( void )
{
    if ((Mix_Init( MIX_INIT_MOD ) & MIX_INIT_MOD) != MIX_INIT_MOD )
    {
        SDL_Log( "Error initializing SDL_mixer:  %s", Mix_GetError());
        return false;
    }
    if ( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    {
        Mix_Quit();
        printf( "Error initializing mixer API: %s\n", Mix_GetError());
        return false;
    }
    started = true;
    return true;
}

int MIDASinit( void )
{
    if (!started) return false;
    return true;
}

int MIDASsetTimerCallbacks( uint16_t rate, int displaySync, void (*preVR)(), void (*immVR)(), void (*inVR)())
{
    if (!started) return false;
    return true;
}

int MIDASopenChannels( int numChannels )
{
    if (!started) return false;
    Mix_AllocateChannels( numChannels );
    return true;
}

int MIDASallocAutoEffectChannels( unsigned numChannels )
{
    if (!started) return false;
    return true;
}

int MIDASclose( void )
{
    if (!started) return false;
    Mix_CloseAudio();
    Mix_Quit();
    return true;
}

MIDASsample MIDASloadWaveSample( const char *fileName, int loopSample )
{
    if (!started) return NULL;
    MIDASsample smp = static_cast<MIDASsample>(calloc( 1, sizeof( _MIDASsample )));
    if (!SDL_LoadWAV(fileName, &smp->spec, &smp->audio_buf, &smp->audio_len))
    {
        SDL_Log( "Error loading sound file %s:  %s", fileName, SDL_GetError());
        return NULL;
    }
    smp->path = strdup( fileName );
    smp->mix_chunk = NULL;
    return smp;
}

MIDASmodule MIDASloadModule( const char *fileName )
{
    if (!started) return NULL;
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

static void resample_if_needed(MIDASsample sample, unsigned rate)
{
    if (sample->mix_chunk && sample->mix_rate == rate)
    {
        // Resampled audio already cached
        return;
    }

    if (sample->mix_chunk)
    {
        // Free the previous cached audio
        Mix_FreeChunk(sample->mix_chunk);
        sample->mix_chunk = NULL;
    }

    int dev_freq;
    uint16_t dev_format;
    int dev_channels;
    Mix_QuerySpec(&dev_freq, &dev_format, &dev_channels);

    // Convert from desired sampling rate to device sampling rate. This
    // resamples the original as if it already had the desired rate.

    SDL_AudioCVT cvt;
    SDL_BuildAudioCVT(&cvt,
                      sample->spec.format, sample->spec.channels, rate,
                      dev_format, dev_channels, dev_freq);

    cvt.len = sample->audio_len;
    cvt.buf = (uint8_t *)malloc(cvt.len * cvt.len_mult);
    memcpy(cvt.buf, sample->audio_buf, sample->audio_len);

    SDL_ConvertAudio(&cvt);

    sample->mix_chunk = Mix_QuickLoad_RAW(cvt.buf, cvt.len_cvt);
    sample->mix_rate = rate;
}

MIDASsamplePlayHandle
MIDASplaySample( MIDASsample sample, unsigned channel, int priority, unsigned rate, unsigned volume, int panning )
{
    if (!started) return 0;
    resample_if_needed(sample, rate);

    int mix_chan = Mix_PlayChannel( -1, sample->mix_chunk, 0 );
    if ( mix_chan == -1 )
    {
        printf( "Mix_PlayChannel: %s\n", Mix_GetError());
    }
    const int max_pan = 255;

    // -64..+64 -> 0..128 -> 0..256
    int right_vol = ( panning + MIDAS_PAN_RIGHT ) * 2;

    // Might cut 256 to 255 but effect is neglible and can't do much better
    // as Mix_SetPanning expects 0..255
    if ( right_vol > max_pan ) right_vol = max_pan;

    Mix_SetPanning( mix_chan, max_pan - right_vol, right_vol );
    int mix_vol = volume * 2;  // original volume is from 0..63, SDL_mixer wants 0..127
    if ( mix_vol >= 127 ) mix_vol = 127;
    Mix_Volume( mix_chan, mix_vol );
    return mix_chan;
}

MIDASmodulePlayHandle MIDASplayModule( MIDASmodule module, int loopSong )
{
    if (!started) return 0;
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
    if (!started) return true;
    Mix_HaltMusic();
    return true;
}

int MIDASsetMusicVolume( MIDASmodulePlayHandle playHandle, unsigned volume )
{
    if (!started) return true;

    // original volume is presumably from 0..64 and SDL_mixer wants 0..128
    Mix_VolumeMusic( volume * 2 );
    return true;
}
