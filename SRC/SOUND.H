#ifndef __TK__SOUND__
#define __TK__SOUND__

// TODO AUDIO

typedef void *MIDASsample;
typedef void *MIDASmodule;
typedef void *MIDASmodulePlayHandle;

#define MIDASstartup(...) true
#define MIDASloadConfig(...) true
#define MIDASinit(...) true
#define MIDASsetTimerCallbacks(...) false
#define MIDASopenChannels(...) true
#define MIDASallocAutoEffectChannels(...) true
#define MIDASclose(...)
#define MIDASloadWaveSample(...) NULL
#define MIDASloadModule(...) NULL
#define MIDASplaySample(...)
#define MIDASplayModule(...) NULL
#define MIDASstopModule(...)
#define MIDASsetMusicVolume(...)

#endif
