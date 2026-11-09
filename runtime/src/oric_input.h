#ifndef _ORIC_INPUT
#define _ORIC_INPUT

#include "SDL/SDL.h"

void joystick_init(int enabled);
int joystick_get_axis(int numjoy,int axis);
int joystick_get_button(int numjoy,int button);

// used for key mappings for some international keyboards on Linux and OS X
typedef struct
{
    int  keycode;
    char mapped;
} KeyMapping;

void set_key_mapping(KeyMapping* mappings, int nb_mappings);

void wait_for_no_keys_pressed();

int is_key_pressed(SDLKey k);
const Uint8 *get_keyboard_state();
/* read key is used by BASIC KEY$ routine. It does not process events
   but gets directly keyboard state. So as long as a key is down, read_key returns it
   It does not handle multiple key input, just returns the lowest matching pressed ASCII keycode.
   For multi-key handling use is_key_pressed() or get_keyboard_state()
returns 0 if no key pressed, else returns ASCII code of the key
 */

char read_key_nonblocking(int lower_case);

/*
read_key_event:
 the blocking mode is used in BASIC GET command
 the non-blocking mode is not used for KEY$ since it detects events, not
   the fact that the key is currently pressed. Use read_key instead

   upper/lower case is handled as well as CTRL/SHIFT inputs. #208/#209 are handled as well
returns 0 if no key pressed (nonblocking mode), else returns ASCII code of the key
 */

char read_key_event(int blocking,int lower_case);
void flush_events();

/* used for remapping keys */
int read_keycode();

/* if called with 1, the function enables french keyboard emulation only for letters (not for numbers) */
void enable_french_keyboard(int enable);


#endif
