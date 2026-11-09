
#include "oric_video_draw.h"
#include "oric_machine.h"
#include "my_assert.h"

#include <proto/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <intuition/screens.h>
#include <string.h>

static struct Screen *screen =0;
static struct Window *window=0;
static struct RastPort *rastport=0;
static struct ViewPort *viewport=0;
static PLANEPTR *planes;
static unsigned char *oric_ram;
extern void video_render();
extern void asm_init(const unsigned char *ram,PLANEPTR *planes,const unsigned char *previous_ram);

// this is a buffer completely left to handle to the asm part

static unsigned char oric_previous_ram[0xC000];

static const VideoParams *vp =0;
void  force_refresh()
{
}

struct Window *get_window()
{
  return window;
}

void open_screen(const VideoParams *pvp)
{
  vp = pvp;
  
  
  IntuitionBase = (struct IntuitionBase *)OpenLibrary((unsigned char*)"intuition.library",37);
  if (IntuitionBase == NULL)
    {
      fatal_error("Cannot open intuition.library");
    }
  
  GfxBase = (struct GfxBase *)OpenLibrary((unsigned char*)"graphics.library", 37);
  if (GfxBase == NULL)
    {
      fatal_error("Cannot open graphics.library");
    }
  
   struct NewScreen Screen1 = {
     0,0,320 /*SCREEN_WIDTH*/,SCREEN_HEIGHT,3,             /* Screen of 640 x 480 of depth 8 (2^8 = 256 colours)    */
     DETAILPEN, BLOCKPEN,
     0,                     /* see graphics/view.h for view modes */
     PUBLICSCREEN|SCREENQUIET,              /* Screen types */
     NULL,                      /* Text attributes (use defaults) */
     (UBYTE *)"My New Screen", 
     NULL,
     NULL
     };
     screen = OpenScreen(&Screen1); 
      
  if (!screen)
    {
      fatal_error("Cannot open screen");
    }
  window = OpenWindowTags(NULL,
			  WA_CustomScreen, (LONG)screen,
			  // WA_Title,       "Press Keys and Mouse in this Window",
			  WA_Width,       320,
			  WA_Height,      SCREEN_HEIGHT,
			  WA_Activate,    TRUE,
			  WA_CloseGadget, FALSE,
			  WA_RMBTrap,     TRUE,
			  WA_Borderless,     TRUE,
			  WA_IDCMP,IDCMP_VANILLAKEY |
			  IDCMP_RAWKEY,
			  TAG_END);
  
  viewport = &screen->ViewPort;
  rastport = &screen->RastPort;
  planes = rastport->BitMap->Planes;
  oric_ram = cpu_get_mem();
  
 
  asm_init(oric_ram,planes,oric_previous_ram);
  
  // init SDL for timer & events only
  if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_EVENTTHREAD) != 0) {
    char buf[200];
    sprintf(buf, "Unable to initialize SDL: %s", SDL_GetError());
    fatal_error(buf);
  }
  
}

int get_pen(int R,int G,int B)
{
  (void)R,(void)G,(void)B;
  return 0;
}


void set_colors(const int rgb[], int nb_colors)
{
  UWORD cmap[nb_colors];
  int i;
  for (i=0;i<nb_colors;i++)
    {
      int v = rgb[i];
      // "downgrade" colors to fit into 4 bits & component
      int r = v >> 20;
      int g = (v >> 12) & 0xF;
      int b = (v >> 4) & 0xF;
      cmap[i] = (r << 8) | (g << 4) | b;
      
    }

  LoadRGB4(viewport,cmap,nb_colors);  

 
}


void fatal_error(const char *message)
{
    fprintf(stderr,"%s\n",message);
    close_screen();

}

void close_screen()
{
  if (window)
    {
      CloseWindow(window);
      window = 0;
    }
  
  if (screen)
    {
      CloseScreen(screen);
      screen  =0;
    }
  
  
}


static int mask_y_limit = SCREEN_HEIGHT;


void screen_lock()
{
  
}

void screen_unlock()
{
 
}

void set_mask_y_limit(int y)
{
    mask_y_limit = y >=0 ? y : SCREEN_HEIGHT;
}

int is_mask_active(int y)
{
    return (y < mask_y_limit);
}


Uint32 refresh_timer_callback(Uint32 interval, void *param)
{
  (void)param;
  video_render();
  return interval;
}





