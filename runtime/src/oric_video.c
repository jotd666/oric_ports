/***************************************************************************

	vidhrdw/oric.c
	
	All graphic effects are supported including mid-line changes.
	There may be some small bugs.

	
***************************************************************************/

  

#include "oric_machine.h"
#include "oric_video.h"
#include "SDL/SDL.h"
#include "charset.h"
#include <stdio.h>
#include "my_assert.h"


#ifdef _WIN32
#include <windows.h>
#endif

#include <string.h>


const VideoParams *vp;

SDL_TimerID refresh_timer_id = 0;


typedef unsigned char UINT8;






static int oric_rgb_pen[] = 
{
 0x000000,  /* 0: black */
 0xFF0000,  /* 1: red */
 0x00FF00,  /* 2: green */
 0xFFFF00,  /* 3: yellow */
 0x0000FF,  /* 4: blue */
 0xFF00FF,  /* 5: magenta */
 0x00FFFF,  /* 6: cyan */
 0xFFFFFF,   /* 7: white */
 // extended colors
0xd2691e,  /* chocolate */
 0x808080,  /* gray */
 0x009999,  /* teal */
 0xAA0114,  /* red2 */
 0x999900,  /* olive */
 0x800080,  /* purple */
 0xFFA500,  /* orange */
 0x4B5320   /* army green */
};

#define NB_COLORS  16
// same thing, but encoded using SDL_MapRGB


void set_pen(unsigned int index,int rgb)
{

  if (index<NB_COLORS)
    {
      oric_rgb_pen[index] = rgb;
    }
  
}

const int *get_pens()
{
  return oric_rgb_pen;  
}


static void shared_init()
{

  open_screen(vp);
  
  
  reset_charset();

 
  

 
  set_colors(oric_rgb_pen,8);

    
    /* initialise refresh timer */

    refresh_timer_id = SDL_AddTimer(vp->update_rate,refresh_timer_callback,0);
  if (refresh_timer_id==0)
    {
      fprintf(stderr,"Cannot create timer: %s\n",SDL_GetError());
      quit();
    }
  
  
}





#ifdef _NDS

#include <nds.h>

void video_init(const VideoParams *pvp)
{
  vp = pvp;
  
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    char buf[200];
    sprintf(buf, "Unable to initialize SDL: %s", SDL_GetError());
    fatal_error(buf);
  }
    
  int vm = SDL_HWSURFACE|SDL_FULLSCREEN;
    
  // image blit won't work if screenmode is not standard
  screen = SDL_SetVideoMode(320,200,8,vm);
 
     
  consoleDemoInit();
    iprintf("Xenon 1 DS: screen OK\n");
    if (screen == 0)
    {
	char buf[200];
	sprintf(buf, "Unable to open SDL screen: %s\n", SDL_GetError());
	fatal_error(buf);
    }

      shared_init();


}
#else
#ifdef __amigaos__
void video_init(const VideoParams *pvp)
{
  vp = pvp;

  shared_init();
 

}
#else

// windows version, everything is pretty much done in shared_init
void video_init(const VideoParams *pvp)
{
  vp = pvp;
 
    
  shared_init();
  SDL_WM_SetCaption(vp->window_title,vp->window_title);
  

}
#endif
#endif


void quit()
{
  close_screen();
    if (refresh_timer_id != 0)
    {
	SDL_RemoveTimer(refresh_timer_id);
	refresh_timer_id = 0;
    }
   

    // wait till we're sure that the timers are removed (SDL bug?)
    SDL_Delay(100);
    
    if (vp->quit_callback)
    {
	vp->quit_callback();
    }
    SDL_Quit();
#ifndef _NDS
    exit(0);
#endif
}



/***************************************************************************
  oric_vh_screenrefresh
  ***************************************************************************/


const unsigned char *video_get_charset()
{
    return charset;
}

void reset_charset()
{
  UINT8 *oric_ram = cpu_get_mem();
    
  memcpy(oric_ram+0xB500,charset,sizeof(charset));

}

