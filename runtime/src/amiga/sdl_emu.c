#include "SDL/SDL.h"

#include <exec/types.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <intuition/screens.h>

#include <exec/memory.h>
#include <exec/interrupts.h>
#include <hardware/intbits.h>

struct Window *get_window();

void SDLCALL SDL_Quit()
{
}

void SDLCALL SDL_Delay(Uint32 ms)
{
  // convert milliseconds since each tick means 20 milliseconds
  Delay(ms/20);
}
static int decode_and_reply_intuition_message(SDL_Event *event,struct Message *msg)
{
  struct IntuiMessage * imsg = (struct IntuiMessage *)msg;
  ULONG cls = imsg->Class;
  int retcode = 0;

  if (cls == IDCMP_VANILLAKEY)
    {
      event->type = SDL_KEYDOWN;
      UWORD code = imsg->Code;
      event->key.keysym.sym = code;
      retcode = 1;
    }

  ReplyMsg(msg);
  return retcode;
}

int SDLCALL SDL_WaitEvent(SDL_Event *event)
{
  struct Window *window = get_window();
  struct Message *msg;
  while(1)
    {
      WaitPort(window->UserPort);
      msg = GetMsg(window->UserPort);
      if (msg != NULL)
	{
	  if (decode_and_reply_intuition_message(event,msg))
	    {
	      break;
	    }
	}

    }
  // event always processed!
  return 1;
}
int SDLCALL SDL_PollEvent(SDL_Event *event)
{
  struct Window *window = get_window();
  struct Message *msg = GetMsg(window->UserPort);
  if (msg != NULL)
    {
      return (decode_and_reply_intuition_message(event,msg));
    }
  return 0;
}
Uint8 * SDLCALL SDL_GetKeyState(int *numkeys)
{
  (void)numkeys;
  return NULL;
}
int SDLCALL SDL_NumJoysticks(void)
{
  return 2;
}

Uint8 SDLCALL SDL_JoystickGetButton(SDL_Joystick *joystick, int button)
{
  (void)joystick;
  (void)button;
  return 0;
}
Sint16 SDLCALL SDL_JoystickGetAxis(SDL_Joystick *joystick, int axis)
{
  (void)joystick;
  (void)axis;
  return 0;
};

static Uint32 ticks=0;

void SDL_PumpEvents()
{
}
void SDL_JoystickUpdate()
{
}

Uint32 SDL_GetTicks()
{
  return ticks;
}
static struct Interrupt interrupt;
SDL_NewTimerCallback vbl_callback;
void *vbl_param = NULL;

static void vbl_int()
{
  /*short *v = (short *)0xDFF180;
  v[0] = 0xF00;*/

  ticks += 20;

  (*vbl_callback)(20, vbl_param);
   __asm("clr  d0");   // successfully clears Z so interrupt chain isn't broken
}


SDL_TimerID SDLCALL SDL_AddTimer(Uint32 interval, SDL_NewTimerCallback callback, void *param)
{
  (void)interval;
  interrupt.is_Code = &vbl_int;
  interrupt.is_Data = NULL;
  interrupt.is_Node.ln_Type = NT_INTERRUPT;
  interrupt.is_Node.ln_Pri = 100;
  interrupt.is_Node.ln_Name = "vbl";
  vbl_callback = callback;
  vbl_param = param;

  AddIntServer(INTB_VERTB, &interrupt);
  return (SDL_TimerID)1;
}

char * SDLCALL SDL_GetError(void)
{
  return "unknown error";
}

SDL_bool SDLCALL SDL_RemoveTimer(SDL_TimerID t)
{
  (void)t;
  return 1;
}
SDL_Joystick * SDLCALL SDL_JoystickOpen(int device_index)
{
  (void)device_index;
  return NULL;
}

extern int SDLCALL SDL_Init(Uint32 flags)
{
  (void)flags;
  return 0;
}
