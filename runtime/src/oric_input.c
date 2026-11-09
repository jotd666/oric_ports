#include "SDL/SDL.h"
#include "oric_machine.h"
#include "oric_video.h"
#include "oric_input.h"
#include "oric_basic.h"

#include <strings.h>

#ifdef __APPLE__
#define INLINE
#else
#define INLINE inline
#endif
static SDL_Joystick *tableJoysticks[4];

// contains very poor emulation of ROM #208/#209 values. Useful for protoyping but since
// this does not allow multiple key input, part of the game which uses that needs to
// be rewritten whenever possible

const int peek_208[256] = {
0x38,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0xac,0xbc,0xb4,0x9c,0x56,0xaf,0x56,0x56,
0x56,0x56,0x56,0x56,0x89,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56, // 16-31
0x84,0xa8,0xbb,0xb8,0x9a,0x90,0x80,0xbb,0x8b,0x97,0x87,0xbf,0x8c,0x9b,0x94,0x9f, // 32-47
0x97,0xa8,0xb2,0xb8,0x9a,0x90,0x8a,0x80,0x87,0x8b,0x93,0x93,0x8c,0xbf,0x94,0x9f, // 48-63
0xb2,0xae,0x92,0xba,0xb9,0x9e,0x99,0x96,0x8e,0x8d,0x81,0x83,0x8f,0x82,0x88,0x95, // 64-79
0x9d,0xb1,0x91,0xb6,0x89,0x85,0x98,0xbe,0xb0,0x86,0xaa,0xbd,0x56,0xb5,0x8a,0x9b, // 80-95
0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,
0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0xbd,0x56,0xb5,0x56,0xad,
0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56,0x56
};

static KeyMapping *keyMappings = NULL;
static int nbKeyMappings = 0;
static int french_keyboard = 0;

void set_208(int keycode)
{
  
  if (keycode < 0x100 && keycode >= 0)
    {
      poke(0x208,peek_208[keycode]);
    }

  
}
void enable_french_keyboard(int enable)
{
  french_keyboard = enable;
}



void joystick_init(int enabled)
{
    memset(tableJoysticks,0,sizeof(tableJoysticks));
    if (enabled)
    {
	int numJoystick = SDL_NumJoysticks();
	int i;
	if(numJoystick > 4)
	{
	    numJoystick = 4;
	}
	for(i=0 ; i < numJoystick ; i++)
	{
	    //printf("Nom du joystick %i : %s\n", i, SDL_JoystickName(i));
	tableJoysticks[i] = SDL_JoystickOpen(i);
    
	/*if(tableJoysticks[i])
	{
	    printf("Nombre d'axes : %d\n",     SDL_JoystickNumAxes   (tableJoysticks[i]));
	    printf("Nombre de boutons : %d\n", SDL_JoystickNumButtons(tableJoysticks[i]));
	    printf("... joystick ouvert\n");
	}
	else
	{
	    printf("... probleme d'ouverture avec ce joystick\n");
	    }*/
    
    }
    }
     

}
int joystick_get_button(int numjoy,int button)
{
    int rval = 0;
    SDL_Joystick *j = tableJoysticks[numjoy];
    if (j != NULL)
    {
	rval = SDL_JoystickGetButton(j, button);
    }
    return rval;
}
int joystick_get_axis(int numjoy,int axis)
{
    int rval = 0;
    SDL_Joystick *j = tableJoysticks[numjoy];
    if (j != NULL)
    {
	rval = SDL_JoystickGetAxis(j, axis);
    }
    return rval;
}

void set_key_mapping(KeyMapping* mappings, int nb_mappings)
{
    keyMappings = mappings;
    nbKeyMappings = nb_mappings;
}

int is_key_pressed(SDLKey k)
{
    Uint8 *keystate = SDL_GetKeyState(NULL);
    return (keystate[k]);
}

const Uint8 *get_keyboard_state()
{
    return SDL_GetKeyState(NULL);
}

static inline void handle_keypress(char *s,unsigned int i,int shift_pressed,int lower_case)
{
  static const char *shifted= ")!@#$%^&*(";
  char c;
  if (french_keyboard)
    {
      switch(i)
	{
	  case SDLK_a:
	
	    i = 'Q';
	  
	    break;
	  case SDLK_q:
	
	    i = 'A';
	  
	    break;
	  case SDLK_w:
	
	    i = 'Z';
	  
	    break;
	  case SDLK_z:
	
	    i = 'W';
	  
	    break;
	  case SDLK_SEMICOLON:
	    i = 'M';
	    break;
	  case SDLK_m:
	    i = shift_pressed ? '?' : ',';  
	    break;	
	  case SDLK_COMMA:	
	    i = shift_pressed ? '.' : ';';
	  
	    break;	
	  case SDLK_PERIOD:	
	    i = shift_pressed ? '/' : ':';
	  
	    break;	
	  default:
	    break;
	} 
    
    }
     
  
  switch(i)
    {
      case SDLK_DELETE:
      case SDLK_BACKSPACE:
	c = 127;
	break;
      case SDLK_UP:
	c = 11;
	break;
      case SDLK_DOWN:
	c = 10;
	break;
      case SDLK_RIGHT:
	c = 9;
	break;
      case SDLK_LEFT:
	c = 8;
	break;
 
      default:
        // key mapping
	if (keyMappings!=NULL) {
	  int j;
            for(j = 0; j<nbKeyMappings; j++) {
                if (i==(unsigned int)keyMappings[j].keycode) {
                    i=keyMappings[j].mapped;
                }
            }
        }

	if (lower_case && !shift_pressed)
	  {
	    c = i;
	  }
	else
	  {
	    c=toupper(i);
	  }
	  
	if (shift_pressed)
	  {
	    if (i>='0' && i<='9')
	      {
		c = shifted[i-'0'];
	      }
	  }

	break;
    }  
  
  s[0] = c;
       
  set_208(c);
	
}


char read_key_nonblocking(int lower_case)
{
  static const int special[] = { SDLK_BACKSPACE,SDLK_RETURN,SDLK_ESCAPE };
  
  unsigned int i;
    unsigned int i_min = 32;
    unsigned int i_max = 128;
  
  char c = 0;
  const Uint8* ks;

  flush_events();
  ks = get_keyboard_state();
  int shift_pressed = ks[SDLK_RSHIFT] || ks[SDLK_LSHIFT];
  
  poke(0x209,shift_pressed ? 0xa4 : 0x56);
  
    if (keyMappings != NULL) {
        // we need to test all keys
        i_min = 0;
        i_max = 282;
    }
    
  for (i=i_min;i<i_max;i++)
    {

      if (ks[i])
	{
	  handle_keypress(&c,i,shift_pressed,lower_case);

	  break;
	}
    }
  
  if (c==0)
    {
      if (ks[SDLK_DELETE] || ks[SDLK_BACKSPACE])
	{
	  c = 127;
	}
      else if (ks[SDLK_UP])
	{
	  c = 11;
	}
      else if (ks[SDLK_DOWN])
	{
	  c = 10;
	}
      else if (ks[SDLK_RIGHT])
	{
	  c = 9;
	  }
      else if (ks[SDLK_LEFT])
	{
	  c = 8;
	  }
	
      if (c==0)
	{
  
      for (i=0;i<sizeof(special)/sizeof(int);i++)
	{
	  int si = special[i];
	  if (ks[si])
	    {
	      c = si;
	      break;
	    }
	    }
	  }
    }
  set_208(c);
 return c;
}

void wait_for_no_keys_pressed()
{
  while(read_key_nonblocking(0) != 0)
    {
      SDL_Delay(10);
    }
  
}

char read_key_event(int blocking,int lower_case)
{
  SDL_Event event;
  int ctrl_pressed = 0;
  int shift_pressed = 0;
  int event_processed = 0;
  char c = 0;
  
  while(1)
    {
      if (blocking)
	{
	  event_processed = SDL_WaitEvent(&event);
	}
      else
	{
	  event_processed = SDL_PollEvent(&event);
	}

      if (!event_processed)
	{
	  break;
	}
      
      switch(event.type)
	{
	  case SDL_QUIT:
	    quit();
	    break;
	  case SDL_KEYUP:	      
	    switch (event.key.keysym.sym)
	      {
		case SDLK_RSHIFT:
		case SDLK_LSHIFT:
		  shift_pressed = 0;
		  poke(0x209,0x56); // reset
		  
		  break;
		case SDLK_RCTRL:
		case SDLK_LCTRL:
		  ctrl_pressed = 0;
		  poke(0x209,0x56); // reset
		  break;
		default:
		  poke(0x208,0x56); // reset

		  break;
	      }
	    break;
	  case SDL_KEYDOWN:	
	    if (!ctrl_pressed)
	      {

		switch (event.key.keysym.sym)
		  {
		      
		    case SDLK_RSHIFT:
		    case SDLK_LSHIFT:
		      shift_pressed = 1;
		      poke(0x209,0xA4);
		      break;
		    case SDLK_RCTRL:
		    case SDLK_LCTRL:
		      ctrl_pressed = 1;
		      poke(0x209,0xA2);
		      break;
		    default:
		      handle_keypress(&c,event.key.keysym.sym,shift_pressed,lower_case);

		      break;
		  }
	      }
	    break;
	  default:
	    break;
	}		    

      if (c!=0)
	{
	  break;
	}
      
	
    }

  

  return c;
}

void flush_events()
{
    SDL_Event event;

    while (SDL_PollEvent(&event));
}

int read_keycode()
{
    SDL_Event event;
    int event_processed = 0;
    
    while(1)
    {
        event_processed = SDL_WaitEvent(&event);
        
        if (!event_processed)
        {
            return 0;
        }
        
        if(event.type == SDL_KEYDOWN)
            return event.key.keysym.sym;
    }
}
