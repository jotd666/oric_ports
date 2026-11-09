#include "oric_sound.h"
#include "oric_basic.h"  // for some utiltiles
#include "oric_video.h"  // for some utilities (not video!!)
#include "SDL/SDL.h"
#include "SDL/SDL_audio.h"
#ifdef USE_SDL_MIX
#include "SDL/SDL_mixer.h"
#else
typedef void Mix_Chunk;
#endif

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define NUM_SOUNDS 10
#define RESERVED_SOUNDS 4


int audio_open = 0;

int sound_flags = 0;

SDL_AudioSpec fmt;

struct Sample_;

typedef struct Sample_ {
    Uint8 *data;
    Uint32 dpos;
    Uint32 dlen;
    int    loop;
} Sample;

#define NUM_OCTAVES 7

typedef struct {
    Sample note[12];
    double freq[12];
} Octave;

static Octave octave[NUM_OCTAVES];

static Sample sounds[NUM_SOUNDS];

struct SampleNode_;


typedef struct SampleNode_
{
    SDL_AudioCVT cvt;
    char name[50];
    struct SampleNode_ *next;
  int loop;

  Mix_Chunk *chunk;

} SampleNode;

SampleNode *sample_node = 0;

static int is_sdl_mix()
{
    return sound_flags & ORIC_SDL_MIXER;
}

static void mixaudio(void *unused, Uint8 *stream, int len)
{
   int i;
    Uint32 amount;
    Sample *s;
  (void)unused;
  
 /*
  (void)stream;
  (void)len;
  return;*/
  
    for ( i=0; i<NUM_SOUNDS; ++i )
    {
	s = sounds+i;
      if (s->dpos<s->dlen)
	{
        amount = (s->dlen-s->dpos);
        if ( amount > (Uint32)len )
	{
            amount = len;
        }
	if (amount > 0)
	{
	  SDL_MixAudio(stream, &s->data[s->dpos],
		       amount, SDL_MIX_MAXVOLUME);
	  if (s->loop)
	    {
	      s->dpos = (s->dpos + amount) % s->dlen;
	    }
	  else
	    {
	      s->dpos += amount;
	    }
	  
	    }
	}
      else
	{
	  // stop
	  s->dpos = s->dlen;
	}
      
    }
}
void sound_stop_music(int channel)
{
    sound_stop(NUM_SOUNDS - RESERVED_SOUNDS + channel - 1);
}

void sound_play_music(int channel, int oct_number, int note_number)
{
  if (oct_number>=NUM_OCTAVES || oct_number<0 || note_number>12 || note_number<1)
    {
      error("MUSIC arguments out of range. octave=",str$(oct_number)," note=",str$(note_number),0);
    }
  
  SDL_LockAudio();
  Octave *o = octave+(oct_number);
  Sample *s = o->note+(note_number-1);

  sounds[NUM_SOUNDS - RESERVED_SOUNDS + channel - 1] = *s;
  SDL_UnlockAudio();
}

void music_init()
{

    // init music

    int i,j,k;

    double c_freq = 55 * pow(2,-0.75);

    double pi = atan(1) * 4;
    
    for (i = 0; i < NUM_OCTAVES; i++)
    {
	Octave *o = octave+i;

	double oc_freq = c_freq * (1<<i);
	
	for (j=0; j < 12; j++)
	{
	  double n_freq = oc_freq * pow(2,j/12.);
	  Sample *s = o->note+j;
	  double divisor = fmt.freq / n_freq;
	    
	  int len = (int)round(divisor);
	    
	  int nb_loops = 400000/len; // not enough loops = crappy sound
	     
	  o->freq[j] = n_freq; // debug

	  short *tmp_buf = (short*)malloc(len*nb_loops*sizeof(short));	    	    
	  for (k=0; k < len; k++)
	    {
	      float phase = k/(float)len;
	      tmp_buf[k] = (short)round(20000 * sin( (2 * pi * phase)));
	    }	
	  for (k=1;k<nb_loops;k++)
	    {
	      memcpy(tmp_buf+(k*len),tmp_buf,len);	      
	    }
	  
	  int sample_len = nb_loops * len * sizeof(short);
	  SDL_AudioCVT cvt;

	  /*
	    {
		char b[100];
		sprintf(b,"oct_%d_note_%d.raw",i,j);
		FILE *f = fopen(b,"wb");
		fwrite(tmp_buf,len,1,f);
		fclose(f);
		}*/
	    
	    SDL_BuildAudioCVT(&cvt, AUDIO_S16, 1, fmt.freq,
			      fmt.format,   fmt.channels,   fmt.freq);
	    
	    cvt.buf = malloc(sample_len*cvt.len_mult);
	    memcpy(cvt.buf, tmp_buf, sample_len);
	    cvt.len = sample_len;
	    SDL_ConvertAudio(&cvt);
	    
	    s->data = cvt.buf;
	    s->dpos = 0;
	    s->dlen = sample_len*cvt.len_mult;
	    s->loop = 1;
	    
	    free(tmp_buf);

	}
    }
}

void sound_init(int oric_sound_flags)
{
    sound_flags = oric_sound_flags;
    
    /* Set 16-bit stereo audio at 22Khz */
    fmt.freq = 44100;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 512;        /* A good value for games */
    fmt.callback = &mixaudio;
    fmt.userdata = NULL;

  // zero the sounds struct array
  memset(&sounds,0,sizeof(sounds));
  
#ifdef USE_SDL_MIX
    if (is_sdl_mix())
    {	
    
       if(Mix_OpenAudio(fmt.freq,MIX_DEFAULT_FORMAT,fmt.channels,fmt.samples)<0)
    {
        fprintf(stderr, "Unable to open audio: %s\n", Mix_GetError());
	fflush(stderr);
        Mix_CloseAudio();
        SDL_Quit();
        exit(1);
    }
       audio_open = 1;
       sample_node = 0;
    }
    else
#endif
    {

    /* Open the audio device and start playing sound! */
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(1);
    }

    audio_open = 1;
    sample_node = 0;

    if (oric_sound_flags & ORIC_SIMPLE_SDL_MUSIC)
    {
	music_init();
    }
    
    SDL_PauseAudio(0);
    }

}

void sound_close()
{
    if (audio_open)
    {
	//sound_stop();
	if (!is_sdl_mix())
	{
	SDL_PauseAudio(0);
	SDL_CloseAudio();
	audio_open = 0;

	SampleNode *sn = sample_node;

	// free sample nodes
	
	while (sn != 0)
	{
	    SampleNode *to_free = sn;
	    sn = sn->next;
	    free(to_free->cvt.buf);
	    free(to_free);
	}

	sample_node = 0;
	}
#ifdef USE_SDL_MIX
	else
	{
	    Mix_CloseAudio();
	}
#endif
    }
}

void sound_stop_all()
{
  int i;
  for (i = 0; i < NUM_SOUNDS; i++)
    {
      sound_stop(i);
    }
}

void sound_stop(int i)
{
    if ((i < NUM_SOUNDS) && (i >= 0))
    {
	sounds[i].loop = 0;
	sounds[i].dpos = sounds[i].dlen;
    }
}

static SampleNode *alloc_sample_node(const char *name)
{
  SampleNode *rval;
  if (sample_node == 0)
    {
      sample_node = malloc(sizeof(SampleNode));
      sample_node->next = NULL;
      rval = sample_node;
	
    }
  else
    {
      rval = sample_node;

      // find the last slot
    
      while (rval->next != NULL)
	{
	  rval = rval->next;
	}

      rval->next = malloc(sizeof(SampleNode));
      rval = rval->next;
    }
      
  memset(rval,0,sizeof(SampleNode));
  strcpy(rval->name,name);
  
    return rval;
    
}


SampleNode *lookup_sample(const char *sound_name)
{
    SampleNode *sn = 0;

    sn = sample_node;
    
    while (sn != 0)
    {
	
	if (strcmp(sound_name,sn->name) == 0)
	{
	    break;
	}
	sn = sn->next;
    }
    

    return sn;
}


int sound_load(const char *path, const char *file, const char *extension, int loop)
{
  if (lookup_sample(file)==0)
    {  
      SDL_AudioSpec wave;
      Uint8 *data;
      Uint32 dlen;
      char filepath[1000];
#ifdef USE_SDL_MIX
      Mix_Chunk *mc = 0;
#endif
  
      sprintf(filepath,"%s/%s.%s",path,file,extension);
#ifdef USE_SDL_MIX
      
      if (is_sdl_mix())
	{
	  mc = Mix_LoadWAV(filepath);
	  if (mc == 0)
	    {
	      fprintf(stderr, "SDL_Mix: Couldn't load %s: %s\n", filepath, Mix_GetError());
	    }
	}
      else
#endif
	{
	  /* Load the sound file and convert it to 16-bit stereo at 22kHz */
	  if ( SDL_LoadWAV(filepath, &wave, &data, &dlen) == NULL )
	    {
	      fprintf(stderr, "SDL: Couldn't load %s: %s\n", filepath, SDL_GetError());
	      return -1;
	    }
	}
    
      SampleNode *sn = alloc_sample_node(file);

      sn->loop = loop;
      if (!is_sdl_mix())
	{
	  SDL_BuildAudioCVT(&(sn->cvt), wave.format, wave.channels, wave.freq,
			    fmt.format,   fmt.channels,        fmt.freq);
	  sn->cvt.buf = malloc(dlen*sn->cvt.len_mult);
	  memcpy(sn->cvt.buf, data, dlen);
	  sn->cvt.len = dlen;
	  SDL_ConvertAudio(&(sn->cvt));
	  SDL_FreeWAV(data);
	}
#ifdef USE_SDL_MIX
      else
	{
	  sn->chunk = mc;
	}
#endif
    }
  
    return 0;
}
void unload_sounds()
{
    SampleNode *sn = 0;
    SampleNode *sn_next;

    sn = sample_node;
    
    while (sn != 0)
    {
	sn_next = sn->next;
	
      free(sn->cvt.buf);
      free(sn);
      sn = sn_next;
    }
  sample_node = 0;
    
}

int sound_play(const char *sound_name)
{
    int index;

    /* Look for an empty (or finished) sound slot */
    for ( index=0; index<NUM_SOUNDS-RESERVED_SOUNDS; ++index ) {
        if ( sounds[index].dpos == sounds[index].dlen ) {
            break;
        }
    }
    if ( index == NUM_SOUNDS - RESERVED_SOUNDS )
        return -1;


    /* Put the sound data in the slot (it starts playing immediately) */    

    SampleNode *sn = lookup_sample(sound_name);

    if (sn != 0)
    {
	if (sn->chunk == 0)
	{
	    SDL_LockAudio();
	    sounds[index].data = sn->cvt.buf;
	    sounds[index].dlen = sn->cvt.len_cvt;
	    sounds[index].dpos = 0;
	    sounds[index].loop = sn->loop;
	    SDL_UnlockAudio();
	}
#ifdef USE_SDL_MIX
	else
	{
	    Mix_PlayChannel(-1,sn->chunk,sn->loop);
	}
#endif
    }

    return index;
}

