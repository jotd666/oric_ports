
#include "oric_video_draw.h"
#include "my_assert.h"
#include "oric_machine.h"

#ifdef _WIN32
#include <windows.h>
#endif

#include <string.h>

extern const int *get_pens();

static SDL_Rect dest_rect;
static SDL_Surface *screen =0;
static const VideoParams *vp =0;

static int mask_y_limit = SCREEN_HEIGHT;

static int oric_sdl_pen[16];
static SDL_Color rgb_sdl[16];
// allocate the memory for 2 buffers
#define SCREEN_BUFFER_SIZE (0xBFE0-0xA000)

static UINT8 screen_buffer_1[SCREEN_BUFFER_SIZE];
static UINT8 screen_buffer_2[SCREEN_BUFFER_SIZE];
static UINT8 *previous_screen_buffer,*current_screen_buffer;
static UINT8 *oric_ram; 

static const Uint32 flash_update_rate = 500;

/* current state of the display */
/* some attributes persist until they are turned off.
This structure holds this persistant information */
struct oric_vh_state
{
	/* foreground and background colour used for rendering */
	/* if flash attribute is set, these two will both be equal
	to background colour */
	int active_foreground_colour;
	int active_background_colour;
	/* current foreground and background colour */
	int foreground_colour;
	int background_colour;
	int mode;
	/* text attributes */
	int text_attributes;

	unsigned long read_addr;

	/* current addr to fetch data */
	unsigned char *char_data;
	/* base of char data */
	unsigned char *char_base;
	
	/* if (1<<3), display graphics, if 0, hide graphics */
	int flash_state;
	/* current count */
	Uint32 flash_counter;
};


struct oric_vh_state vh_state;
	
void fatal_error(const char *message)
{
#ifdef WIN32
  MessageBox(NULL, message, "Fatal Error", MB_OK);
  fprintf(stderr,"%s\n",message);
#else
#ifdef _NDS
    iprintf("%s\n",message);
#else
    fprintf(stderr,"%s\n",message);
#endif
    SDL_Quit();
#endif
#ifndef _NDS
    exit(1);
#endif
}

static inline int is_hires_active()
{
  return vh_state.mode & (1<<2);
}


static inline int is_flash_active()
{
  return vh_state.text_attributes & (1<<2);
}

/* the alternate charset follows from the standard charset.
   Each charset holds 128 chars with 8 bytes for each char.

   The start address for the standard charset is dependant on the video mode */
static inline void oric_refresh_charset(void)
{
    /* alternate char set? */
    if ((vh_state.text_attributes & (1<<0))==0)
    {
	/* no */
	vh_state.char_data = vh_state.char_base;
    }
    else
    {
	/* yes */
	vh_state.char_data = vh_state.char_base + (128*8);
    }
}

static inline void oric_vh_update_flash(void)
{
    /* flash active? */
    if (is_flash_active())
    {
	/* yes */

	/* show or hide text? */
	if (vh_state.flash_state)
	{
	    /* hide */
	    /* set foreground and background to be the same */
	    vh_state.active_foreground_colour = vh_state.background_colour;
	    vh_state.active_background_colour = vh_state.background_colour;
	    return;
	}
    }
	

    /* show */
    vh_state.active_foreground_colour = vh_state.foreground_colour;
    vh_state.active_background_colour = vh_state.background_colour;
}

/* update video hardware state depending on the new attribute */
static inline void oric_vh_update_attribute(int attribute)
{
  int extended_colors =  (vp->extended_colors && (attribute & 0x80)) ? 8 : 0;
  /* attribute */
  //int attribute = c & 0x03f;

    switch (attribute & (0x03<<3))
    {
      case 0:
      {
	  /* set foreground colour */
	  vh_state.foreground_colour = (attribute & 0x07) + extended_colors;
	  oric_vh_update_flash();
      }
      break;

      case 1<<3:
      {
	  vh_state.text_attributes = (attribute & 0x07);

	  oric_refresh_charset();

	  /* text attributes */
	  oric_vh_update_flash();
      }
      break;

      case 2<<3:
      {
	  /* set background colour */
	  vh_state.background_colour = (attribute & 0x07) + extended_colors;
	  oric_vh_update_flash();
      }
      break;

      case 3<<3:
      {
	  /* set video mode */
	  vh_state.mode = attribute & 0x07;

	  /* a different charset base is used depending on the video mode */
	  /* hires takes all the data from 0x0a000 through to about 0x0bf68,
	     so the charset is moved to 0x09800 */
	  /* text mode starts at 0x0bb80 and so the charset is in a different location */
	  if (is_hires_active())
	    {
	      // HIRES
	      /* set screen memory base and standard charset location for this mode */
	      vh_state.read_addr = 0x0a000;
				
	      vh_state.char_base = oric_ram + (unsigned long)0x09800; 
	  }
	  else
	    {
	      // TEXT
	      /* set screen memory base and standard charset location for this mode */
	      vh_state.read_addr = 0x0bb80;
				
	      vh_state.char_base = oric_ram + (unsigned long)0x0b400;
			
	  }
	  /* changing the mode also changes the position of the standard charset
	     and alternative charset */
	  oric_refresh_charset();
     }
      break;

      default:
	break;
    }
}

void open_screen(const VideoParams *pvp)
{
    if (pvp->draw_only_changes)
    {
      // not very clean, but set the values to some value
      // very unlikely to be written at startup AT ALL since
      // video memory is zeroed anyway
      memset(screen_buffer_1,0xff,SCREEN_BUFFER_SIZE);
      memset(screen_buffer_2,0xff,SCREEN_BUFFER_SIZE);

      previous_screen_buffer = screen_buffer_1;
      current_screen_buffer = screen_buffer_2;
    }

  oric_ram = cpu_get_mem();
  
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    char buf[200];
    sprintf(buf, "Unable to initialize SDL: %s", SDL_GetError());
    fatal_error(buf);
  }
  
  vp = pvp;
  int sz = vp->double_size ? 2 : 1;           
  
  int vm = SDL_HWSURFACE;
  if (vp->double_buffering)
    {
      vm |= SDL_DOUBLEBUF;
    }
  
  if (vp->full_screen)
    {
      vm |= SDL_FULLSCREEN;
      // image blit won't work if screenmode is not standard
      screen = SDL_SetVideoMode(640,480,vp->screen_depth,vm);
    }
  else
    {
      screen = SDL_SetVideoMode(SCREEN_WIDTH*sz,SCREEN_HEIGHT*sz,vp->screen_depth,vm);
    }
     
  if (screen == 0)
    {
      char buf[200];
      sprintf(buf, "Unable to open SDL screen: %s\n", SDL_GetError());
      fatal_error(buf);
    }
    /* initialise flash timer */
    vh_state.flash_counter = 0;
    vh_state.flash_state = 0;
    
    /* mode */
    oric_vh_update_attribute((1<<3)|(1<<4));
}

static inline void draw_pixel(int px, int py, Uint32 color)
{
  int x = px;
  int y = py;
  int screen_pitch = y*screen->pitch;
  
  if (vp->double_size)
  {
      x = px*2;
      y = py*2;
      screen_pitch*=2;
  }
  
  switch (screen->format->BytesPerPixel)
  {
    case 1: // Assuming 8-bpp
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + screen_pitch + x;
        *bufp = color;
	if (vp->double_size)
	{
	    *(bufp+1) = color;
	}
      }
      break;
    case 2: // Probably 15-bpp or 16-bpp
      {
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + screen_pitch/2 + x;
        *bufp = color;
	if (vp->double_size)
	{
	    *(bufp+1) = color;
	}
      }
      break;
    case 3: // Slow 24-bpp mode, usually not used
      {
        Uint8 *bufp;
        bufp = (Uint8 *)screen->pixels + screen_pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
          bufp[0] = color;
          bufp[1] = color >> 8;
          bufp[2] = color >> 16;
        } else {
          bufp[2] = color;
          bufp[1] = color >> 8;
          bufp[0] = color >> 16;
        }
	if (vp->double_size)
	{
	    *(bufp+3) = bufp[0];
	    *(bufp+4) = bufp[1];
	    *(bufp+5) = bufp[2];
	}
      }
      break;
      case 4: // Probably 32-bpp
      {
        Uint32 *bufp;
        bufp = (Uint32 *)screen->pixels + screen_pitch/4 + x;
        *bufp = color;

	if (vp->double_size)
	{
	    *(bufp+1) = color;
	    
	    bufp += screen->pitch/4;
	    *(bufp++) = color;
	    *bufp = color;
	}
      }
      
      break;
      }
}


static inline void do_draw_6_pixels(int px, int py, int sdl_pen_array[])
{
  int x = px;
  int y = py;
  int screen_pitch = y*screen->pitch;
  int i;
  
  if (vp->double_size)
  {
      x = px*2;
      y = py*2;
      screen_pitch*=2;
  }
  
  switch (screen->format->BytesPerPixel)
  {
    case 1: // Assuming 8-bpp
      {
	  if (vp->double_size)
	  {
	      Uint8 *bufp_s = (Uint8 *)screen->pixels + screen_pitch + x;
	      for (i=0;i<6;i++)
	      {
		  Uint8 *bufp = bufp_s;
		  int color = oric_sdl_pen[sdl_pen_array[i]];
		  *(bufp) = color;
		  
		  *(bufp+1) = color;
		  bufp += screen->pitch;
		  *(bufp++) = color;
		  *bufp = color;

		  bufp_s+=2;
	      }
	  }
	  else
	    {	      
	      Uint8 *bufp = (Uint8 *)screen->pixels + screen_pitch + x;
	      for (i=0;i<6;i++)
	      {
		  *(bufp++) = oric_sdl_pen[sdl_pen_array[i]];
		}
	      
	  }
      }
      break;
    case 2: // Probably 15-bpp or 16-bpp also broken as is
      {
        Uint16 *bufp;
        bufp = (Uint16 *)screen->pixels + screen_pitch/2 + x;
        *bufp = 0;
	if (vp->double_size)
	{
	    *(bufp+1) = 0;
	}
      }
      break;
    case 3: // Slow 24-bpp mode, usually not used (broken as is)
      {
	  Uint8 *bufp;
	int  color =0;
        bufp = (Uint8 *)screen->pixels + screen_pitch + x * 3;
        if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
        {
          bufp[0] = color;
          bufp[1] = color >> 8;
          bufp[2] = color >> 16;
        } else {
          bufp[2] = color;
          bufp[1] = color >> 8;
          bufp[0] = color >> 16;
        }
	if (vp->double_size)
	{
	    *(bufp+3) = bufp[0];
	    *(bufp+4) = bufp[1];
	    *(bufp+5) = bufp[2];
	}
      }
      break;
      case 4: // Probably 32-bpp
      {
	  // optimized to avoid too many computations
	  
	  int sp4 = screen_pitch/4;
	  
	  if (vp->double_size)
	    {
	      int spp4 = screen->pitch/4;
	      Uint32 *bufp_s = (Uint32 *)screen->pixels + sp4 + x;
	      for (i = 0; i < 6; i++)
	      {
		  Uint32 *bufp = bufp_s;
		  int color = oric_sdl_pen[sdl_pen_array[i]];
		  if (color == -1)
		    {
		      bufp+=1+spp4;
		    }
		  else
		    {
		      *bufp = color;
		      *(bufp+1) = color;
		
		      bufp += spp4;
		      *(bufp++) = color;
		      *bufp = color;
		    }
		  bufp_s += 2;			
		}
	  }
	  else
	  {
	      Uint32 *bufp = (Uint32 *)screen->pixels + sp4 + x;
	      int color = 0;
	      for (i = 0; i < 6; i++)
		{
		  if (color == -1)
		    {
		      bufp++;
		    }
		  else
		    {
		      color =oric_sdl_pen[sdl_pen_array[i]];
		      *(bufp++) = color;
		    }
		}
	  }
      }
      
      break;
  }
}

/* render 6-pixels using foreground and background colours specified */
/* used in hires and text mode */
static inline void oric_vh_render_6pixels(int x,int y, int fg, int bg,int data, int invert_flag)
{
    int mask_active = is_mask_active(y);
    int i;
    int sdl_pens[2];
    int rgb_pens[2];
    int px;
    int sdl_pen_array[6];
    const int *pens = get_pens();
    /* invert? */
    if (invert_flag)
    {
	fg ^=0x07;
	bg ^=0x07;
    }
    

    if (data != 0)
    {
	sdl_pens[1] = fg;
	sdl_pens[0] = bg;
	rgb_pens[1] = pens[fg];  // for mask
	rgb_pens[0] = pens[bg];
	    
	px = x+6;
	int ii = 6;
	// process 6 bits
	for (i=0; i<6; i++)
	{
	    int d = data & 0x1;
	    
	    px--;
	    ii--;
	    if (!mask_active || (rgb_pens[d] != vp->mask_color))
	    {
		sdl_pen_array[ii] = sdl_pens[d];
	    }
	    else
	    {
	      // mask active and RGB = mask color
		sdl_pen_array[ii] = -1;  // don't draw anything, leave background as-is
	    }
	    data >>= 1;
	}
      draw_6_pixels(x,y,sdl_pen_array);
	 
	     
    }
    else
    {
	/* often occurs that there is an empty data to plot:
	   this is much faster */
	plot_box(x,y,6,1,bg);
    }

}
static inline void do_draw_6_pixels_old(int x, int y, int sdl_pen_array[])
{
    int i;
    for (i = 0; i < 6; i++)
    {
	int p = oric_sdl_pen[sdl_pen_array[i]];
	if (p != -1)
	{
	    draw_pixel(x+i,y,p);
	}
    }
}

void set_colors(const int rgb[],int nb_colors)
{
  int i;
  for (i=0; i<nb_colors; i++)
    {
      Uint32 rgb_pen = rgb[i];
      Uint8 R = rgb_pen >> 16;
      Uint8 G = (rgb_pen >> 8) & 0xff;
      Uint8 B = rgb_pen & 0xff;
	
      SDL_Color *c = rgb_sdl+i;
      c->r = R;
      c->g = G;
      c->b = B;
      oric_sdl_pen[i] = SDL_MapRGB(screen->format, R, G, B);
    }
  
  
      SDL_SetColors(screen, rgb_sdl, 0, nb_colors);
          
      
}

void draw_6_pixels(int x, int y, int sdl_pen_array[])
{
  switch (screen->format->BytesPerPixel)
    {
      case 4:
      case 1:
	do_draw_6_pixels(x,y,sdl_pen_array);
	break;
      default:
	// compatibility mode
	do_draw_6_pixels_old(x,y,sdl_pen_array);
	break;
    }
}

void screen_lock()
{
  if ( SDL_MUSTLOCK(screen) )
  {
    if ( SDL_LockSurface(screen) < 0 )
    {
      return;
    }
   }
    
  if (vp->background_callback != 0)
    {
      vp->background_callback(screen,vp->update_rate);
     }
  
}
void   close_screen()
{
  // nothing specific to do
}

void screen_unlock()
{
  
  if (vp->foreground_callback != 0)
    {
      vp->foreground_callback(screen,vp->update_rate);
    }
  
  if ( SDL_MUSTLOCK(screen) )
  {
    SDL_UnlockSurface(screen);
    }
  SDL_Flip(screen);
}

void plot_box(int x, int y, int w, int h, int pen)
{
    if ((pen != vp->mask_color) || !is_mask_active(y))

    {
	if (vp->double_size)
	{
	    dest_rect.x = x*2;
	    dest_rect.y = y*2;
	    dest_rect.w = w*2;
	    dest_rect.h = h*2;
	}
	else
	{
	    dest_rect.x = x;
	    dest_rect.y = y;
	    dest_rect.w = w;
	    dest_rect.h = h;

	}
	SDL_FillRect(screen, &dest_rect, oric_sdl_pen[pen]);
    }
}


void set_mask_y_limit(int y)
{
    mask_y_limit = y >=0 ? y : SCREEN_HEIGHT;
}

int is_mask_active(int y)
{
    return (y < mask_y_limit);
}


void force_refresh()
{
  memset(previous_screen_buffer,0xff,SCREEN_BUFFER_SIZE);
}
static inline int is_serial_attribute(int c)
{
  return ((c & ((1<<6) | (1<<5)))==0);
}

/*
// addr is RAM+read_addr (in the middle of a line)
// we need start_byte to know when to stop
static inline int no_attributes_till_eol(const UINT8 *previous_RAM, const UINT8 *RAM, int address, int start_byte)
{
  int i;
  for (i=start_byte;i<40;i++)
    {
      int addoff = address+i;
      int c = RAM[addoff];
      int previous_c=previous_RAM[addoff];
      if (c != previous_c) return 0;
      if (is_serial_attribute(c)) return 0;
      
    }
  return 1; // ok, no changes on the line & more attributes
  
   }
 */

static inline void render_text_mode(int c,int x,int y)
{
  // plot text
  int char_index;
  int char_data;
  int ch_line;

  char_index = (c & 0x07f);

  ch_line = y & 7;

  /* is double height set? */
  if (vh_state.text_attributes & (1<<1))
    { 
      /* if char line is even, top half of character is displayed,
	 if char line is odd, bottom half of character is displayed */
      int double_height_flag = ((y>>3) & 0x01);

      /* calculate line to fetch */
      ch_line = (ch_line>>1) + (double_height_flag<<2);
    }
					
  /* fetch pixel data for this char line */
  char_data = vh_state.char_data[(char_index<<3) | ch_line] & 0x03f;

  /* draw! */
  oric_vh_render_6pixels(x,y,
			 vh_state.active_foreground_colour, 
			 vh_state.active_background_colour, char_data, (c & 0x080));
}

#define HANDLE_FORCE_REDRAW	do {  c = RAM[read_addr]; \
if (!force_redraw)   { \
  UINT8 cm = c & 0x7F; \
  if (cm <16 && cm >= 12) \
    { \
      force_redraw = 2; /* now different: force for flashing */ \
    } \
} \
if (!force_redraw) \
  { \
    previous_c = previous_RAM[read_addr]; \
    if (c != previous_c) \
      { \
	force_redraw = 1; \
      } \
  } } while(0)


static void video_update()
{
  
  // first step is to copy the video memory to a buffer
  // it avoids "locking" the CPU to update the screen
  // (the "draw only changes" method cannot miss one furtive change of memory)
    
  // compares memory lines and compute start x
  // compute the x start where we should redraw. 0: draw all, FF: draw none

 
  UINT8 *RAM;
  if (vp->draw_only_changes)
    {
      RAM = current_screen_buffer-0xA000;      
      memcpy(current_screen_buffer,oric_ram+0xA000,SCREEN_BUFFER_SIZE);
    }
  else
    {
      RAM = oric_ram;
    }
  
  UINT8 *previous_RAM =  previous_screen_buffer-0xA000;
  
  int byte_offset,y;
  unsigned long read_addr_base;
  int hires_active = is_hires_active();
  int full_refresh = 0;
  
  screen_lock();

  
  /* set initial base */
  read_addr_base = vh_state.read_addr;
    
  /* is hires active? */
    
  
  for (y = 0; y < HIRES_LIMIT; y++)
    {
      int x = 0;      
       /* foreground colour white */
      oric_vh_update_attribute(7);
      /* background colour black */
      oric_vh_update_attribute((1<<3));
      oric_vh_update_attribute((1<<4));                 
  		
      unsigned long y_addr_offset = y*40;
      int force_redraw = (!vp->draw_only_changes)<<1;  // 2: force redraw no matter what
      
      for (byte_offset=0; byte_offset<40; byte_offset++)
	{
	  int c,previous_c;
	  unsigned long read_addr;

	  /* after line HIRES_LIMIT all rendering is done in text mode */
	      
	  /* calculate fetch address based on current line and
	     current mode */
	  if (hires_active)
	    {
	      read_addr = (unsigned long)read_addr_base + (unsigned long)byte_offset + y_addr_offset;
	    }
	  else
	    {
	      int char_line = (y>>3);
	      read_addr = (unsigned long)read_addr_base + (unsigned long)byte_offset + (unsigned long)(char_line*40);		
	    }

	  HANDLE_FORCE_REDRAW;
	  
          /* if bits 6 and 5 are zero, the byte contains a serial attribute */
	  if (is_serial_attribute(c))
	    {	      
	      oric_vh_update_attribute(c);	    
	      		  
	      if (force_redraw)
		{
		  /* display background colour when attribute has been found */
		  oric_vh_render_6pixels(x,y,vh_state.active_foreground_colour, vh_state.active_background_colour, 0,0); /* was (c & 0x080) */		    		  
		}
	      
	      /* refresh is hires active flag ? */
	      if (hires_active != is_hires_active())
		{
		  hires_active = !hires_active;
		  // force redraw of everything
		  full_refresh = 1;
		  
		}
					
	      read_addr_base = vh_state.read_addr;
		
	    }
	      else if (force_redraw)
		{
		  /* hires? */
		  if (hires_active)
		    {
		      int pixel_data = c & 0x03f;
		      /* plot hires pixels */		 
		    
		      oric_vh_render_6pixels(x,y,vh_state.active_foreground_colour, vh_state.active_background_colour, pixel_data,(c & 0x080));		    		  
		    }
		  else
		    {
		      render_text_mode(c,x,y);		  
		    }

	    }
	  // if simple force redraw (flashing overrides that) and no attributes till the end of line
	  // then no need to draw
	 /* if ((force_redraw==1) && (no_attributes_till_eol(previous_RAM,RAM,read_addr,byte_offset+1)))
	    {
	      // no need to redraw
	      break;
	    }*/	  
			
	  x+=6;	
	}
    
      
    }
    /* after HIRES_LIMIT lines have been drawn, force a change of the read address */
	  /* there are HIRES_LIMIT lines of hires/text mode, then 24 lines of text mode */
	  /* the mode can't be changed in the last 24 lines. */
  read_addr_base = (unsigned long)0x0bf68;
  hires_active = 0;
  // last lines in text mode
  for (y = HIRES_LIMIT; y < SCREEN_HEIGHT; y++)
    {
      int x = 0;

      /* foreground colour white */
      oric_vh_update_attribute(7);
      /* background colour black */
      oric_vh_update_attribute((1<<3));
      oric_vh_update_attribute((1<<4));
		  
      int char_line = ((y-HIRES_LIMIT)>>3)*40;
      
      int force_redraw = !vp->draw_only_changes;
      
      for (byte_offset=0; byte_offset<40; byte_offset++)
	{
	  int c,previous_c;
	  unsigned long read_addr;
	  
	  read_addr = (unsigned long)read_addr_base + (unsigned long)byte_offset + (unsigned long)char_line;
	  
	  HANDLE_FORCE_REDRAW;

	  /* if bits 6 and 5 are zero, the byte contains a serial attribute */
	  if (is_serial_attribute(c))
	    {	      
	      oric_vh_update_attribute(c);
	      if (force_redraw)
		{
		  /* display background colour when attribute has been found */
		  oric_vh_render_6pixels(x,y,vh_state.active_foreground_colour, vh_state.active_background_colour, 0,0); /* was (c & 0x080) */
		}
	    }
	  else if (force_redraw)
	    {	 
	      render_text_mode(c,x,y);		  
	    }
	  x += 6;
	}
    }
  if (vp->draw_only_changes)
    {
      // swap current & previous buffers
      UINT8 *swp = current_screen_buffer;
      current_screen_buffer = previous_screen_buffer;
      previous_screen_buffer = swp;
      if (full_refresh)	
	{
	  //memset(previous_screen_buffer,0xFF,SCREEN_BUFFER_SIZE);
	  int i;
	  for (i=0;i<203*40;i+=40)
	    {
	      previous_screen_buffer[i] = 0xFF;
	    }
	}
    }
  
  screen_unlock();
  

}





Uint32 refresh_timer_callback(Uint32 interval, void *param)
{
  // merged flash callback here, so we only have one timer (some SDL implementations don't support
  // several timers so flash is more portable, and refresh_timer is the one refreshing, so why using
  // another (slower) timer? let's keep it simple
  
  vh_state.flash_counter += interval;

  if (vh_state.flash_counter > flash_update_rate)
    {
      vh_state.flash_counter -= flash_update_rate;
      vh_state.flash_state ^=(1<<3);
      oric_vh_update_flash();
     
    }
  
   (void)param;
   video_update();

    return interval;
}
