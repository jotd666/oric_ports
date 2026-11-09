#ifndef _CUSTOM_H
#define _CUSTOM_H

// put game constants & defines here
enum GameContext { INTRO, MENU, GAME_SPIDER, GAME_REPTILE, GAME_SHIP,
		   GAME_BIRD, GAME_BRIDGE, MISSION_SELECT,
		   GAME_ZORGON, COMPLETED, GAME_OVER };

void set_game_context(enum GameContext gc);
int get_max_nb_lives();
void load_hiscore();
void save_hiscore();

#endif
