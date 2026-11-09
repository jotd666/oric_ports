#include "oric_sound.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SOUNDS 10
#define RESERVED_SOUNDS 4


void sound_init(int oric_sound_flags)
{
 (void)oric_sound_flags;
}

void sound_close()
{}

int sound_load(const char *path, const char *name, const char *extension, int loop)
{
  (void)path, (void)name, (void)extension, (void)loop;
  
  return 0;
}

int sound_play(const char *sound_name)
{
  (void)sound_name;
  return 0;
}

void unload_sounds()
{}

void sound_stop(int i)
{
  (void)i;
}

void sound_stop_all()
{}

void sound_play_music(int channel, int oct_number, int note_number)
{
(void)channel,(void)oct_number,(void)note_number;
}

void sound_stop_music(int channel)
{
  (void)channel;
}

