#pragma once

#include "SDL/SDL.h"
#include "oric_video_draw.h"

typedef void (*GfxCallback)(SDL_Surface *,int);
typedef void (*QuitCallback)();



void reset_charset();

// change colors 0-8 or 0-15 (extended mode)
void set_pen(unsigned int index,int rgb);
const int *get_pens();

/*void set_colors(const SDL_Palette *p);*/

void video_init(const VideoParams *vp);
void quit();
void fatal_error(const char *message);

void force_refresh();

