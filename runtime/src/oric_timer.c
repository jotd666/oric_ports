#include "oric_timer.h"
#include "my_assert.h"


static OricTimer *last_timer = 0;


OricTimer timer_init(int nb_cycles, int delay)
{
    OricTimer rval;
    rval.cycle_counter = 0;
    rval.nb_cycles = nb_cycles;
    rval.delay = delay;
    rval.ticks = SDL_GetTicks(); // current time

    return rval;
}

void timer_tick(OricTimer *t)
{
    if (last_timer != t)
    {
	// break the sequence
	
	last_timer = t;

	timer_init(t->nb_cycles,t->delay);
    }
    
    my_assert(t->nb_cycles > 0);
    t->cycle_counter++;

    if (t->cycle_counter == t->nb_cycles)
    {	
	t->cycle_counter = 0;
	
	Uint32 new_ticks = SDL_GetTicks();
	int delta = new_ticks - t->ticks;
	t->ticks = SDL_GetTicks();
	
	if (delta < t->delay)
	{
	    SDL_Delay(t->delay - delta);	    
	}
	    	
    }
}
