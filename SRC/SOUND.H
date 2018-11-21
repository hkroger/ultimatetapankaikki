#ifndef __TK__SOUND__
#define __TK__SOUND__

// TODO AUDIO

typedef void *MIDASsample;
typedef void *MIDASmodule;
typedef void *MIDASmodulePlayHandle;
typedef void *MIDASsamplePlayHandle;

#define MIDAS_CHANNEL_AUTO 0
#define MIDAS_PAN_MIDDLE 0

#define MIDASstartup(...) true
#define MIDASinit(...) true
#define MIDASsetTimerCallbacks(...) false
#define MIDASopenChannels(...) true
#define MIDASallocAutoEffectChannels(...) true
#define MIDASclose(...) NULL
#define MIDASloadWaveSample(...) NULL
#define MIDASloadModule(...) NULL
MIDASsamplePlayHandle MIDASplaySample(MIDASsample sample, unsigned channel, int priority, unsigned rate, unsigned volume, int panning);
#define MIDASplayModule(...) NULL
#define MIDASstopModule(...) NULL
#define MIDASsetMusicVolume(...) NULL

#endif
