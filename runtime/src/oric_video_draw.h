#pragma once

#include "SDL/SDL.h"
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 224
#define HIRES_LIMIT 200

typedef void (*GfxCallback)(SDL_Surface *,int);
typedef void (*QuitCallback)();

typedef struct VideoParams_
{
  const char *window_title;
  int double_size;   // 1=double video size
  int update_rate;   // in ms
  int full_screen;   // 1=full screen else windowed
  int mask_color;    // -1: no mask
  int extended_colors; // 0: normal, 1: extended
  int draw_only_changes; // 0: brutal complete draw, 1: smart diff draw
  int screen_depth;
  int double_buffering;
  GfxCallback background_callback;
  GfxCallback foreground_callback;
  QuitCallback quit_callback;
  SDL_Palette *palette;
} VideoParams;

void set_screen_params(const VideoParams *pvp,const void *pscreen);
	
int get_pen(int R,int G,int B);

void set_colors(const int rgb[], int nb_colors);


void screen_unlock();

// pass -1 to cancel the limit
void set_mask_y_limit(int y);
// pass -1 to cancel the limit

int is_mask_active(int y);

void draw_6_pixels(int px, int py, int sdl_pen_array[]);
void plot_box(int x, int y, int w, int h, int pen);

void screen_lock();

void open_screen(const VideoParams *pvp);
void close_screen();
void fatal_error(const char *message);

Uint32 refresh_timer_callback(Uint32 interval, void *param);
