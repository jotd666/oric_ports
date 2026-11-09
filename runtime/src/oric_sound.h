#ifndef _ORIC_SOUND
#define _ORIC_SOUND


#define ORIC_SIMPLE_SDL 0
#define ORIC_SIMPLE_SDL_MUSIC 1
#define ORIC_SDL_MIXER 2

void sound_init(int oric_sound_flags);
void sound_close();
int sound_load(const char *path, const char *name, const char *extension, int loop);
int sound_play(const char *sound_name);
void unload_sounds();
void sound_stop(int i);
void sound_stop_all();

void sound_play_music(int channel, int oct_number, int note_number);
void sound_stop_music(int channel);

#endif
