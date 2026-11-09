#include "xenon1.h"
#include "xenon1_custom.h"
#include "oric_video.h"
#include "oric_machine.h"
#include "oric_basic.h"
#include "oric_sound.h"
#include "oric_input.h"

#include "xenon1_data.h"
#ifdef HAS_SDL_IMAGE
#include "SDL/SDL_image.h"
#endif

#include <string.h>
#include <unistd.h>
#include <getopt.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _NDS
#include <nds.h>
#endif

#include "oric_input.h"

#define JUMP_TO(a,pref) case 0x##a: pref##_##a(); break

#undef main

enum GameContext { INTRO, MENU, GAME, GAME_OVER };

SDL_Surface *planets_background_image = 0;
SDL_Surface *m35_scrolling_image = 0;
enum GameContext gctx = INTRO;
int m35_pos = 0;
int scroll_timer = 0;
int scroll_period = 50;
SDL_Rect m35_src_1,m35_src_2,m35_dst_2;
SDL_Rect planets_src;

int last_gctx = INTRO;

void randomize_background()
{
    last_gctx = INTRO;
}
#ifdef HAS_SDL_IMAGE
static void draw_background(SDL_Surface *screen,int elapsed_time)
{
    
    switch(gctx)
    {
      case INTRO:
      case MENU:
      case GAME_OVER:
	scroll_timer += elapsed_time;

	while (scroll_timer > scroll_period)
	{
	    m35_pos = (m35_pos+1) % m35_scrolling_image->h;
	    scroll_timer -= scroll_period;
	}

	m35_src_1.y = m35_pos;
	m35_src_1.h = m35_scrolling_image->h - m35_pos;
	m35_src_2.y = 0;
	//m35_src_2.h = m35_pos;
	
	m35_dst_2.y = m35_scrolling_image->h - m35_pos;
	//m35_dst_2.h = m35_pos;

	SDL_Rect dest_rect;
	dest_rect.x = m35_scrolling_image->w;
	dest_rect.y = 0;
	dest_rect.w = screen->w - m35_scrolling_image->w;
	dest_rect.h = screen->h;
	
	SDL_BlitSurface(m35_scrolling_image, &m35_src_1, screen, NULL);
	SDL_BlitSurface(m35_scrolling_image, &m35_src_2, screen, &m35_dst_2);
	SDL_FillRect(screen,&dest_rect,0);
	break;
      case GAME:
      {
	  /*int gctx_change = gctx != last_gctx;
	if (gctx_change)
	{
	    planets_src.x = ((planets_src.w - screen->w) * rand()) / 32768;
	    planets_src.y = ((planets_src.h - screen->h) * rand()) / 32768;
	    }*/
	SDL_BlitSurface(planets_background_image, NULL, screen, NULL);
      }
	break;
    }

    last_gctx = gctx;
}
#endif

void quit_callback()
{
    save_hiscore();

  #ifdef HAS_SDL_IMAGE
    if (planets_background_image)
    {
	SDL_FreeSurface(planets_background_image);
	planets_background_image = 0;
    }
    if (m35_scrolling_image)
    {
	SDL_FreeSurface(m35_scrolling_image);
	m35_scrolling_image = 0;
    }
  #endif
    //cpu_dump_memory("memory.bin");

    
    sound_close();
}

void load_hiscore()
{
    FILE *f = fopen("hiscores","rb");
    if (f != 0)
    {
	fread(cpu_get_mem()+hiscore_buffer_6700,hiscore_length,1,f);
	fclose(f);
    }
    
}
void save_hiscore()
{
    FILE *f = fopen("hiscores","wb");
    if (f != 0)
    {
	fwrite(cpu_get_mem()+hiscore_buffer_6700,hiscore_length,1,f);
	fclose(f);
    }
    
}
#ifdef HAS_SDL_IMAGE

static int img_initialized = 0;

static SDL_Surface *load_image(const char *image_name)
{
 
    char buf[200];
  sprintf(buf,"gfx/%s",image_name);
  if (!img_initialized)
    {
      IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
      img_initialized=1;
    }
  
    SDL_Surface *s = IMG_Load(buf);

    if (s == 0)
    {
	fprintf(stderr,"Cannot load image \"%s\": %s\n",buf,SDL_GetError());
	quit();
    }
  return s;
}
#endif
    
int main(int argc, char **argv)
{
    VideoParams vp;
    int c;
    int enhanced = 0;
    int joystick = 0;
    memset(&vp,0,sizeof(vp));

  vp.extended_colors = 1;
  
    vp.mask_color = -1;
    vp.window_title = "Xenon 1 v1.2 - remake by JOTD";
    vp.update_rate = 30;
  vp.draw_only_changes = 1;
  vp.extended_colors = 1;
  vp.double_size = 1;
  set_pen(8,0xFFA500); // orange
  set_pen(12,0x120A8F); // navy blue
    #ifdef _NDS
        consoleDemoInit();
	iprintf("Xenon 1 DS\n");
	
#else
    while ((c = getopt (argc, argv, "sfej")) != -1)
    {
         switch (c)
           {
	     case 's':
	       vp.double_size = 0;
	       break;
	     case 'e':
	       enhanced = 1;
	       vp.double_size = 1; // enhanced means double size
	       break;
	     case 'j':
	       joystick = 1;
	       break;
	     case 'f':
	       vp.full_screen = 1;
	       break;
	     case '?':
	       fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	       exit(1);
	   }
    }
#endif

    vp.quit_callback = &quit_callback;
    #ifdef HAS_SDL_IMAGE

    if (enhanced)
    {
	
	vp.mask_color = 0;
	
	planets_background_image = load_image("planets.jpg");
	m35_scrolling_image = load_image("star_scroll.jpg");
	
	vp.background_callback = &draw_background;
	vp.palette = planets_background_image->format->palette;

	
	m35_src_1.x = 0;
	m35_src_1.w = m35_scrolling_image->w;

	m35_src_2 = m35_src_1;
	m35_dst_2 = m35_src_1;

	planets_src.x = 0;
	planets_src.y = 0;
	planets_src.w = planets_background_image->w;
	planets_src.h = planets_background_image->h;
	
    }
  #else
  (void)enhanced;
    #endif
  
    cpu_init();
    
    memset(cpu_get_mem(),0x0,0xC000);
    memcpy(cpu_get_mem(),block,sizeof(block));


    video_init(&vp);

    sound_init(ORIC_SIMPLE_SDL);

    joystick_init(joystick);
    basic_init();
    paper(0);ink(0);
        

    //cpu_add_memwatch(0,2,MEMWATCH_WRITE);
    doke(0,0x700);
    poke(hires_text_flag_05fe,2);
    
    while(1)
    {
	unsigned char hires_text = peek(hires_text_flag_05fe);

	switch(hires_text)
	{
	  case 2:
	    hires();
	    break;
	  case 1:
	    text();
	    break;
	  default:
	     printf("unknown resolution mode %d\n",hires_text);
	    exit(1);
	}

	int next_jump = deek(0);

	flush_events();
	
	switch(next_jump)
	{
	  case 0x700:
	    gctx = INTRO;
	    // intro
	    E_0700();
	    break;
	  case 0x735:
	    gctx = MENU;
	    // menu
	    E_0735();
	    break;
	  case 0x0751:
	    gctx = GAME;
	    // game
	    E_0751();
	    break;
	  case 0x4783:
	    gctx = GAME_OVER;
	    E_game_over_4783();
	    break;
	  default:
	    printf("unknown address %x\n",next_jump);
	    exit(1);
	}
    }
    
    return 0;
}

