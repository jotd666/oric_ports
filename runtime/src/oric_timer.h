#ifndef _ORIC_TIMER_H
#define _ORIC_TIMER_H

#include <SDL/SDL.h>

typedef struct _OricTimer
{
    int nb_cycles;
    int cycle_counter;
    int delay;
    Uint32 ticks;
} OricTimer;

OricTimer timer_init(int nb_cycles, int delay);

void timer_tick(OricTimer *t);

#endif
