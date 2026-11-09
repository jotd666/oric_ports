#include "oric_basic.h"
#include "oric_video.h"
#include "oric_sound.h"
#include "oric_input.h"
#include "oric_machine.h"
#include "oric_timer.h"
#include "zorgon_data.h"
#include "zorgon_code.h"
#include "zorgon_custom.h"
#ifdef HAS_SDL_IMAGE
#include "SDL/SDL_image.h"
#endif

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

#define hiscore_buffer_9C00 0x9C00
#define hiscore_length 328

#ifdef HAS_SDL_IMAGE
static const char *background_image_names[] =
{ "bridge.jpg","cavern.jpg","castle.jpg",
 "sky.jpg","spiderweb.jpg", "hearts.jpg"};
#endif

int nb_background_images = 0;

int max_nb_lives = 3;

SDL_Surface *m35_scrolling_image = 0;
int m35_pos = 0;
int scroll_timer = 0;
int scroll_period = 50;
SDL_Rect m35_src_1,m35_src_2,m35_dst_2;
SDL_Surface **background_image = 0;

enum GameContext gctx = INTRO;

void E_0713();
void J_076C();

int music_on = 0;

#undef main

void P_monophonic_music_100()
{
  int i,b,d,ns,v,nn,n;
  float o;
    
  b=deek(4);ns=peek(6);v=peek(0x4fd);
  for (i=1;i<=ns;i++)
    {
      nn=peek(b)-5;
      d=peek(b+1);
      o=nn/48.0;
      n=(int)((o-(int)o)*12+1.5);
      music(1,o+2,n,v);

      oric_wait(d*5);
      b+=2;
    }
  play(0,0,0,0);
}

void P_polyphonic_music_200()
{
  int v,x,n,nn;
  float o;
    
  v=peek(0x4fd);
  P_5200();
  P_523C();
  play(7,0,0,0);

  OricTimer tmr = timer_init(1,120);
    
  do
    {
      x=peek(2);
      nn=peek(3)-5;
      o=nn/48.0;
      n=(int)((o-(int)o)*12+1.5);
      music(x,o+2,n,v);

      P_527C();

      timer_tick(&tmr);
    }
    while (peek(3)!=255);

  play(0,0,0,0);
}
#ifdef HAS_SDL_IMAGE

static SDL_Surface *load_image(const char *image_name)
{
  char buf[200];
  sprintf(buf,"gfx/%s",image_name);
  SDL_Surface *s = IMG_Load(buf);

  if (s == 0)
    {
      fprintf(stderr,"Cannot load image \"%s\"\n",buf);
      quit();
    }
  return s;
}
#endif

int get_max_nb_lives()
{
  return max_nb_lives;
}

void set_game_context(enum GameContext gc)
{
  gctx = gc;
}

void basic_main()
{
  basic_init();
  paper(0);ink(0);hires();
  doke(0,0x713);

 
  while(1)
    {
      switch(deek(0))
	{
	  case 0x713:
	    // intro
	    gctx = INTRO;
	    E_0713();
	    break;
	  case 0x652D:
	    E_intro_652D();
	    break;
	  case 0x075A:
	    gctx = MENU;
	    E_075A();
	    break;
	  case 0x7B6:
	    E_07B6();
	    break;
	  case 0x7C8:
	    // main game
	    E_07C8();
	    break;
	  case 0x8DF:
	    gctx = GAME_OVER;
	    // game over
	    E_08DF();
	    break;
	  case 0x8FB:
	    gctx = GAME_OVER;
	    // game over 2
	    E_08FB();
	    break;
	  case 0x76C:
	    // game completed: restart
	    if (!music_on)
	      {
		oric_wait(300);
	      }
	    J_076C();
	    break;
	  default:
	    {
	      char buf[100];
	      sprintf(buf,"unsupported address %x\n",deek(0));
	      fatal_error(buf);
	    }
	}
	
      int f=peek(3);
	
      if ( f==1 ) { text();}
      if ( f==2 ) { hires();}
      if ( f==3 ) { if (music_on) P_monophonic_music_100(); }
      if ( f==4 ) { if (music_on) P_polyphonic_music_200(); }
    }
}
void quit_callback()
{
 #ifdef HAS_SDL_IMAGE
  int i = 0;
  for (i = 0; i < nb_background_images; i++)
    {
      SDL_FreeSurface(background_image[i]);
    }

  free(background_image);
    
 
  if (m35_scrolling_image)
    {
      SDL_FreeSurface(m35_scrolling_image);
      m35_scrolling_image = 0;
    }
 #endif
  
  //cpu_dump_memory("memory.bin");
  save_hiscore();

  sound_close();
}
void draw_background(SDL_Surface *screen,int elapsed_time)
{
 #ifdef HAS_SDL_IMAGE
   
  switch(gctx)
    {
      case INTRO:
	set_mask_y_limit(-1);
	// sky
	SDL_BlitSurface(background_image[3], NULL, screen, NULL);
	break;
      case COMPLETED:
	set_mask_y_limit(200);
	SDL_BlitSurface(background_image[5], NULL, screen, NULL);
	break;
      case GAME_BRIDGE:
	set_mask_y_limit(200);
	SDL_BlitSurface(background_image[0], NULL, screen, NULL);
	break;
      case GAME_SHIP:
	set_mask_y_limit(200);
	scroll_timer += elapsed_time;

	while (scroll_timer > scroll_period)
	  {
	    m35_pos = (m35_pos+1) % m35_scrolling_image->w;
	    scroll_timer -= scroll_period;
	  }

	m35_src_1.x = m35_pos;
	m35_src_1.w = m35_scrolling_image->w - m35_pos;
	m35_src_2.x = 0;
	m35_src_2.w = m35_pos;
	
	m35_dst_2.x = m35_scrolling_image->w - m35_pos;
	//m35_dst_2.h = m35_pos;

	SDL_Rect dest_rect;
	dest_rect.y = m35_scrolling_image->h;
	dest_rect.x = 0;
	dest_rect.w = screen->w;
	dest_rect.h = screen->h - m35_scrolling_image->h;
	
	SDL_BlitSurface(m35_scrolling_image, &m35_src_1, screen, NULL);
	SDL_BlitSurface(m35_scrolling_image, &m35_src_2, screen, &m35_dst_2);
	SDL_FillRect(screen,&dest_rect,0);
	break;
      case GAME_SPIDER:
	{
	  set_mask_y_limit(200);
	  SDL_BlitSurface(background_image[4], NULL, screen, NULL);
	}
	break;
      case GAME_REPTILE:
	{
	  set_mask_y_limit(200);
	  SDL_BlitSurface(background_image[1], NULL, screen, NULL);
	}
	break;
      case GAME_BIRD:
	{
	  set_mask_y_limit(200);
	
	  SDL_BlitSurface(background_image[3], NULL, screen, NULL);
	}
	break;

      case GAME_OVER:
	// do nothing
	break;
      case MENU:
	set_mask_y_limit(-1);
	SDL_BlitSurface(m35_scrolling_image, NULL, screen, NULL);
	break;
      case MISSION_SELECT:
	set_mask_y_limit(-1);
	SDL_BlitSurface(m35_scrolling_image, NULL, screen, NULL);
	break;
      case GAME_ZORGON:
	SDL_BlitSurface(background_image[2], NULL, screen, NULL);

	break;
      default:
	set_mask_y_limit(-1);
	SDL_FillRect(screen,NULL,0);
	break;
    }
#else
  (void)screen;
  (void)  elapsed_time;
  #endif
  
 
}


int main(int argc, char **argv)
{
  VideoParams vp;
  int c;
  int enhanced = 0;
  int joystick = 0;
    
  memset(&vp,0,sizeof(vp));

  vp.mask_color = -1;
  vp.window_title = "Zorgon's Revenge v2.0 - remake by JOTD";
  vp.update_rate = 30;
  vp.draw_only_changes = 1;
  
  while ((c = getopt (argc, argv, "dfejml:")) != -1)
    {
      switch (c)
	{
	  case 'd':
	    vp.double_size = 1;
	    break;
	  case 'e':
	    enhanced = 1;
	    vp.double_size = 1;
	    break;
	  case 'j':
	    joystick = 1;
	    break;
	  case 'f':
	    vp.full_screen = 1;
	    break;
	  case 'l':
	    // define number of lives
	    max_nb_lives = atoi(optarg);
	    if ((max_nb_lives <= 0) || (max_nb_lives > 9))
	      {
		max_nb_lives = 3;
	      }
	    break;
	  case 'm':
	    music_on = 1;
	    break;
	  case '?':
	    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	    exit(1);
	}
    }

  vp.quit_callback = &quit_callback;

  cpu_init();
    
  memset(cpu_get_mem(),0x0,0xC000);
  memcpy(cpu_get_mem(),block,sizeof(block));
#ifdef HAS_SDL_IMAGE

  if (enhanced)
    {
      vp.mask_color = 0;
      m35_scrolling_image = load_image("star_scroll.jpg");
	
      m35_src_1.y = 0;
      m35_src_1.h = m35_scrolling_image->h;

      m35_src_2 = m35_src_1;
      m35_dst_2 = m35_src_1;
	
      nb_background_images = sizeof(background_image_names)/sizeof(char*);
	
      background_image = (SDL_Surface**)malloc(sizeof(SDL_Surface*) * nb_background_images);

      int i;

      for (i = 0; i < nb_background_images; i++)
	{
	  background_image[i] = load_image(background_image_names[i]);
	}
	
      vp.background_callback = &draw_background;
      vp.palette = background_image[1]->format->palette;
    }
  #else
  (void)enhanced;
    #endif
  

  video_init(&vp);

  joystick_init(joystick);
    
  sound_init(1);
  basic_main();


  return 0;
}
void load_hiscore()
{
  FILE *f = fopen("hiscores","rb");
  if (f != 0)
    {
      fread(cpu_get_mem()+hiscore_buffer_9C00,hiscore_length,1,f);
      fclose(f);
    }
    
}
void save_hiscore()
{
  const unsigned char *score_mem = cpu_get_mem()+hiscore_buffer_9C00;
  if (score_mem[0]!='U') // don't save hiscore if not initialized
    {
  
      FILE *f = fopen("hiscores","wb");
      if (f != 0)
	{
	  fwrite(score_mem,hiscore_length,1,f);
	  fclose(f);
	}
    }
  
}
