// converted by asm2c (C) JOTD 2008

#include "opcodes_legacy.h"
#include "xenon1_custom.h"
#include "constants.h"
#include "xenon1.h"
#include "my_assert.h"

#include "oric_basic.h"
#include "oric_input.h"
#include "oric_video.h"
#include "oric_timer.h"
#include "oric_sound.h"

#include "SDL/SDL.h"

#include <string.h>

//#define START_AT_ZORGONS 1
//#define START_AT_PARATROOPS 1
//#define DISENGAGE_ALWAYS
//#define START_AT_METEORITES
//#define START_AT_LEVEL_2

#ifdef START_AT_ZORGONS
#define START_AT_PARATROOPS 1
#endif

#ifdef START_AT_PARATROOPS
#define START_AT_METEORITES
#endif


// TODO:
//
// 
//
// - remove volume settings
// - add sounds



typedef struct
{
    int esc_pressed;
    int fire_pressed;
    int left_pressed;
    int right_pressed;
    int fire_was_pressed;
    int fire_down;
    int disengage_pressed;
} Controls;

Controls controls;

int initial_number_of_lives = 3;

int falling_sound_id[20];
int special_ship_sound = -1;



static inline void one_foe_killed(int snd)
{
    if (snd) sound_play("killed");
    dec(number_of_foes_961f);
}

static inline void one_foe_spawned()
{
    inc(number_of_foes_961f);
}

static void stop_kamikaze_sounds()
{
    unsigned int i;
    for (i = 0; i < sizeof(falling_sound_id)/sizeof(int); i++)
    {
	sound_stop(falling_sound_id[i]);
	falling_sound_id[i] = 0;
    }
}

static void update_controls()
{
    
    SDL_PumpEvents();

    const Uint8 *keys = get_keyboard_state();

    controls.right_pressed = keys[SDLK_RIGHT];
    controls.left_pressed = keys[SDLK_LEFT];
    controls.fire_was_pressed = controls.fire_down;
    controls.fire_pressed = keys[SDLK_RCTRL] || keys[SDLK_LCTRL];
    controls.disengage_pressed = keys[SDLK_UP];
    controls.esc_pressed = keys[SDLK_ESCAPE];
    controls.fire_down = controls.fire_pressed;
    
    if (keys[SDLK_p])
    {
	SDL_Event event;
	flush_events();
	int paused = 1;
	
	// pause mode
	while(paused)
	{
	    if (SDL_WaitEvent(&event))
	    {
		switch(event.type)
		{
		  case SDL_QUIT:
		    quit();
		    break;
		  case SDL_KEYDOWN:	      
		    if (event.key.keysym.sym == SDLK_p)
		    {
			paused = 0;
		    }
		    break;
		}
	    }
	}
	// way for P key up
	int key_up = 0;
	while(!key_up)
	{
	    if (SDL_WaitEvent(&event))
	    {
		switch(event.type)
		{
		  case SDL_QUIT:
		    quit();
		    break;
		  case SDL_KEYUP:	      
		    if (event.key.keysym.sym == SDLK_p)
		    {
			key_up = 1;
		    }
		    break;
		}
	    }
	}
	
    }
    
    SDL_JoystickUpdate();
    int horiz = joystick_get_axis(0,0);
    
    if (horiz > 16384)
    {
	controls.right_pressed = 1;
    }
    else if (horiz < -16384)
    {
	controls.left_pressed = 1;
    }
    if (joystick_get_button(0,1))
    {
	controls.fire_down = 1;
    }

    
    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
	switch (event.type)
	{

	  case SDL_QUIT:
	    quit();
	    break;	      	      
	    
	    
	  case SDL_JOYBUTTONDOWN:
	  {
	      SDL_JoyButtonEvent *joy_event  = (SDL_JoyButtonEvent*) & event;
	      if (joy_event->button == 1)
	      {
		  controls.fire_pressed = 1;
	      }
	      else if (joy_event->button == 2)
	      {
		  controls.disengage_pressed = 1;
	      }
	      break;
	  }
	}
	}
    
}
static void poke_41F_80()
{
    poke(0x41F,0x80);
    update_controls(); // controls up to date
}
#define goto_menu P_text_and_menu_0727();return

/* display title screen */


static void P_level_1_0774();
static void P_refuel_level_0A12();
static void P_meteorites_0B0B();
static void P_move_ship_0e33();
static void P_decrease_fuel_0F43();
static void P_0F69();
static void P_1058();
static void P_10B6();
static void P_11A2();
static void P_animate_enemy_shots();
static void P_1222();
static void P_1241();
static void P_1260();
static void P_12DC();
static void P_135A();
static void P_1365();
static void P_139C();
static void P_1436();
static void P_drop_paratroop_14A0();
static void P_14DF();
static void P_15F2();
static void P_162E();
static void P_1760();
static void P_1781();
static void P_1889();
//static void P_2000();
static void P_209C();
static void P_4124();
static void P_zorgon_bonus_418D();
static void P_4201();
static void P_adjust_xxx_level_20A9();
static void P_draw_red_bar_20EF();
static void P_214A();
static void P_221F();
static void P_show_mothership_intro_2230();
static void P_intro_screen_22F8();
static void P_2323();
static void P_display_bonus_4000();
static void P_write_hires_string_4050();
static void P_refuel_4234(int *engine_id);
static void P_4387();
static void P_do_refuel_44B7();
static void P_tempo_Y_44FF();
static void P_ship_explodes_451C();
static void P_ship_explosion_45B7();
static void P_init_fuel_display_45F6();
static void P_display_text_468A();
static void P_next_line_46C6();
static void P_main_menu_46E7();
static void P_plot_472A();
static void P_game_over_4770();
static void P_47B6();
static void P_482D();
static void P_enter_hiscore_name_489E();
static void P_hiscore_enter_letter_4909();
static void P_decode_char_4984();
static void P_499C();
static void P_0_times_1_4A41();
static void P_sound_routine_4A57();
static void P_sound_routine_4A8B();
static void P_4ABF();
static void P_4ADF();
static void P_4AFE();
static void P_animate_shoot_4B12();
static void P_4B55();
static void P_4B73();
static void P_4B9B();
static void P_4BB9();
static void P_4BCC();
static void P_4BF4();
static void P_4C30();
static void P_4C94();
static void P_4C95();
static void P_4CEB();
static void P_4D45();
static void P_animate_birds_5000();
static void P_screen_copy_5172();
static void P_fill_zone_51BB();
static void P_51F2();
static void P_523C();
static void P_52D4();
static void P_537C();
static void P_53A0();
static void P_542C();
static void P_5506();
static void P_55AD();
static void P_55D9();
static void P_5633();
static void P_handle_rescue_ship_5724();
static void P_remove_rescue_ship_5783();
static void P_57A8();
static void P_5871();
static void P_58CE();
static void P_5917();
static void P_5981();
static void P_59A2();
static void P_59C5();
static void P_handle_ship_shots_zorgons_5A32();
static void P_5BC0();
static void P_move_zorgons_5C16();
static void P_draw_mothership_5CE2();
static void P_draw_mothership_top_5D62();
static void P_5DA1();
static void P_5DFF();
static void P_animate_zorgon_shots_5E2A();
static void P_5E53();
static void P_decide_zorgon_shoot_5e9a();
static void P_zorgon_shoot_5ead();
static void P_animate_zorgon_ship_5EE4();
static void P_5F5B();
static void P_anim_foe_death_5F96();
static void P_update_player_status_5FDE();
static void P_lose_a_life_6029();
static void P_6037();
static void P_6090();
static void P_60BF();
static void P_6152();
static void P_6262();
static void P_6333();
static void P_zorgon_ship_explodes_63A5();
static void P_63D7();
static void P_increase_score_641B();

static const char *sounds[] =
{ "killed", "shoot", "falling", "title",
  "mothership_laser","big_explosion","explosion", "kamikaze",
  "special_ship","yell", "level_end", "alarm", "metallic_crash", "wall_break" };

static void load_sounds()
{
    unsigned int i = 0;
    for (i = 0; i < (sizeof(sounds)/sizeof(char *)); i++)
    {
	sound_load("snd",sounds[i],"wav",0);
    }

    sound_load("snd","rain","wav",1); // loops
    sound_load("snd","refuel","wav",1); // loops
    sound_load("snd","engine","wav",1); // loops
}

static int fuel_too_low()
{
    static int old_value = 40;
    
    int rval = peek(fuel_gauge_96ff) < 10;

    if (rval && (old_value >= 10))
    {
	sound_play("alarm");
    }
    old_value = peek(fuel_gauge_96ff);

#ifdef DISENGAGE_ALWAYS
    return 1;
#else
    return rval;
#endif
}

subroutine(E_0700)
{
    load_sounds();
    memset(&controls,0,sizeof(controls));
    memset(falling_sound_id,0,sizeof(falling_sound_id));
    
    /* game used SMC to prevent intro from being replayed
       was it on purpose? 
	lda(jump_flag_05f0);
	cmp_imm(0x01);
	bne(B_702);
label(B_702)
    */
	jsr(P_show_mothership_intro_2230);

/* clear hiscore zone 0x6700-0x67FF */
	lda_imm(0x00);
	ldx_imm(0x00);
label(B_0707)
	sta_X(hiscore_buffer_6700);
	dex;
	bne(B_0707);

	// init hiscores to 1000
	
	lda_imm(0x01);
	ldx_imm(0x39);
label(B_0711)
	lda_imm(0x01);
	sta_X(hiscore_buffer_6700);
	txa;
	sec;
	sbc_imm(0x06);
	tax;
	bpl(B_0711);

	// init names to .......
	
	lda_imm(0x2E);
	ldx_imm(0xC8);
label(B_0721)
	sta_X(0x677F);
	dex;
	bne(B_0721);

	load_hiscore();
	
	jsr(P_text_and_menu_0727);
	rts;
	  
}
end_subroutine

/* switch to menu screen */


subroutine(P_text_and_menu_0727)
{
    flush_events();
    
    lda_imm(0x01);
    sta(hires_text_flag_05fe);
    move16(0x735,0);
    rts;
  
}
end_subroutine

/* menu screen */

subroutine(E_0735)
{

    lda_imm(0x07);
    sta(0x00);
    lda_imm(0xFF);
    sta(0x01);
    jsr(P_sound_routine_4A57);
    jmpl(P_main_menu_46E7);
  
}
end_subroutine

subroutine(P_0743)
{

    lda_imm(0x02);
    sta(hires_text_flag_05fe);
    move16(0x751,0x0);
    rts;
  
}
end_subroutine

/* start game */
subroutine(E_0751)
{

	lda_imm(0x00);
	ldx_imm(0x00);
label(B_0755)
	sta_X(0x9000);
	dex;
	bne(B_0755);

	sta(0x96F3);

	// reset 9 digit score
	
	ldx_imm(0x09);
label(B_0760)
	sta_X(score_9610);
	dex;
	bpl(B_0760);
  
	lda_imm(initial_number_of_lives);
	sta(nb_lives_9616);
	lda(0x05FF);
	sta(skill_level_9619);

	jsr(P_4ABF);
#ifdef START_AT_METEORITES
	jmpl(P_meteorites_0B0B);
#else
	jmpl(P_level_1_0774);	
#endif
}
end_subroutine

// level 1

subroutine(P_level_1_0774)
{
#ifdef START_AT_LEVEL_2
    jmp(J_level_2_08D7);
#endif
	jsr(P_1781);
	inc(0x9618);
	lda(0x9618);
	cmp_imm(0x0A);
	bne(J_0789);
  
	lda_imm(0x00);
	sta(0x9618);
	inc(0x9617);
label(J_0789)
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_59A2);
	lda_imm(0x01);
	sta(stage_9600);
	sta(0x0408);
	lda_imm(0xA2);
	sta(0x9601);
	lda_imm(0x8A);
	sta(0x9602);
	lda(0x6601);
	sta(0x0405);
	jsr(P_1058);
	jsr(P_4BF4);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	jsr(P_init_fuel_display_45F6);
	jsr(P_60BF);
	lda_imm(0x00);
	sta(0x041F);
	sta(0x0406);
	lda_imm(0x04);
	sta(number_of_foes_961f);
	sta(0x66FF);

	OricTimer tmr = timer_init(1,50);
	
	// game loop (level 1)
label(J_07CA)
	lda(0x66FF);
	cmp(number_of_foes_961f);
	bne(B_07E0);
  
	dec(0x0403);
	bne(B_07F7);
  
	lda(0x040D);
	sta(0x0403);
	jsr(P_decrease_fuel_0F43);
label(B_07E0)
	lda(number_of_foes_961f);
	sta(0x66FF);
	lda(0x040D);
	clc;
	adc(0x6602);
	sta(0x040D);
	bcc(B_07F7);
  
	lda_imm(0xB4);
	sta(0x040D);
label(B_07F7)
	dec(0x0408);
	bne(J_081F);
  
	lda_imm(0x0A);
	sta(0x0408);
	lda(0x9601);
	cmp_imm(0xA2);
	beq(B_0815);
  
	lda_imm(0xA2);
	sta(0x9601);
	lda_imm(0x8A);
	sta(0x9602);
	jmp(J_081F);
  
label(B_0815)
	lda_imm(0x00);
	sta(0x9601);
	lda_imm(0x70);
	sta(0x9602);
	
	label(J_081F)
	
	jsr(P_animate_birds_5000);
	jsr(P_anim_foe_death_5F96);

	// slowed down if too few foes
	// not required anymore
	/*
	lda_imm(0x06);
	sec;
	sbc(number_of_foes_961f);
	cmp_imm(0x02);
	beq(B_0839);  
	tax;
label(B_0839)
	*/
	lda(skill_level_9619);
	cmp_imm(0x01);
	bcs(B_0843);

	// enemy shoot is faster from level 1 onwards
	jsr(P_animate_enemy_shots);
label(B_0843)
jsr(P_0F69);
	jsr(P_move_ship_0e33);
	lda(0x041F);
	bne(B_08B6);
  
	jsr(P_animate_enemy_shots);
	jsr(P_0F69);
	jsr(P_move_ship_0e33);
	jsr(P_4ADF);
	jsr(P_animate_shoot_4B12);

	timer_tick(&tmr);
	
	update_controls();

	if (controls.esc_pressed)
	{
	   goto_menu;
	}
  
	lda(number_of_foes_961f);
	bne(B_087C);

	// end of level 1
	
	jsr(P_4B73);
	jsr(P_display_bonus_4000);
	jmp(J_level_2_08D7);
  
	label(B_087C)
	if (fuel_too_low())
	{
	/* disengage */

	if (controls.disengage_pressed)
	{	    
	    jsr(P_refuel_level_0A12);
	    
	    lda(0x041F);
	    cmp_imm(0x80);
	    beq(B_08AB);
  
	    lda_imm(0x23);
	    sta(fuel_gauge_96ff);
	    jsr(P_lose_a_life_6029);
	    lda(0x041F);
	    beq(B_08AB);
  
	    jmpl(P_game_over_4770);
  
	    label(B_08AB)
	    jmp(J_0789);
	}
	}
  
	lda(0x041F);
	bne(B_08B6);
  
	jmp(J_07CA);
  
label(B_08B6)
	jsr(P_lose_a_life_6029);
	lda(0x041F);
	bne(B_08D4);
  
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_4BF4);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	jsr(P_init_fuel_display_45F6);
	jsr(P_6152);
	jmp(J_07CA);
  
label(B_08D4)
	jmpl(P_game_over_4770);
  
label(J_level_2_08D7)
randomize_background();

	lda_imm(0x5A);
	sta(0x040D);
	inc(0x9618);
	lda(0x9618);
	cmp_imm(0x0A);
	bne(J_08EE);
  
	inc(0x9617);
	lda_imm(0x00);
	sta(0x9618);
label(J_08EE)
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_4ABF);
	jsr(P_59A2);
	jsr(P_init_fuel_display_45F6);
	jsr(P_60BF);
	jsr(P_4BF4);
	lda_imm(0x00);
	sta(0x041F);
	sta(0x0406);
	lda_imm(0x80);
	sta(0x0407);
	lda_imm(0x02);
	sta(stage_9600);
	lda_imm(0x00);
	sta(0x9601);
	sta(0x9609);
	lda_imm(0x80);
	sta(0x9602);
	jsr(P_1058);
	lda_imm(0x0C);
	sta(number_of_foes_961f);
	sta(0x66FF);

	tmr = timer_init(1,40);
	
	// level 2 game loop
	label(J_092C)
	timer_tick(&tmr);
	
	lda(0x66FF);
	cmp(number_of_foes_961f);
	bne(B_0942);
  
	dec(0x0403);
	bne(B_0959);
  
	lda(0x040D);
	sta(0x0403);
	jsr(P_decrease_fuel_0F43);
label(B_0942)
	lda(number_of_foes_961f);
	sta(0x66FF);
	lda(0x040D);
	clc;
	adc(0x6604);
	sta(0x040D);
	bcc(B_0959);
  
	lda_imm(0xC8);
	sta(0x040D);
	
	label(B_0959)
	jsr(P_animate_birds_5000);
	jsr(P_anim_foe_death_5F96);
	
	lda_imm(0x0C);
	sec;
	sbc(number_of_foes_961f);
	beq(B_0971);
  
	tax;
label(B_0968)
	ldy(0x6603);
label(B_096B)
	dey;
	bne(B_096B);
  
	dex;
	bne(B_0968);
  
label(B_0971)
	lda(skill_level_9619);
	cmp_imm(0x01);
	bcs(B_097B);
  
	jsr(P_animate_enemy_shots);
label(B_097B)
	jsr(P_0F69);
	jsr(P_move_ship_0e33);
	lda(0x041F);
	bne(B_09F1);
  
	jsr(P_animate_enemy_shots);
	jsr(P_0F69);
	jsr(P_move_ship_0e33);
	jsr(P_4ADF);
	jsr(P_animate_shoot_4B12);

       

	update_controls();

	if (controls.esc_pressed)
	{
	    goto_menu;
	}
	
	if (!peek(number_of_foes_961f))
	{
	    
	    // end of level 2
	    
	    jsr(P_4B73);
	    jsr(P_display_bonus_4000);
	    randomize_background();
	    jmpl(P_meteorites_0B0B);
	} 
	
	// fuel low
	if (fuel_too_low())
	{
	    /* disengage */

	    if (controls.disengage_pressed)
	    {
		/*
		  lda(VIA_ PB);
		  cmp_imm(0xBC);
		  bne(B_09E6);
  
		  lda(VIA_PA);
		  cmp_imm(0xF7);
		  bne(B_09E6);
		*/
	    
		jsr(P_refuel_level_0A12);
		lda(0x041F);
		cmp_imm(0x80);
		beq(B_09E3);
  
		lda_imm(0x23);
		sta(fuel_gauge_96ff);
		jsr(P_lose_a_life_6029);
		lda(0x041F);
		beq(B_09E3);
  
		jmpl(P_game_over_4770);
  
		label(B_09E3)
		jmp(J_08EE);
	    }
	}

	jsr(P_5917);
	lda(0x041F);
	bne(B_09F1);
  
	jmp(J_092C);
  
label(B_09F1)
	jsr(P_lose_a_life_6029);
	lda(0x041F);
	bne(B_0A0F);
  
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_4BF4);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	jsr(P_init_fuel_display_45F6);
	jsr(P_6152);
	jmp(J_092C);
  
label(B_0A0F)
	jmpl(P_game_over_4770);
  
}
end_subroutine

subroutine(P_refuel_level_0A12)
{
    lda_imm(0x0C);
    jsr(P_write_hires_string_4050);
    jsr(P_59A2);
    lda_imm(0x00);
    sta(0x00);
    lda_imm(0xCF);
    sta(0x01);
    lda_imm(0xBF);
    sta(0x02);
    lda_imm(0xF4);
    sta(0x03);
    lda_imm(0x18);
    sta(0x04);
    lda_imm(0x07);
    sta(0x05);
    lda_imm(0x07);
    sta(0x06);
    jsr(P_screen_copy_5172);
    jsr(P_draw_mothership_5CE2);
    lda_imm(0x00);
    sta(0x96E6);
    sta(0x96E0);
    sta(0x041F);
    lda_imm(0x01);
    sta(0x96E7);
    sta(0x96E8);
    sta(0x96E9);
    sta(0x96E0);
    lda_imm(0xFF);
    sta(0x96EB);
    lda_imm(0x03);
    sta(0x96EA);
    jsr(P_init_fuel_display_45F6);
    jsr(P_4C30);
    jsr(P_4124);
    lda_imm(0x01);
    sta(0x040E);
    sta(0x0403);
    sta(0x0404);

    OricTimer tmr = timer_init(5,40);

    int engine_sound_id = 0;
    
    // refuel loop
    while(1)
    {
	
	timer_tick(&tmr);

	dec(0x0403);
	bne(B_0A8F);
  
	dec(0x0404);
	bne(B_0A8F);
  
	lda_imm(0x00);
	sta(0x0403);
	lda_imm(0x04);
	sta(0x0404);
	jsr(P_decrease_fuel_0F43);
	label(B_0A8F)
	jsr(P_6037);
	P_refuel_4234(&engine_sound_id);
	jsr(P_4387);
	dec(0x0408);
	bne(B_0AAF);
  
	dec(0x040A);
	bne(B_0AAF);
  
	lda_imm(0x50);
	sta(0x0408);
	lda_imm(0x02);
	sta(0x040A);
	jsr(P_1222);
	label(B_0AAF)
	dec(0x040B);
	bne(B_0AC6);
  
	dec(0x040C);
	bne(B_0AC6);
  
	lda_imm(0x50);
	sta(0x040B);
	lda_imm(0x02);
	sta(0x040C);
	jsr(P_1241);
	label(B_0AC6)
	dec(0x0409);
	bne(B_0AD4);
  
	lda(0x660D);
	sta(0x0409);
	jsr(P_12DC);
	label(B_0AD4)
	dec(0x040E);
	bne(B_0AE2);
  
	lda(0x660D);
	sta(0x040E);
	jsr(P_1260);
	label(B_0AE2)
	dec(0x0401);
	if (r.z)
	{
	    lda_imm(0x4B);
	    sta(0x0401);
	    jsr(P_6262);
	}
  	    
	if (peek(0x041F)) break;
    }
    sound_stop(engine_sound_id);

    rts;
  
}
end_subroutine

subroutine(P_meteorites_0B0B)
{
#ifdef START_AT_ZORGONS
    poke(0x041F,0x0);
    jmp(J_zorgons_0D13);
#endif
#ifdef START_AT_PARATROOPS
    jmp(J_paratrons_0BE4);
#endif
    // level 3: asteroids
    
	inc(0x9618);
	lda(0x9618);
	cmp_imm(0x0A);
	bne(J_0B1D);
  
	inc(0x9617);
	lda_imm(0x00);
	sta(0x9618);
label(J_0B1D)
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_59A2);
	jsr(P_135A);
	jsr(P_1889);
	
	jsr(P_init_fuel_display_45F6);
	lda_imm(0x00);
	sta(0x041F);
	lda(0x6605);
	sta(number_of_foes_961f);

	int rain_sound = -1;
	
	OricTimer tmr = timer_init(2,40);
	// game loop
	label(J_0B39)

	if (rain_sound == -1)
	{
	    rain_sound = sound_play("rain");
	}
	
	dec(0x0403);
	bne(B_0B5C);
  
	lda_imm(0xC8);
	sta(0x0403);
	jsr(P_decrease_fuel_0F43);
	lda_imm(0x00);
	sta(0x961A);
	sta(0x961B);
	sta(0x961C);
	sta(0x961D);
	lda_imm(0x01);
	sta(0x961E);
	jsr(P_increase_score_641B);
label(B_0B5C)
	jsr(P_139C);
	jsr(P_move_ship_0e33);
	lda(0x041F);
	bne(B_killed_0BC3);

	timer_tick(&tmr);

	update_controls();
	if (controls.esc_pressed)
	{
	    sound_stop(rain_sound);
	    goto_menu;
	}
	
	if (fuel_too_low())
	{
	    if (controls.disengage_pressed)
	    {
		sound_stop(rain_sound);
		rain_sound = -1;
		
		/*lda(VIA_ PB);
		  cmp_imm(0xBC);
		  bne(B_0BAA);
		  
		  lda(VIA_PA);
		  cmp_imm(0xF7);
		  bne(B_0BAA);*/
		
		jsr(P_refuel_level_0A12);
		lda(0x041F);
		cmp_imm(0x80);
		beq(B_0BA7);
		
		lda_imm(0x23);
		sta(fuel_gauge_96ff);
		jsr(P_lose_a_life_6029);
		lda(0x041F);
		beq(B_0BA7);
		
		jmpl(P_game_over_4770);
		
		label(B_0BA7)
		jmp(J_0B1D);
	    }
	}
	
	lda(number_of_foes_961f);
	bne(B_0BC0);
	
	// end of level
	
	sound_stop(rain_sound);
	
	/*lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);*/
	
	jsr(P_display_bonus_4000);
	randomize_background();
	jmp(J_paratrons_0BE4);
  
label(B_0BC0)
	jmp(J_0B39);
  
label(B_killed_0BC3)
	sound_stop(rain_sound);
	rain_sound = -1;
	
	jsr(P_lose_a_life_6029);
	lda(0x041F);
	bne(B_0BE1);
  
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	jsr(P_init_fuel_display_45F6);
	jsr(P_1889);
	jsr(P_135A);
	jmp(J_0B39);
  
label(B_0BE1)
	jmpl(P_game_over_4770);

// level 4: paratrons

label(J_paratrons_0BE4)

	inc(0x9618);
	lda(0x9618);
	cmp_imm(0x0A);
	bne(J_0C01);
  
	inc(0x9617);
	lda_imm(0x00);
	sta(0x9618);
label(J_0C01)
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_59A2);
	lda_imm(0x00);
	sta(0x6908);
	jsr(P_init_fuel_display_45F6);
	jsr(P_1436);
	jsr(P_15F2);
	jsr(P_4201);
	lda_imm(0x00);
	sta(0x041F);
	lda(0x6607);
	sta(number_of_foes_961f);

	tmr = timer_init(1,40);
	
	// game loop
label(J_0C25)
	dec(0x0403);
	bne(B_0C32);
  
	lda_imm(0xC8);
	sta(0x0403);
	jsr(P_decrease_fuel_0F43);
label(B_0C32)
	jsr(P_542C);
	jsr(P_542C);
	jsr(P_anim_foe_death_5F96);
	jsr(P_55D9);

	// make paratroops appear from time to time (random!)

	//lda(VIA_T1CL);
	r.a = cpu_random_8_bit();
	cmp_imm(0xFE);
	bcc(B_0C48);
  
	
	jsr(P_drop_paratroop_14A0);
label(B_0C48)
	jsr(P_handle_rescue_ship_5724);
	jsr(P_4ADF);
	jsr(P_animate_shoot_4B12);

	update_controls();
	
	timer_tick(&tmr);
  
	dec(0x0400);
	bne(B_0C6D);
  
	lda(0x6609);
	sta(0x0400);
	jsr(P_14DF);
	jsr(P_53A0);
label(B_0C6D)
	jsr(P_162E);
	dec(0x0401);
	bne(B_0C7E);
  
	lda(0x660A);
	sta(0x0401);
	jsr(P_523C);
	label(B_0C7E)
	
	if (fuel_too_low())
	{
  
	    /* disengage */

	    if (controls.disengage_pressed)
	    {	
  
		jsr(P_refuel_level_0A12);
		lda(0x041F);
		cmp_imm(0x80);
		beq(B_0CAD);
  
		lda_imm(0x23);
		sta(fuel_gauge_96ff);
		jsr(P_lose_a_life_6029);
		lda(0x041F);
		beq(B_0CAD);
  
		jmpl(P_game_over_4770);
  
		label(B_0CAD)
		jmp(J_0C01);
	    }
	}
	
	lda(0x041F);
	bne(B_0CE2);

	// ESC

	if (controls.esc_pressed)
	{
	    goto_menu;
	}
	
	lda(number_of_foes_961f);
	bne(B_0CDF);
  
	jsr(P_4B73);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	jsr(P_display_bonus_4000);
	randomize_background();
	jmp(J_zorgons_0D13);  // zorgon
  
label(B_0CDF)
	jmp(J_0C25);

// man killed
label(B_0CE2)
	lda_imm(0x00);
	sta(0x6908);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	jsr(P_lose_a_life_6029);
	lda(0x041F);
	bne(B_0D10);
  
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_15F2);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	jsr(P_init_fuel_display_45F6);
	jsr(P_1436);
	jmp(J_0C25);
  
label(B_0D10)
	jmpl(P_game_over_4770);

// last level: the zorgons

label(J_zorgons_0D13)

	inc(0x9618);
	lda(0x9618);
	cmp_imm(0x0A);
	bne(J_0D25);
  
	inc(0x9617);
	lda_imm(0x00);
	sta(0x9618);
label(J_0D25)
	lda_imm(0x0C);
	jsr(P_write_hires_string_4050);
	jsr(P_59A2);
	jsr(P_5DA1);
	lda_imm(0x01);
	sta(number_of_foes_961f);
	jsr(P_4BF4);
	jsr(P_5BC0);
	jsr(P_init_fuel_display_45F6);
	jsr(P_59C5);
	jsr(P_5DFF);
	lda_imm(0x09);
	sta(0x9620);
	sta(0x9621);
	lda_imm(0x01);
	sta(0x0406);
	sta(0x0400);
	lda_imm(0x00);
	sta(0x690B);

	tmr = timer_init(2,40);
	// game loop
label(J_0D59)
	dec(0x0403);
	bne(B_0D66);
  
	lda_imm(0x00);
	sta(0x0403);
	jsr(P_decrease_fuel_0F43);
label(B_0D66)
	dec(0x0400);
	bne(B_0D86);
  
	lda(0x660C);
	sta(0x0400);
	jsr(P_move_zorgons_5C16);
	lda(0x9620);
	beq(B_0D86);
  
	dec(0x9621);
	bpl(B_0D86);
  
	lda_imm(0x09);
	sta(0x9621);
	dec(0x9620);
label(B_0D86)
	dec(0x0406);
	bne(B_0D97);
  
	lda(0x660B);
	sta(0x0406);
	
	jsr(P_animate_zorgon_shots_5E2A);
	jsr(P_animate_zorgon_ship_5EE4);
	label(B_0D97)
	
	jsr(P_decide_zorgon_shoot_5e9a);
	jsr(P_handle_ship_shots_zorgons_5A32);
	
	jsr(P_move_ship_0e33);
	jsr(P_4ADF); // shared
	jsr(P_animate_shoot_4B12);
	lda(0x041F);
	cmp_imm(0x80);
	bne(B_0DC6);

	// level complete
	
	/*lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);*/
	
	jsr(P_zorgon_bonus_418D);

	// harder
	lda(skill_level_9619);
	beq(B_0DC3);
  
	dec(skill_level_9619);
	
label(B_0DC3)
// game completed: back to level 1
	jmpl(P_level_1_0774);
  
label(B_0DC6)
	lda(0x041F);
	bne(B_0E20);



	update_controls();
	
	if (controls.esc_pressed)
	{
	    goto_menu;
	}
	
	if (fuel_too_low())
	{
	
	    if (controls.disengage_pressed)
	    {
		
		
		jsr(P_refuel_level_0A12);
		lda(0x041F);
		cmp_imm(0x80);
		beq(B_0E0B);
		
		lda_imm(0x23);
		sta(fuel_gauge_96ff);
		jsr(P_lose_a_life_6029);
		lda(0x041F);
		beq(B_0E0B);
  
		jmpl(P_game_over_4770);
  
		label(B_0E0B)
		lda_imm(0x00);
		sta(0x041F);
		jmp(J_0D25);
	    }
	}

	timer_tick(&tmr);
	
	jmp(J_0D59);
  
label(B_0E20)
	jsr(P_lose_a_life_6029);
	lda(0x041F);
	bne(B_0E30);
  
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	jmp(J_0D25);
  
label(B_0E30)
	jmpl(P_game_over_4770);
  
}
end_subroutine

subroutine(P_move_ship_0e33)
{

	lda(0x96F0);
	sec;
	sbc_imm(0x28);
	sta(0x00);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x01);
	ldy_imm(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_0E4F);
  
	bit(0x96B0);
	bne(B_0E62);
  
label(B_0E4F)
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_0E62);
  
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_0E6B);
  
	bit(0x96B1);
	beq(B_0E6B);
  
label(B_0E62)
	lda_imm(0xFF);
	sta(0x041F);
	jsr(P_ship_explodes_451C);
	rts;
  
	label(B_0E6B)
	// controls (level 1)
	if (controls.left_pressed)   {jmp(B_0EEC);}
	else if (controls.right_pressed)    {jmp(B_0E84);}

/*
lda(VIA_ PA);
	cmp_imm(0xDF); // left
	beq(B_0EEC);
  
	cmp_imm(0xBF); // right
	beq(B_0E84);
*/
	dec(0x660E);
	bne(B_0E83);
  
	lda_imm(0x0A);
	sta(0x660E);
	jmp(J_0EDB);
  
label(B_0E83)
	rts;
  
label(B_0E84)
	inc(0x96F2);
	lda(0x96F2);
	cmp_imm(0xCC);
	beq(B_0ECF);
  
	lsr(0x96B0);
	lda(0x96B1);
	lsr_A();
	ora_imm(0x20);
	sta(0x96B1);
	lda(0x96EF);
	clc;
	adc_imm(0x01);
	cmp_imm(0x06);
	sta(0x96EF);
	sta(0x00);
	bcc(J_0EDB);
  
	lda_imm(0x00);
	sta(0x96B1);
	sta(0x96EF);
	sta(0x00);
	lda(0x96F0);
	adc_imm(0x00);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	adc_imm(0x00);
	sta(0x96F1);
	sta(0x02);
	lda_imm(0x3F);
	sta(0x96B0);
	jmp(J_0F2F);
  
label(B_0ECF)
	lda_imm(0xCB);
	sta(0x96F2);
	rts;
  
label(B_0ED5)
	lda_imm(0x01);
	sta(0x96F2);
	rts;
  
label(J_0EDB)
	lda(0x96EF);
	sta(0x00);
	lda(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sta(0x02);
	bne(J_0F2F);
  
label(B_0EEC)
	dec(0x96F2);
	beq(B_0ED5);

	lda(0x96B0);
	asl_A();
	ora_imm(0x01);
	sta(0x96B0);
	asl(0x96B1);
	lda(0x96EF);
	sec;
	sbc_imm(0x01);
	sta(0x96EF);
	sta(0x00);
	bpl(J_0EDB);
  
	lda_imm(0x01);
	sta(0x96B0);
	lda_imm(0x3F);
	sta(0x96B1);
	lda_imm(0x05);
	sta(0x96EF);
	sta(0x00);
	lda(0x96F0);
	sbc_imm(0x00);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x96F1);
	sta(0x02);
label(J_0F2F)
	lda_imm(0x4A);
	sta(0x03);
	lda_imm(0x81);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	// move: draw the ship
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_decrease_fuel_0F43)
{

	lda(fuel_gauge_96ff);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	dec(fuel_gauge_96ff);
	bne(B_0F68);

	// no more fuel
	jsr(P_ship_explodes_451C);
	lda_imm(0x01);
	sta(0x041F);
label(B_0F68)
	rts;
  
}
end_subroutine

subroutine(P_0F69)
{

	lda(0x96F3);
	beq(B_0F71);
  
	jmp(J_0FE8);
  
	label(B_0F71)
// player fire test

	if (controls.fire_pressed) jmp(B_0F80);
/*
	lda(VIA_ PB);
	cmp_imm(0xBC);
	bne(B_0F7F);
  
	lda(VIA_ PA);
	cmp_imm(0xFE);
	beq(B_0F80);
*/
	rts;
  
label(B_0F80)

sound_play("shoot");

	ldx_imm(0x01);
	stx(0x00);
	lda(0x96EF);
	clc;
	adc_imm(0x04);
	cmp_imm(0x06);
	bcc(B_0F95);
  
	sec;
	sbc_imm(0x06);
	ldx_imm(0x02);
	stx(0x00);
label(B_0F95)
	lsr_A();
	sta(0x96F4);
	tax;
	lda_imm(0x30);
	cpx_imm(0x00);
	beq(B_0FA5);
  
label(B_0FA0)
	lsr_A();
	lsr_A();
	dex;
	bne(B_0FA0);
  
label(B_0FA5)
	sta(0x96F7);
	lda(0x96F0);
	clc;
	adc(0x00);
	sta(0x96F5);
	lda(0x96F1);
	adc_imm(0x00);
	sta(0x96F6);
	lda_imm(0x01);
	sta(0x96F3);
	lda(0x96F5);
	sec;
	sbc_imm(0xF0);
	sta(0x96F5);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x96F6);
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	and_imm(0xF7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x00);
	sta(0x6900);
	lda_imm(0x01);
	sta(0x6901);
	rts;
  
label(J_0FE8)
	lda(0x96F5);
	sec;
	sbc_imm(0x50);
	sta(0x96F5);
	sta(0x01);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x96F6);
	sta(0x02);
	cmp_imm(0xA1);
	bcc(B_101B);
  
	ldy_imm(0x00);
	lda_ind_Y(0x01);
	cmp_imm(0x40);
	bcc(B_103F);
  
	cmp_imm(0x80);
	bcs(B_103F);
  
	bit(0x96F7);
	beq(B_103F);
  
	jsr(P_4AFE);
	jsr(P_10B6);
	jsr(P_5F5B);
label(B_101B)
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x08);
	sta(0x06);
	lda(0x96F5);
	clc;
	adc_imm(0x50);
	sta(0x01);
	lda(0x96F6);
	adc_imm(0x00);
	sta(0x02);
	jsr(P_fill_zone_51BB);
	lda_imm(0x00);
	sta(0x96F3);
	rts;
  
label(B_103F)
	lda(0x96F4);
	sta(0x00);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	lda_imm(0x2A);
	sta(0x03);
	lda_imm(0x83);
	sta(0x04);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_1058)
{

	lda(0x9601);
	sta(0x02);
	lda(0x9602);
	sta(0x03);
	lda_imm(0x40);
	sta(0x00);
	lda_imm(0xA1);
	sta(0x01);
	ldy_imm(0x00);
	ldx_imm(0x14);
label(B_106E)
	lda_ind_Y(0x02);
	sta_ind_Y(0x00);
	lda(0x02);
	clc;
	adc_imm(0x05);
	sta(0x02);
	lda(0x03);
	adc_imm(0x00);
	sta(0x03);
	dex;
	bne(B_108E);
  
	lda(0x9601);
	sta(0x02);
	lda(0x9602);
	sta(0x03);
	ldx_imm(0x14);
label(B_108E)
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	cmp_imm(0xBC);
	bcc(B_106E);
  
	rts;
  
}
end_subroutine


subroutine(P_10B6)
{

	lda(0x96F5);
	sta(0x00);
	lda(0x96F6);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_11A2);
	lda(0x03);
	sta(0x05);
	lda(0x04);
	sta(0x06);
	ldx_imm(0x27);
label(B_10D4)
	lda_X(0x9000);
	bne(B_10E1);
  
	dex;
	dex;
	dex;
label(B_10DC)
	dex;
	dex;
	bpl(B_10D4);
  
	rts;
  
label(B_10E1)
	dex;
	dex;
	lda_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_11A2);
	lda(0x05);
	cmp(0x03);
	bcc(B_10DC);
  
	sec;
	sbc_imm(0x05);
	cmp(0x03);
	bcs(B_10DC);
  
	lda(0x06);
	cmp(0x04);
	bcc(B_10DC);
  
	sec;
	sbc_imm(0x17);
	cmp(0x04);
	bcs(B_10DC);
  
	inx;
	inx;
	inx;
	lda(stage_9600);
	cmp_imm(0x02);
	beq(B_111F);
  
	dec_X(0x9000);
label(B_111F)
	dec_X(0x9000);
	ldy_X(0x9000);
	lda_imm(0x00);
	sta(0x961A);
	sta(0x961B);
	sta(0x961C);
	sta(0x961E);
	lda_imm(0x01);
	sta(0x961D);
	cpy_imm(0x00);
	bne(B_1141);
  
	lda_imm(0x05);
	sta(0x961E);
label(B_1141)
	jsr(P_increase_score_641B);
	jsr(P_4B55);
	cpy_imm(0x00);
	beq(B_1153);
  
	inc(nb_kamikaze_birds_96fe);
	lda_imm(0x01);
	sta_X(0x9014);
label(B_1153)
	dex;
	cpy_imm(0x00);
	beq(B_115E);

	// split in 2 birds, one dashing to the ground
	falling_sound_id[r.x] = sound_play("falling");
	
	lda_X(0x9000);
	sta_X(0x9014);
label(B_115E)
	dex;
	lda_X(0x9000);
	sta(0x01);
	cpy_imm(0x00);
	beq(B_116F);
  
	clc;
	adc_imm(0x02);
	sta_X(0x9014);
	php;
label(B_116F)
	cpy_imm(0x00);
	bne(B_117F);
  
	inc(0x0406);
	lda(0x0407);
	clc;
	adc_imm(0x04);
	sta(0x0407);
label(B_117F)
	dex;
	lda_imm(0x40);
	sta(0x03);
	lda_X(0x9000);
	sta(0x02);
	cpy_imm(0x00);
	beq(B_1193);
  
	plp;
	adc_imm(0x00);
	sta_X(0x9014);
label(B_1193)
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x6E);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	one_foe_killed(1);
	rts;
  
}
end_subroutine

subroutine(P_11A2)
{

	ldy_imm(0x08);
	sec;
	lda(0x01);
	sbc(0x02);
label(B_11A9)
	php;
	rol(0x04);
	asl(0x00);
	rol_A();
	plp;
	bcc(B_11B7);
  
	sbc(0x02);
	jmp(J_11B9);
  
label(B_11B7)
	adc(0x02);
label(J_11B9)
	dey;
	bne(B_11A9);
  
	bcs(B_11C1);
  
	adc(0x02);
	clc;
label(B_11C1)
	rol(0x04);
	sta(0x03);
	rts;
  
}
end_subroutine

subroutine(P_animate_enemy_shots)
{

	lda(0x96C0);
	bne(B_11CC);
  
	rts;
  
label(B_11CC)
	lda(0x96C2);
	clc;
	adc_imm(0x50);
	sta(0x96C2);
	sta(0x01);
	lda(0x96C3);
	adc_imm(0x00);
	sta(0x96C3);
	sta(0x02);
	cmp_imm(0xBE);
	beq(B_11FE);
  
	lda(0x96C1);
	sta(0x00);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	lda_imm(0x8A);
	sta(0x04);
	lda_imm(0x2A);
  sta(0x03);
  // animate enemy shots
	jsr(P_screen_copy_5172);
	rts;
  
label(B_11FE)
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	lda(0x96C2);
	sec;
	sbc_imm(0x50);
	sta(0x01);
	lda(0x96C3);
	sbc_imm(0x00);
	sta(0x02);
	jsr(P_fill_zone_51BB);
	lda_imm(0x00);
	sta(0x96C0);
	rts;
  
}
end_subroutine

subroutine(P_1222)
{

	ldx_imm(0x08);
label(B_1224)
	lda_X(0x9000);
	beq(B_122F);
  
	dex;
	dex;
	dex;
	bpl(B_1224);
  
	rts;
  
label(B_122F)
	lda_imm(0x01);
	sta_X(0x9000);
	dex;
	lda_imm(0x00);
	sta_X(0x9000);
	dex;
	lda_imm(0x11);
	sta_X(0x9000);
	rts;
  
}
end_subroutine

subroutine(P_1241)
{

	ldx_imm(0x08);
label(B_1243)
	lda_X(0x9009);
	beq(B_124E);
  
	dex;
	dex;
	dex;
	bpl(B_1243);
  
	rts;
  
label(B_124E)
	lda_imm(0x01);
	sta_X(0x9009);
	dex;
	lda_imm(0x00);
	sta_X(0x9009);
	dex;
	lda_imm(0x4D);
	sta_X(0x9009);
	rts;
  
}
end_subroutine

subroutine(P_1260)
{

	ldx_imm(0x08);
label(B_1262)
	lda_X(0x9009);
	bne(B_126D);
  
	dex;
	dex;
	dex;
	bpl(B_1262);
  
	rts;
  
label(B_126D)
	dex;
	dec_X(0x9009);
	lda_X(0x9009);
	sta(0x00);
	bpl(B_12AB);
  
	lda_imm(0x02);
	sta_X(0x9009);
	sta(0x00);
	dex;
	dec_X(0x9009);
	lda_X(0x9009);
	sta(0x01);
	cmp_imm(0x29);
	bcc(B_12B4);
  
label(J_128C)
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x48);
	sta(0x03);
	lda_imm(0x8A);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(B_1262);
  
	rts;
  
label(B_12AB)
	dex;
	lda_X(0x9009);
	sta(0x01);
	jmp(J_128C);
  
label(B_12B4)
	inx;
	inx;
	lda_imm(0x00);
	sta_X(0x9009);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x29);
	sta(0x01);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	txa;
	pha;
	jsr(P_fill_zone_51BB);
	pla;
	tax;
	dex;
	dex;
	dex;
	bpl(B_1262);
  
	rts;
  
}
end_subroutine

subroutine(P_12DC)
{

	ldx_imm(0x08);
label(B_12DE)
	lda_X(0x9000);
	bne(B_12E9);
  
	dex;
	dex;
	dex;
	bpl(B_12DE);
  
	rts;
  
label(B_12E9)
	dex;
	inc_X(0x9000);
	lda_X(0x9000);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_1329);
  
	lda_imm(0x00);
	sta_X(0x9000);
	sta(0x00);
	dex;
	inc_X(0x9000);
	lda_X(0x9000);
	sta(0x01);
	cmp_imm(0x35);
	bcs(B_1332);
  
label(J_130A)
	lda_imm(0xAE);
	sta(0x02);
	lda_imm(0x48);
	sta(0x03);
	lda_imm(0x8A);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(B_12DE);
  
	rts;
  
label(B_1329)
	dex;
	lda_X(0x9000);
	sta(0x01);
	jmp(J_130A);
  
label(B_1332)
	inx;
	inx;
	lda_imm(0x00);
	sta_X(0x9000);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0xAE);
	sta(0x02);
	lda_imm(0x35);
	sta(0x01);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	txa;
	pha;
	jsr(P_fill_zone_51BB);
	pla;
	tax;
	dex;
	dex;
	dex;
	bpl(B_12DE);
  
	rts;
  
}
end_subroutine

subroutine(P_135A)
{

	lda_imm(0x00);
	ldx_imm(0x00);
label(B_135E)
	sta_X(0x9000);
	dex;
	bne(B_135E);
  
	rts;
  
}
end_subroutine

subroutine(P_1365)
{

	ldx_imm(0x0F);
label(B_1367)
	lda_X(0x9000);
	beq(B_1371);
  
	dex;
	dex;
	dex;
	bpl(B_1367);
  
label(B_1371)
	lda_imm(0x01);
	sta_X(0x9000);
	dex;
	lda(VIA_T1CL);
	and_imm(0x1F);
	sta(0x01);
	lda(VIA_T1CL);
	eor(0x02);
	and_imm(0x07);
	clc;
	adc(0x01);
	clc;
	adc_imm(0x68);
	sta_X(0x9000);
	sta(0x01);
	dex;
	lda_imm(0xA1);
	adc_imm(0x00);
	sta_X(0x9000);
	one_foe_killed(0);
	rts;
  
}
end_subroutine

subroutine(P_139C)
{

	lda(VIA_T1CL);
	cmp_imm(0x0A);
	bcs(B_13A6);
  
	jsr(P_1365);
label(B_13A6)
	ldx_imm(0x0F);
label(J_13A8)
	lda_X(0x9000);
	bne(B_13B9);
  
	dex;
	dex;
	dex;
	ldy_imm(0xAF);
label(B_13B2)
	dey;
	bne(B_13B2);
  
	txa;
	bpl(J_13A8);
  
	rts;
  
label(B_13B9)
	ldy_imm(0x01);
	sty(0x00);
	clc;
	adc_imm(0x01);
	sta_X(0x9000);
	cmp_imm(0x03);
	bcc(B_13D0);
  
	lda_imm(0x01);
	sta_X(0x9000);
	lda_imm(0x00);
	sta(0x00);
label(B_13D0)
	dex;
	lda_X(0x9000);
	clc;
	adc_imm(0x50);
	sta_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	adc_imm(0x00);
	cmp_imm(0xBB);
	sta(0x02);
	sta_X(0x9000);
	bcs(B_1406);
  
	lda_imm(0x4A);
	sta(0x03);
	lda_imm(0x71);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x3C);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(J_13A8);
  
	rts;
  
label(B_1406)
	inx;
	inx;
	lda_imm(0x00);
	sta_X(0x9000);
	dex;
	lda(0x01);
	sec;
	sbc_imm(0x50);
	sta(0x01);
	dex;
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x3C);
	sta(0x06);
	txa;
	pha;
	jsr(P_fill_zone_51BB);
	pla;
	tax;
	dex;
	bmi(B_1435);
  
	jmp(J_13A8);
  
label(B_1435)
	rts;
  
}
end_subroutine

subroutine(P_1436)
{

	lda_imm(0x00);
	ldx_imm(0x00);
label(B_143A)
	sta_X(0x9000);
	dex;
	bne(B_143A);
  
	lda_imm(0x01);
	sta(0x0400);
	sta(0x0401);
	sta(0x900B);
	lda_imm(0x04);
	sta(0xBCC1);
	sta(0xBCE9);
	sta(0xBD39);
	sta(0xBDB1);
	sta(0xBDD9);
	sta(0xBE01);
	sta(0xBE51);
	sta(0xBE79);
	sta(0xBEA1);
	sta(0xBEC9);
	lda_imm(0x06);
	sta(0xBD11);
	sta(0xBD89);
	lda_imm(0x05);
	sta(0xBD61);
	sta(0xBE29);
	lda_imm(0x98);
	sta(0x01);
	lda_imm(0xBC);
	sta(0x02);
	ldy_imm(0x00);
label(B_1485)
	lda_imm(0x13);
	sta_ind_Y(0x01);
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	php;
	cmp_imm(0x18);
	beq(B_149E);
  
	plp;
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	bne(B_1485);
  
label(B_149E)
	plp;
	rts;
  
}
end_subroutine

subroutine(P_drop_paratroop_14A0)
{

	ldx_imm(0x0A);
label(B_14A2)
	lda_X(0x9000);
	beq(B_14AF);
  
	dex;
	dex;
	dex;
	dex;
	dex;
	bpl(B_14A2);
  
	rts;
  
label(B_14AF)
	lda(VIA_T1CL);
	and_imm(0x01);
	clc;
	adc_imm(0x01);
	sta_X(0x9000);
	dex;
	lda(0x9604);
	sec;
	sbc_imm(0x05);
	sta_X(0x9000);
	dex;
	lda(0x9605);
	sta_X(0x9000);
	dex;
	lda(0x9606);
	clc;
	adc_imm(0x90);
	sta_X(0x9000);
	dex;
	lda(0x9607);
	adc_imm(0x01);
	sta_X(0x9000);
	rts;
  
}
end_subroutine

subroutine(P_14DF)
{

	dec(0x9603);
	bne(B_14EC);
  
	lda_imm(0x14);
	sta(0x9603);
	jsr(P_1760);
label(B_14EC)
	ldx_imm(0x0A);
label(J_14EE)
	lda_X(0x9000);
	bne(B_14FB);
  
	dex;
	dex;
	dex;
	dex;
	dex;
	bpl(J_14EE);
  
	rts;
  
label(B_14FB)
	cmp_imm(0x02);
	bne(B_1502);
  
	jmp(J_157B);
  
label(B_1502)
	dex;
	dec_X(0x9000);
	bpl(B_1514);
  
	inx;
	lda_imm(0x02);
	sta_X(0x9000);
	dex;
	lda_imm(0x00);
	sta_X(0x9000);
label(B_1514)
	dex;
	dec_X(0x9000);
	lda_X(0x9000);
	sta(0x00);
	bpl(B_1561);
  
	lda_imm(0x02);
	sta_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	clc;
	adc_imm(0x4F);
	sta_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	adc_imm(0x00);
	sta_X(0x9000);
	sta(0x02);
label(J_153D)
	lda(0x02);
	cmp_imm(0xBA);
	bcc(B_1546);
  
	jmp(J_15BB);
  
label(B_1546)
	lda_imm(0xC2);
	sta(0x03);
	lda_imm(0x71);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(J_14EE);
  
	rts;
  
label(B_1561)
	dex;
	lda_X(0x9000);
	clc;
	adc_imm(0x50);
	sta_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	adc_imm(0x00);
	sta_X(0x9000);
	sta(0x02);
	jmp(J_153D);
  
label(J_157B)
	dex;
	inc_X(0x9000);
	lda_X(0x9000);
	cmp_imm(0x50);
	bcc(B_158D);
  
	inx;
	lda_imm(0x01);
	sta_X(0x9000);
	dex;
label(B_158D)
	dex;
	inc_X(0x9000);
	lda_X(0x9000);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_1561);
  
	lda_imm(0x00);
	sta_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	clc;
	adc_imm(0x51);
	sta_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	adc_imm(0x00);
	sta_X(0x9000);
	sta(0x02);
	jmp(J_153D);
  
label(J_15BB)
	txa;
	pha;
	jsr(P_52D4);
	lda(0x01);
	sec;
	sbc_imm(0x51);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x73);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pla;
	tax;
	inx;
	inx;
	inx;
	inx;
	lda_imm(0x00);
	sta_X(0x9000);
	dex;
	dex;
	dex;
	dex;
	dex;
	bmi(B_15F1);
  
	jmp(J_14EE);
  
label(B_15F1)
	rts;
  
}
end_subroutine

subroutine(P_15F2)
{

	lda_imm(0x00);
	sta(0x96B1);
	sta(0x96C0);
	sta(0x96F3);
	sta(0x00);
	sta(0x96EF);
	lda_imm(0xD0);
	sta(0x01);
	sta(0x96F0);
	lda_imm(0xB9);
	sta(0x02);
	sta(0x96F1);
	lda_imm(0xD2);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x48);
	sta(0x06);
	lda_imm(0x2A);
	sta(0x96F2);
	jsr(P_screen_copy_5172);
	lda_imm(0x3F);
	sta(0x96B0);
	rts;
  
}
end_subroutine

subroutine(P_162E)
{

	lda(0x96F0);
	sec;
	sbc_imm(0x28);
	sta(0x00);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x01);
	ldy_imm(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_164A);
  
	bit(0x96B0);
	bne(B_165D);
  
label(B_164A)
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_165D);
  
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_1666);
  
	bit(0x96B1);
	beq(B_1666);
  
label(B_165D)
	jsr(P_6333);
	lda_imm(0xFF);
	sta(0x041F);
	rts;
  
label(B_1666)
	lda(0x00);
	clc;
	adc_imm(0xF8);
	sta(0x00);
	lda(0x01);
	adc_imm(0x02);
	sta(0x01);
	ldy_imm(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_1680);
  
	bit(0x96B0);
	bne(B_165D);
  
label(B_1680)
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_165D);
  
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_1693);
  
	bit(0x96B1);
	bne(B_165D);
  
label(B_1693)

	if (controls.left_pressed)   {jmp(B_1709);}
	else if (controls.right_pressed)    {jmp(B_16A1);}
/*lda(VIA_PA);
	cmp_imm(0xDF);
	beq(B_1709);
  
	cmp_imm(0xBF);
	beq(B_16A1);*/
  
	jmp(J_16F8);
  
label(B_16A1)
	inc(0x96F2);
	lda(0x96F2);
	cmp_imm(0xCC);
	beq(B_16EC);
  
	lsr(0x96B0);
	lda(0x96B1);
	lsr_A();
	ora_imm(0x20);
	sta(0x96B1);
	lda(0x96EF);
	clc;
	adc_imm(0x01);
	cmp_imm(0x06);
	sta(0x96EF);
	sta(0x00);
	bcc(J_16F8);
  
	lda_imm(0x00);
	sta(0x96B1);
	sta(0x96EF);
	sta(0x00);
	lda(0x96F0);
	adc_imm(0x00);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	adc_imm(0x00);
	sta(0x96F1);
	sta(0x02);
	lda_imm(0x3F);
	sta(0x96B0);
	jmp(J_174C);
  
label(B_16EC)
	lda_imm(0xCB);
	sta(0x96F2);
	rts;
  
label(B_16F2)
	lda_imm(0x01);
	sta(0x96F2);
	rts;
  
label(J_16F8)
	lda(0x96EF);
	sta(0x00);
	lda(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sta(0x02);
	bne(J_174C);
  
label(B_1709)
	dec(0x96F2);
	beq(B_16F2);
  
	lda(0x96B0);
	asl_A();
	ora_imm(0x01);
	sta(0x96B0);
	asl(0x96B1);
	lda(0x96EF);
	sec;
	sbc_imm(0x01);
	sta(0x96EF);
	sta(0x00);
	bpl(J_16F8);
  
	lda_imm(0x01);
	sta(0x96B0);
	lda_imm(0x3F);
	sta(0x96B1);
	lda_imm(0x05);
	sta(0x96EF);
	sta(0x00);
	lda(0x96F0);
	sbc_imm(0x00);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x96F1);
	sta(0x02);
label(J_174C)
	lda_imm(0xD2);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x48);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_1760)
{

	ldx_imm(0x0A);
label(B_1762)
	lda_X(0x9000);
	bne(B_176F);
  
label(J_1767)
	dex;
	dex;
	dex;
	dex;
	dex;
	bpl(B_1762);
  
	rts;
  
label(B_176F)
	lda(VIA_T1CL);
	adc(0x01);
	and_imm(0x01);
	clc;
	adc_imm(0x01);
	sta_X(0x9000);
	inc(0x01);
	jmp(J_1767);
  
}
end_subroutine

subroutine(P_1781)
{

	lda_imm(0x64);
	sta(0x040D);
	lda_imm(0xFF);
	sta(0x6600);
	eor_imm(0xFF);
	sta(0x6602);
	lda_imm(0x02);
	sta(0x6601);
	lda_imm(0x80);
	sta(0x6603);
	clc;
	adc_imm(0x7F);
	eor_imm(0xFF);
	sta(0x6604);
	lda_imm(0x64);
	sta(0x6605);
	lda_imm(0x0A);
	sta(0x6606);
	lda_imm(0x0A);
	sta(0x6607);
	lda_imm(0x50);
	sta(0x6608);
	lda_imm(0x03);
	sta(0x6609);
	lda_imm(0x14);
	sta(0x660A);
	lda_imm(0x02);
	sta(0x660B);
	lda_imm(0x1E);
	sta(0x660C);
	lda_imm(0x14);
	sta(0x660D);
	lda_imm(0x09);
	sec;
	sbc(skill_level_9619);
	sta(0x00);
	beq(B_17E5);
  
	tax;
label(B_17DA)
	lda(0x6600);
	sbc_imm(0x1C);
	sta(0x6600);
	dex;
	bne(B_17DA);
  
label(B_17E5)
	ldx(0x00);
	beq(B_17FD);
  
label(B_17E9)
	lda(0x6601);
	clc;
	adc_imm(0x03);
	sta(0x6601);
	dex;
	bne(B_17E9);
  
	eor_imm(0xFF);
	sec;
	sbc_imm(0x28);
	sta(0x6602);
label(B_17FD)
	ldx(0x00);
	beq(B_1812);
  
label(B_1801)
	lda(0x6603);
	sec;
	sbc_imm(0x0E);
	sta(0x6603);
	dex;
	bne(B_1801);
  
	eor(0xFF);
	sta(0x6604);
label(B_1812)
	ldx(0x00);
	beq(B_1822);
  
label(B_1816)
	lda(0x6605);
	clc;
	adc_imm(0x0B);
	sta(0x6605);
	dex;
	bne(B_1816);
  
label(B_1822)
	ldx(0x00);
	beq(B_1832);
  
label(B_1826)
	lda(0x6607);
	clc;
	adc_imm(0x03);
	sta(0x6607);
	dex;
	bne(B_1826);
  
label(B_1832)
	ldx(0x00);
	beq(B_1842);
  
label(B_1836)
	lda(0x6608);
	sec;
	sbc_imm(0x08);
	sta(0x6608);
	dex;
	bne(B_1836);
  
label(B_1842)
	lda(skill_level_9619);
	bne(B_184C);
  
	lda_imm(0x02);
	sta(0x6609);
label(B_184C)
	ldx(0x00);
	beq(B_185C);
  
label(B_1850)
	lda(0x660A);
	sec;
	sbc_imm(0x01);
	sta(0x660A);
	dex;
	bne(B_1850);
  
label(B_185C)
	lda(skill_level_9619);
	cmp_imm(0x04);
	bcs(B_1868);
  
	lda_imm(0x01);
	sta(0x660B);
label(B_1868)
	ldx(0x00);
	beq(B_1878);
  
label(B_186C)
	lda(0x660C);
	sec;
	sbc_imm(0x02);
	sta(0x660C);
	dex;
	bne(B_186C);
  
label(B_1878)
	ldx(0x00);
	beq(B_1888);
  
label(B_187C)
	lda(0x660D);
	sec;
	sbc_imm(0x01);
	sta(0x660D);
	dex;
	bne(B_187C);
  
label(B_1888)
	rts;
  
}
end_subroutine

subroutine(P_1889)
{

	lda_imm(0x00);
	sta(0x96B1);
	sta(0x96C0);
	sta(0x96F3);
	sta(0x00);
	sta(0x96EF);
	sta(0x96F0);
	lda_imm(0xBC);
	sta(0x96F1);
	lda_imm(0x0A);
	sec;
	sbc(skill_level_9619);
	tax;
label(B_18A8)
	lda(0x96F0);
	sec;
	sbc_imm(0x90);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sbc_imm(0x01);
	sta(0x96F1);
	sta(0x02);
	dex;
	bne(B_18A8);
  
	lda_imm(0x4A);
	sta(0x03);
	lda_imm(0x81);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	lda_imm(0x2A);
	sta(0x96F2);
	jsr(P_screen_copy_5172);
	lda_imm(0x3F);
	sta(0x96B0);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xF7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x00);
	sta(0x04);
	jsr(P_sound_routine_4A8B);
	rts;
  
 
}
end_subroutine

/*
subroutine(P_2000)
{

	lda_imm(0x96);
	sta(0x09);
	lda_imm(0x23);
	sta(0x0A);
	lda_imm(0x07);
	sta(0x08);
	jsr(P_display_text_468A);
	lda_imm(0x00);
	sta(0x6924);
	lda(0x6922);
	sta(0x6920);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFE);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0xE8);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	jsr(P_sound_routine_4A8B);
	lda_imm(0x00);
	sta(0x041F);
	lda(0x6920);
	clc;
	adc_imm(0x01);
	jsr(P_draw_red_bar_20EF);
	ldx_imm(0x06);
	lda_imm(0x17);
	sta(0x09);
label(B_2048)
	lda_imm(0x00);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x2367);
	jsr(P_plot_472A);
	dec(0x09);
	dex;
	bpl(B_2048);
  
	ldx_imm(0x27);
	stx(0x09);
label(B_205F)
	lda_imm(0x0C);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x236E);
	jsr(P_plot_472A);
	dec(0x09);
	dex;
	bpl(B_205F);
  
	jsr(P_214A);
label(B_2075)
	ldx_imm(0x00);
	ldy_imm(0x14);
label(B_2079)
	dex;
	bne(B_2079);
  
	dey;
	bne(B_2079);
  
	jsr(P_adjust_xxx_level_20A9);
	lda(0x041F);
	beq(B_2075);
  
	lda(0x6920);
	sta(0x6922);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
  
 
}
end_subroutine
*/
subroutine(P_209C)
{

	lda(0x05FD);
	sta(0x01);
	lda_imm(0x08);
	sta(0x00);
	jsr(P_sound_routine_4A57);
	rts;
  
}
end_subroutine

subroutine(P_adjust_xxx_level_20A9)
{
    int done = 0;

    OricTimer tmr = timer_init(1,30);
    while(!done)
    {
	timer_tick(&tmr);
	update_controls();

	if (controls.right_pressed)
	{
	    lda(0x6920);
	    cmp_imm(0x27);
		    
	    bcc(B_20D7);
	    rts;
		    
	    label(B_20D7)
	    inc(0x6920);
		    
	    jsr(P_draw_red_bar_20EF);
	    done = 1;
	}
	else if (controls.left_pressed)
	{
       
	    lda(0x6920);
	    cmp_imm(0x02);
	    bcs(B_20EC);
	    rts;
	    
	    label(B_20EC)
	    dec(0x6920);
	    jsr(P_draw_red_bar_20EF);
	    done = 1;
	}
	if (controls.fire_pressed)
	{
	    lda_imm(0xFF);
	    sta(0x041F);
	    done = 1;
	    flush_events();
	}
    }
		
}
end_subroutine

// < A

subroutine(P_draw_red_bar_20EF)
{    
    sta(0x6921);
    ldx_imm(0x06);   // min Y
    label(B_20F4)
    lda(0x6920);
    sta(0x00);
    stx(0x01);
    lda_imm(0x10);     // black foreground
    jsr(P_plot_472A);     // print char
    inx;
    cpx_imm(0x0B);   // max Y
    bne(B_20F4);
  
    ldx_imm(0x06);
    label(B_2107)
    lda(0x6921);
    sta(0x00);
    stx(0x01);
    lda_imm(0x20);     // space
    jsr(P_plot_472A);
    inx;
    cpx_imm(0x0B);
    bne(B_2107);
  
    lda(0x6924);
    beq(B_2132);
  
    lda(0x6920);
    lsr_A();
    lsr_A();
    sta(0x05FF);
    lda_imm(0x09);
    sec;
    sbc(0x05FF);
    sta(0x05FF);
    jsr(P_221F);
    rts;
  
    label(B_2132)
    lda(0x6920);
    lsr_A();
    cmp_imm(0x10);
    bcc(B_2143);
  
    lda_imm(0x0F);
    sta(0x05FD);
    jsr(P_209C);
    rts;
  
    label(B_2143)
    sta(0x05FD);
    jsr(P_209C);
    rts;
  
}
end_subroutine

subroutine(P_214A)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x05);
	sta(0x01);
	lda_imm(0x12);
	jsr(P_plot_472A);
	ldx_imm(0x06);
label(B_2159)
	lda_imm(0x00);
	sta(0x00);
	stx(0x01);
	lda_imm(0x11);
	jsr(P_plot_472A);
	inx;
	cpx_imm(0x0B);
	bne(B_2159);
  
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x0B);
	sta(0x01);
	lda_imm(0x12);
	jsr(P_plot_472A);
	rts;
  

  
}
end_subroutine

subroutine(P_221F)
{
    // print digit? (volume/skill)
	lda_imm(0x04);
	sta(0x01);
	lda_imm(0x16);
	sta(0x00);
	lda(0x05FF);
	ora_imm(0x30);
	jsr(P_plot_472A);
	rts;
  
}
end_subroutine

subroutine(P_show_mothership_intro_2230)
{
    SDL_Event event;
    
	jsr(P_draw_mothership_5CE2);

	lda_imm(0x09);
	sta(0x05FF);
	lda_imm(0x30);
	sta(0x09);
	lda_imm(0x00);
	sta(0x08);

	/* title rising from the bottom of the screen */
	int i;

	OricTimer tmr = timer_init(1,30);

	int sound_id = sound_play("title");
	
	for (i = 0; i < 0x3E; i++)
	{
	    jsr(P_intro_screen_22F8);
	    jsr(P_2323);
	
	    timer_tick(&tmr);

	    if (SDL_PollEvent(&event))
	    {
		switch(event.type)
		{
		  case SDL_QUIT:
		    quit();
		    break;
		  case SDL_JOYBUTTONUP:
		    goto out;
		    
		  case SDL_KEYDOWN:	      
		    flush_events();
		    switch (event.key.keysym.sym)
		    {
		      case SDLK_RCTRL:
		      case SDLK_LCTRL:
			goto out;
		      break;
		    default:
		      break;
		    }
		}
	    }
	}
  
	SDL_Delay(100);

	sound_stop(sound_id);

	sound_id = sound_play("mothership_laser");
	
	// laser
	
	lda_imm(0x23);
	sta(0x07);
	lda_imm(0xA9);
	sta(0x08);
	lda_imm(0x4B);
	sta(0x09);  /* loop 0x4C times */
	lda_imm(0x05);
	sta(0x05FD);
	jsr(P_4ABF);

	tmr = timer_init(1,20);

label(B_2278)
	lda_imm(0x47);
	ldy_imm(0x00);
	sta_ind_Y(0x07);
	lda(0x07);
	clc;
	adc_imm(0x28);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);

	
/* delay */
	timer_tick(&tmr);
	    if (SDL_PollEvent(&event))
	    {
		switch(event.type)
		{
		  case SDL_QUIT:
		    quit();
		    break;
		  case SDL_KEYDOWN:	      
		    flush_events();
		    switch (event.key.keysym.sym)
		    {
		      case SDLK_RCTRL:
		      case SDLK_LCTRL:
		  goto out;
		default:
		  break;
		    }
		}
	    }  
	dec(0x09);
	bne(B_2278);

	sound_stop(sound_id);

	sound_play("big_explosion");

	// destruction of the title

	/*
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xC7);
	jsr(P_sound_routine_4A57);*/
	lda_imm(0x0A);
	sta(0x04);
	timer_init(1,10);
label(B_22A6)
	lda_imm(0xF0);
	sta(0x00);
	lda_imm(0xB4);
	sta(0x01);
	lda_imm(0x00);
	sta(0x02);
	lda_imm(0xBF);
	sta(0x03);
	jsr(P_63D7);

	timer_tick(&tmr);

	if (peek(4) == 10)
	{
	    sound_play("metallic_crash");
	}
  
	dec(0x04);
	bne(B_22A6);
  
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_63D7);
	/*lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);*/
	lda(0x0450);
	cmp_imm(0x0C);
	beq(B_22EF);
	label(out);
	lda_imm(0x01);
	sta(jump_flag_05f0);

label(B_22EF)
	rts;
  
}
end_subroutine

subroutine(P_intro_screen_22F8)
{

	lda_imm(0x00);
	sta(0x00);
move16(0xBE80,1);
/*	lda_imm(0x80);
	sta(0x01);
	lda_imm(0xBE);
	sta(0x02);*/

	lda(0x08);
	sta(0x03);
	lda(0x09);
	sta(0x04);
	lda_imm(0x18);
	sta(0x05);
	sta(0x06);

/* display mothership */

	jsr(P_screen_copy_5172);

	lda(0x08);
	clc;
	adc_imm(0x18);
	sta(0x08);
	lda(0x09);
	adc_imm(0x00);
	sta(0x09);
	rts;
  
}
end_subroutine

subroutine(P_2323)
{

	lda_imm(0xF0);
	sta(0x00);
	lda_imm(0xB4);
	sta(0x01);
	lda_imm(0x18);
	sta(0x02);
	lda_imm(0xB5);
	sta(0x03);
	ldy_imm(0x00);
label(B_2335)
	lda_ind_Y(0x02);
	sta_ind_Y(0x00);
	iny;
	bne(B_2335);
  
	inc(0x01);
	inc(0x03);
	lda(0x03);
	cmp_imm(0xBF);
	bne(B_2335);
  
	rts;
  

 
}
end_subroutine

subroutine(P_display_bonus_4000)
{
	sound_play("level_end");
	ldx_imm(0x00);
	lda_imm(0x0F);
	sta(0x07);
label(J_4006)
	lda_imm(0x0F);
	sta(0x01);
	lda_imm(0x00);
	sta(0x05);
	lda(0x07);
	sta(0x00);
	txa;
	pha;
	lda_X(0x4A35);
	cmp_imm(0x0D);
	beq(B_4026);

	// display bonus
	
	jsr(P_write_hires_string_4050);
	pla;
	tax;
	inx;
	inc(0x07);
	jmp(J_4006);
  
label(B_4026)
	pla;
	lda_imm(0x01);
	sta(0x961C);
	lda_imm(0x00);
	sta(0x961A);
	sta(0x961B);
	sta(0x961D);
	sta(0x961E);
	jsr(P_increase_score_641B);

	SDL_Delay(2000);
  
	rts;
  
}
end_subroutine

subroutine(P_write_hires_string_4050)
{
	cmp_imm(0x0C);
	beq(B_40B3);
  
	cmp_imm(0x1F);
	bcs(B_405B);
  
	jmp(J_40D5);
  
label(B_405B)
	pha;
	lda(0x00);
	pha;
	lda(0x01);
	sta(0x00);
	lda_imm(0xF0);
	sta(0x01);
	jsr(P_0_times_1_4A41);
	pla;
	clc;
	adc(0x02);
	sta(0x01);
	lda(0x03);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0x00);
	sta(0x04);
	sta(0x00);
	pla;
	asl_A();
	rol(0x04);
	asl_A();
	rol(0x04);
	asl_A();
	rol(0x04);
	sta(0x03);
	lda(0x04);
	clc;
	adc_imm(0x98);
	sta(0x04);
	lda(0x02);
	clc;
	adc_imm(0xA0);
	sta(0x02);
	ldy_imm(0x00);
	ldx_imm(0x00);
label(B_409A)
	lda_ind_Y(0x03);
	ora_imm(0x40);
	sta_ind_X(0x01);
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	iny;
	cpy_imm(0x08);
	bne(B_409A);
  
	rts;
  
label(B_40B3)
	ldy_imm(0x00);
	sty(0x00);
	lda_imm(0xA0);
	sta(0x01);
label(B_40BB)
	lda_imm(0x40);
	sta_ind_Y(0x00);
	dey;
	bne(B_40BB);
  
	inc(0x01);
	lda(0x01);
	cmp_imm(0xBF);
	bne(B_40BB);
  
	ldx_imm(0x3F);
label(B_40CC)
	lda_imm(0x00);
	sta_X(0xBF00);
	dex;
	bpl(B_40CC);
  
	rts;
  
label(J_40D5)
	pha;
	lda(0x00);
	pha;
	lda(0x01);
	sta(0x00);
	lda_imm(0xF0);
	sta(0x01);
	jsr(P_0_times_1_4A41);
	pla;
	clc;
	adc(0x02);
	sta(0x01);
	lda(0x03);
	adc_imm(0xA0);
	sta(0x02);
	ldy_imm(0x00);
	ldx_imm(0x00);
	pla;
	sta(0x03);
label(B_40F7)
	lda(0x03);
	sta_ind_X(0x01);
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	iny;
	cpy_imm(0x08);
	bne(B_40F7);
  
	rts;
  
}
end_subroutine

subroutine(P_4124)
{

	lda_imm(0x00);
	ldx_imm(0x00);
label(B_4128)
	sta_X(0x9000);
	dex;
	bne(B_4128);
  
	lda_imm(0x01);
	sta(0x0408);
	sta(0x0409);
	sta(0x040A);
	sta(0x040B);
	sta(0x040C);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x61);
	sta(0x01);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x12);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x75);
	sta(0x01);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x12);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0x06);
	sta(0xA938);
	lda_imm(0x14);
	sta(0xAE10);
	sta(0xAE88);
	sta(0xAF78);
	sta(0xAF00);
	sta(0xB428);
	sta(0xB4A0);
	sta(0xB518);
	sta(0xB590);
	rts;
  
}
end_subroutine

subroutine(P_zorgon_bonus_418D)
{
    sound_play("level_end");
	ldx_imm(0x00);
	lda_imm(0x0F);
	sta(0x07);
label(J_4193)
	lda_imm(0x12);
	sta(0x01);
	lda_imm(0x00);
	sta(0x05);
	lda(0x07);
	sta(0x00);
	txa;
	pha;
	lda_X(0x4A35);  // BONUS 1000
	cmp_imm(0x0D);
	beq(B_41B3);
  
	jsr(P_write_hires_string_4050);
	pla;
	tax;
	inx;
	inc(0x07);
	jmp(J_4193);
  
label(B_41B3)
	pla;
	lda_imm(0x12);
	sta(0x01);
	lda_imm(0x16);
	sta(0x00);
	lda(0x9620);
	ora_imm(0x30);
	jsr(P_write_hires_string_4050);
	lda_imm(0x12);
	sta(0x01);
	lda_imm(0x17);
	sta(0x00);
	lda(0x9621);
	ora_imm(0x30);
	jsr(P_write_hires_string_4050);
	lda_imm(0x00);
	sta(0x961A);
	sta(0x961B);
	sta(0x961E);
	lda(0x9620);
	sta(0x961C);
	lda(0x9621);
	sta(0x961D);
	jsr(P_increase_score_641B);
	
	SDL_Delay(2000);
  
	rts;
  
}
end_subroutine

subroutine(P_4201)
{

	lda_imm(0x01);
	sta(0x9608);
	lda_imm(0x28);
	sta(0x9604);
	lda_imm(0x00);
	sta(0x9605);
	sta(0x00);
	lda_imm(0x51);
	sta(0x9606);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	sta(0x9607);
	lda_imm(0x82);
	sta(0x03);
	lda_imm(0x75);
	sta(0x04);
	lda_imm(0x06);
	sta(0x05);
	lda_imm(0x48);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

void P_refuel_4234(int *engine_sound_id)
{

	lda(0xA9C3);
	cmp_imm(0x41);
	bcs(B_4242);
  
	lda(0xA9C4);
	cmp_imm(0x41);
	bcc(B_4245);
  
label(B_4242)
	jsr(P_4BB9);
	lda_imm(0x00);
	sta(0x96E3);
	jsr(P_6037);
	
	sound_stop(*engine_sound_id);
	engine_sound_id = 0;

	
	jsr(P_do_refuel_44B7);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);

	poke_41F_80();
	rts;
  
label(B_4245)
	lda(0x96F0);
	sec;
	sbc_imm(0x28);
	sta(0x00);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x01);
	ldy_imm(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(J_426A);
  
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(J_426A);
  
	iny;
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_427B);
  
label(J_426A)
	lda_imm(0x01);
	sta(0x96E4);
	jsr(P_6090);
	sound_stop(*engine_sound_id);
	*engine_sound_id = 0;
	
	jsr(P_ship_explodes_451C);
	lda_imm(0xFF);
	sta(0x041F);
	rts;
  
label(B_427B)
	lda(0x00);
	clc;
	adc_imm(0x40);
	sta(0x00);
	lda(0x01);
	adc_imm(0x01);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(J_426A);
  
	ldy_imm(0x04);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(J_426A);
  
	lda(0x00);
	clc;
	adc_imm(0xB8);
	sta(0x00);
	lda(0x01);
	adc_imm(0x01);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(J_426A);
  
	ldy_imm(0x04);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(J_426A);

	update_controls();

	
	if (controls.fire_down)
	{
	    // engine on

	    if (!controls.fire_was_pressed)
	    {
		*engine_sound_id = sound_play("engine");
	    }
	    
	    jsr(P_4B9B);
	    lda_imm(0x01);
	    sta(0x96E3);
	    lda(0x96D6);
	    beq(B_42DD);
  
	    lda(0x96D1);
	    cmp_imm(0x0A);
	    beq(J_42EF);
  
	    dec(0x96D1);
	    jmp(J_42EF);
  
	    label(B_42DD)
	    inc(0x96D1);
	    bne(J_42EF);
  
	    lda_imm(0xD8);
	    sta(0x96D5);
	    lda_imm(0xFF);
	    sta(0x96D6);
	    dec(0x96D1);
	    label(J_42EF)
	    lda(0x96D1);
	    bne(B_42F5);
  
	    rts;
  
	    label(B_42F5)
	    dec(0x96D0);
	    beq(B_42FB);
  
	    rts;
  
	    label(B_42FB)
	    lda(0x96D1);
	    sta(0x96D0);
	    lda(0x96EF);
	    sta(0x00);
	    lda(0x96F0);
	    clc;
	    adc(0x96D5);
	    sta(0x96F0);
	    sta(0x01);
	    lda(0x96F1);
	    adc(0x96D6);
	    sta(0x96F1);
	    sta(0x02);
	    cmp_imm(0xBA);
	    bcc(B_4324);
  
	    lda_imm(0x01);
	    sta(0x96E4);
	    jsr(P_6090);
	    sound_stop(*engine_sound_id);
	    *engine_sound_id = 0;
	    jsr(P_ship_explodes_451C);
	    lda_imm(0xFF);
	    sta(0x041F);
	    rts;
    
	    label(B_4324)
	    lda_imm(0x4A);
	    sta(0x03);
	    lda_imm(0x81);
	    sta(0x04);
	    lda_imm(0x04);
	    sta(0x05);
	    lda_imm(0x50);
	    sta(0x06);
	    jsr(P_screen_copy_5172);
	}
	else
	{
	    sound_stop(*engine_sound_id);
	    *engine_sound_id = 0;
	    
	    lda_imm(0x00);
	    sta(0x00);
	    lda_imm(0x00);
	    sta(0x01);
	    jsr(P_4BB9);
	    ldy_imm(0x82);
	    label(B_4345)
	    dey;
	    bne(B_4345);
  
	    lda_imm(0x00);
	    sta(0x96E3);
	    dec(0x96D3);
	    bne(J_42EF);
  
	    lda(0x96D4);
	    sta(0x96D3);
	    lda(0x96D1);
	    beq(J_42EF);
  
	    lda(0x96D4);
	    sta(0x96D3);
	    lda(0x96D6);
	    beq(B_437A);
  
	    inc(0x96D1);
	    bne(J_42EF);
  
	    dec(0x96D1);
	    lda_imm(0x00);
	    sta(0x96D6);
	    lda_imm(0x28);
	    sta(0x96D5);
	    label(B_437A)
	    lda(0x96D1);
	    cmp_imm(0x0A);
	    beq(B_4384);
  
	    dec(0x96D1);
	    label(B_4384)
	    jmp(J_42EF);
	}
}


subroutine(P_4387)
{

	dec(0x96DA);
	bne(J_43A2);
  
	lda_imm(0x04);
	sta(0x96DA);

	// while refueling
	
	if (controls.left_pressed)   {jmp(J_444F);}
	else if (controls.right_pressed)    {jmp(J_4420);}

	/*
	lda(VIA_PA);
	cmp_imm(0xDF);
	bne(B_439B);
  
	jmp(J_444F);
  
label(B_439B)
	cmp_imm(0xBF);
	bne(J_43A2);
  
	jmp(J_4420);
	*/
label(J_43A2)
	
	lda(0x96D9);
	bne(B_43AE);
  
	lda(0x96D8);
	sta(0x96D9);
	rts;
  
label(B_43AE)
	dec(0x96D9);
	beq(B_43B4);
  
	rts;
  
label(B_43B4)
	lda(0x96D8);
	sta(0x96D9);
	lda(0x96F2);
	clc;
	adc(0x96D7);
	sta(0x96F2);
	beq(B_43DF);
  
	cmp_imm(0xCC);
	beq(B_43DF);
  
	lda(0x96EF);
	clc;
	adc(0x96D7);
	sta(0x96EF);
	sta(0x00);
	bmi(B_43E2);
  
	cmp_imm(0x06);
	bcs(B_4401);
  
	jmp(J_4492);
  
label(B_43DF)
	lda_imm(0x01);
	sta(0x96E4);
	jsr(P_6090);
	jsr(P_ship_explodes_451C);
	lda_imm(0xFF);
	sta(0x041F);
	rts;
 
label(B_43E2)
	lda_imm(0x05);
	sta(0x96EF);
	sta(0x00);
	lda(0x96F0);
	sec;
	sbc_imm(0x01);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x96F1);
	sta(0x02);
	jmp(J_447E);
  
label(B_4401)
	lda_imm(0x00);
	sta(0x96EF);
	sta(0x00);
	lda(0x96F0);
	clc;
	adc_imm(0x01);
	sta(0x96F0);
	sta(0x01);
	lda(0x96F1);
	adc_imm(0x00);
	sta(0x96F1);
	sta(0x02);
	jmp(J_447E);
  
label(J_4420)
	lda(0x96D7);
	cmp_imm(0x01);
	beq(B_443F);
  
	lda(0x96D8);
	beq(B_4432);
  
	inc(0x96D8);
	jmp(J_43A2);
  
label(B_4432)
	lda_imm(0x01);
	sta(0x96D7);
	lda_imm(0x00);
	sta(0x96D8);
	jmp(J_43A2);
  
label(B_443F)
	lda(0x96D8);
	cmp_imm(0x0A);
	bne(B_4449);
  
	jmp(J_43A2);
  
label(B_4449)
	dec(0x96D8);
	jmp(J_43A2);
  
label(J_444F)
	lda(0x96D7);
	cmp_imm(0x01);
	beq(B_4466);
  
	lda(0x96D8);
	cmp_imm(0x0A);
	bne(B_4460);
  
	jmp(J_43A2);
  
label(B_4460)
	dec(0x96D8);
	jmp(J_43A2);
  
label(B_4466)
	lda(0x96D8);
	beq(B_4471);
  
	inc(0x96D8);
	jmp(J_43A2);
  
label(B_4471)
	lda_imm(0xFF);
	sta(0x96D7);
	lda_imm(0x00);
	sta(0x96D8);
	jmp(J_43A2);
  
label(J_447E)
	lda_imm(0x4A);
	sta(0x03);
	lda_imm(0x81);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
label(J_4492)
	lda(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sta(0x02);
	bne(J_447E);
  
	jsr(P_4BB9);
	lda_imm(0x00);
	sta(0x96E3);
	jsr(P_6037);
	jsr(P_do_refuel_44B7);
	lda_imm(0x23);
	sta(fuel_gauge_96ff);
	// stage end
	poke_41F_80();
	rts;
  
}
end_subroutine

subroutine(P_do_refuel_44B7)
{

	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x00);
	sta(0x01);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x28);
	sta(0x05);
	lda_imm(0xF0);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0x0B);  // set low fuel (for better "animation"!)
	sta(fuel_gauge_96ff);
	jsr(P_init_fuel_display_45F6);
	lda_imm(0x0C);
	sta(0x07);

	int fuel_sound_id = sound_play("refuel");

label(B_44DA)
	lda(0x07);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	inc(0x07);
	ldy_imm(0x96);
	jsr(P_tempo_Y_44FF);
	lda(0x07);
	cmp_imm(0x23);
	bne(B_44DA);

	sound_stop(fuel_sound_id);
	rts;
  
}
end_subroutine

subroutine(P_tempo_Y_44FF)
{
    SDL_Delay(r.y * 2);
    r.y = 0;
}
end_subroutine

subroutine(P_ship_explodes_451C)
{
    stop_kamikaze_sounds();
	lda_imm(0x00);
	sta(0x07);
	lda(0x96EF);
	cmp_imm(0x03);
	bcs(B_4508);
  
	lda(0x96F0);
	sec;
	sbc_imm(0xC8);
	sta(0x96F0);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x96F1);
label(J_4538)
	lda(0x96F0);
	sec;
	sbc_imm(0xC7);
	sta(0x01);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);   // copy blank
	sta(0x03);
	lda_imm(0x07);   // on 7 columns
	sta(0x05);
	lda_imm(0xD2);   // on 210 lines!
	sta(0x06);
	jsr(P_fill_zone_51BB);


	int sidx = sound_play("big_explosion");

label(B_4556)
	jsr(P_4BCC);
	lda(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sta(0x02);
	lda(0x07);
	sta(0x00);
	lda_imm(0xEC);
	sta(0x03);
	lda_imm(0x8B);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x78);
	sta(0x06);
	jsr(P_screen_copy_5172);

	OricTimer tmr = timer_init(5,20);

	int i;
	

	for (i = 0; i < 80; i++)
	{
	    P_ship_explosion_45B7();
	    //if (i % 5 == 0) SDL_Delay(10);
	    timer_tick(&tmr);
	}
  
	inc(0x07);
	lda(0x07);
	cmp_imm(0x06);
	bne(B_4556);
  
	tmr = timer_init(5,20);
	for (i = 0; i < 90; i++)
	{
	    P_ship_explosion_45B7();
	    timer_tick(&tmr);
	}

	
	lda(0x96F0);
	sta(0x01);
	lda(0x96F1);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x78);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	sound_stop(sidx);
	rts;
	
label(B_4508)
	lda(0x96F0);
	sec;
	sbc_imm(0xC9);
	sta(0x96F0);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x96F1);
	jmp(J_4538);
	
}
end_subroutine

subroutine(P_ship_explosion_45B7)
{
// 0x96F0: ship position
    
	lda(0x96F0);
	sec;
	sbc_imm(0x01);
	sta(0x01);
	lda(0x96F1);
	sbc_imm(0x00);
	sta(0x02);
	tya;
	pha;
	ldx_imm(0x18);


label(B_45CA)
// pick a random color through red, white, yellow, magenta for the explosion
r.a = ((rand() * 4) / RAND_MAX) * 2 + 1;

/*lda(VIA_T1CL);
	and_imm(0x07);
	cmp_imm(0x00);
	beq(B_45CA);
  
	cmp_imm(0x02);
	beq(B_45CA);
  
	cmp_imm(0x04);
	beq(B_45CA);
  
	cmp_imm(0x06);
	beq(B_45CA);*/
  
	ldy_imm(0x00);
	sta_ind_Y(0x01);
	lda(0x01);
	clc;
	// next line
	
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	dex;
	bne(B_45CA);
	pla;
	tay;
	rts;
  
}
end_subroutine

subroutine(P_init_fuel_display_45F6)
{

	lda_imm(0x01);
	sta(0x0403);

	move16(0xA000,1);

	// red part
	
	lda_imm(0x11);
	sta(0x03);
	lda_imm(0x01);   // 1 column
	sta(0x05);
	lda_imm(0x06);   // 6 lines
	sta(0x06);

	jsr(P_fill_zone_51BB);

	// yellow part
	
	move16(0xA005,1);

	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	
	jsr(P_fill_zone_51BB);

	// green part
	
	move16(0xA00A,1);

	lda_imm(0x12);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);

	
	lda_imm(0x27);
	sta(0x04);
	
	label(B_4644)
	
	lda(0x04);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x10);  // black
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	dec(0x04);
	lda(0x04);
	cmp(fuel_gauge_96ff);
	bne(B_4644);

	
	ldx_imm(0x00);
	lda_imm(0x24);
	sta(0x07);
label(J_466A)
	lda_imm(0x00);
	sta(0x01);
	sta(0x05);
	lda(0x07);
	sta(0x00);
	txa;
	pha;
	lda_X(0x4D5D);  // "FUEL" string
	cmp_imm(0x0D);
	beq(B_4688);
  
	jsr(P_write_hires_string_4050);
	pla;
	tax;
	inx;
	inc(0x07);
	jmp(J_466A);
  
label(B_4688)
	pla;
	rts;
  
}
end_subroutine

// < 0x9.W: source text
// < 0x8.B: X start

subroutine(P_display_text_468A)
{
    lda_imm(0x0C);
    jsr(P_plot_472A);
    lda_imm(0x00);
    clr(0x07);     // line index
    lda(0x08);
    sta(0x06);
    label(J_4697)
    lda(0x06);
    sta(0x00);
    lda(0x07);
    sta(0x01);
    ldy_imm(0x00);
    lda_ind_Y(0x09);
    cmp_imm(0x16);
    beq(B_46C5);     // if 0x16 met then end of text
    
    cmp_imm(0x0D);   // if 0xD met then linefeed
    bne(B_46AE);
    
    jsr(P_next_line_46C6);
    
    label(B_46AE)
    jsr(P_plot_472A);
    inc(0x06);
    lda(0x06);
    cmp_imm(0x28);
    bne(B_46BC);    // if too much on the right, linefeed
    
    jsr(P_next_line_46C6);
    
    label(B_46BC)
    inc(0x09);
    bne(B_46C2);
    
    inc(0x0A);
    label(B_46C2)
    jmp(J_4697);
    
label(B_46C5)
rts;
  
}
end_subroutine

subroutine(P_next_line_46C6)
{

	lda(0x08);
	sta(0x06);
	inc(0x07);
	rts;
  

  
}
end_subroutine

static void adjust_skill_level()
{
    // adjust skill level
	

    move16(0x2396,0x9);
    lda_imm(0x07);
    sta(0x08);
    jsr(P_display_text_468A);
    lda_imm(0x01);
    sta(0x6924);
    ldx_imm(0x05);
    lda_imm(0x1B);
    sta(0x09);
    label(B_2191)
    lda_imm(0x11);
    sta(0x01);
    lda(0x09);
    sta(0x00);
    lda_X(0x2408);
    jsr(P_plot_472A);
    dec(0x09);
    dex;
    bpl(B_2191);
  
    ldx_imm(0x05);
    lda_imm(0x16);
    sta(0x09);
    
    label(B_21AA)
    lda_imm(0x00);
    sta(0x01);
    lda(0x09);
    sta(0x00);
    lda_X(0x2408);
    jsr(P_plot_472A);
    dec(0x09);
    dex;
    bpl(B_21AA);
  
    ldx_imm(0x27);
    stx(0x09);
    label(B_21C1)
    lda_imm(0x0C);
    sta(0x01);
    lda(0x09);
    sta(0x00);
    lda_X(0x240E);
    jsr(P_plot_472A);
    dec(0x09);
    dex;
    bpl(B_21C1);
  
    ldx_imm(0x05);
    lda_imm(0x14);
    sta(0x09);
    label(B_21DA)
    lda_imm(0x04);
    sta(0x01);
    lda(0x09);
    sta(0x00);
    lda_X(0x2408);
    jsr(P_plot_472A);
    dec(0x09);
    dex;
    bpl(B_21DA);
  
    jsr(P_221F);
    lda(0x6923);
    sta(0x6920);
    clc;
    adc_imm(0x01);
    jsr(P_draw_red_bar_20EF);
    jsr(P_214A);
    lda_imm(0x00);
    sta(0x041F);
    label(B_2204)

    // tempo
  
    jsr(P_adjust_xxx_level_20A9);
    lda(0x041F);
    beq(B_2204);
  
    lda(0x6920);
    sta(0x6923);

}
static void main_menu_display()
{
    flush_events();
    
    move16(0x7B89,0x09);
    lda_imm(0x05);
    sta(0x08);
    jsr(P_display_text_468A);
}
subroutine(P_main_menu_46E7)
{
    SDL_Event event;

    main_menu_display();
    
    while(1)
    {

	// keyboard loop

	while (SDL_WaitEvent(&event))
	{
	    switch(event.type)
	    {
	      case SDL_QUIT:
		quit();
		break;
	      case SDL_JOYBUTTONUP:
		
		P_0743();
		return;
		break;

	      case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		  case SDLK_RCTRL:
		  case SDLK_LCTRL:
		    P_0743();
		    return;
		      break;
		    default:
		      break;
		}
	      case SDL_KEYDOWN:	      
		flush_events();
		switch (event.key.keysym.sym)
		{
		  case SDLK_ESCAPE:
		    quit();
		    break;
		  case SDLK_i:
		    // instructions
		    lda_imm(0x00);
		    sta(0x08);
		    move16(0x2436,0x9);
		    jsr(P_display_text_468A);
		    SDL_Event event;
		    
		    while(1)
		    {
			SDL_WaitEvent(&event);
			if ((event.type == SDL_KEYDOWN) ||
			    (event.type == SDL_MOUSEBUTTONDOWN))
			{
			    break;
			}
		    }
		    main_menu_display();   
		    break;
		    
		    case SDLK_s:
		      // alter skill
	
		      adjust_skill_level();
		      main_menu_display();
		      break;
		    default:
		      break;

		}
		  
		break;
	    }
	}
    }
}


end_subroutine

// < A: character to plot. if 0xC then clear screen
// < 0x0: x
// < 0x1: y
//
// sounds to me like the ROM rip of the "PLOT" command...

void P_plot_472A()
{
    cmp_imm(0x0C);
    beq(B_46CD);

    plot_char(peek(0),peek(1)-1,r.a);
    rts;

    sta(0x03);
    // save x and y (x is uselessly saved ?)
    tya;
    pha;
    txa;
    pha;

    // compute location on screen
    
    lda(0x03);
    pha;
    lda(0x00);
    pha;
    lda(0x01);
    sta(0x00);
    lda_imm(0x28);
    sta(0x01);
    jsr(P_0_times_1_4A41);
    pla;
    clc;
    adc(0x02);
    sta(0x01);
    lda(0x03);
    adc_imm(0x00);
    sta(0x02);
    
    lda(0x01);
    clc;
    // add 0xBB80 to 1.W
    adc_imm(0x80);
    sta(0x01);
    lda(0x02);
    adc_imm(0xBB);
    sta(0x02);
    
    cmp_imm(0xA0);
    bcc(B_out);
  
    pla;

    // plot character on screen
    
    ldy_imm(0x00);
    sta_ind_Y(0x01);

    // restore x and y
    pla;
    tax;
    pla;
    tay;
    rts;

    // clear screen from 0xBB80 until 0xC000
    
    label(B_46CD)
    
    move16(0xBB00,0);
    ldy_imm(0x80);
    
    label(B_46D7)
    
    lda_imm(0x20);
    sta_ind_Y(0x00);
    iny;
    bne(B_46D7);
  
    inc(0x01);
    lda(0x01);
    cmp_imm(0xC0);
    bne(B_46D7);
  
    rts;
		  
label(B_out)
	pla;
	pla;
	tax;
	pla;
	tay;
	rts;
  
}

subroutine(P_game_over_4770)
{

	ldx_imm(0x00);
	jsr(P_tempo_Y_44FF);
	lda_imm(0x01);
	sta(hires_text_flag_05fe);
	move16(0x4783,0);
	rts;
  
 
}
end_subroutine

subroutine(E_game_over_4783)
{    
    jsr(P_47B6);
    jsr(P_482D);
    
    if (peek(0x400) != 0)
    {    
	jsr(P_499C);
	jsr(P_47B6);
    }
    
     
    jsr(P_4D45);
    
    SDL_Event event;
	// ESC pressed ?
    int replay = 0;
    while (!replay)
    {
	if (SDL_WaitEvent(&event))
	{
	    flush_events();
	    switch(event.type)
	    {
	      case SDL_JOYBUTTONUP:
		replay=1;
		break;
	      case SDL_QUIT:
		quit();
		break;
	      case SDL_KEYDOWN:	      
		switch (event.key.keysym.sym)
		{
		  case SDLK_ESCAPE:
		    goto_menu;
		    break;
		  case SDLK_RCTRL:
		  case SDLK_LCTRL:
		    replay=1;
		      break;
		    default:
		      break;
		}
	    }
	}
    }

    jsr(P_0743);

}
end_subroutine

subroutine(P_47B6)
{

	lda_imm(0x7D);
	sta(0x0A);
	lda_imm(0x49);
	sta(0x09);
	lda_imm(0x00);
	sta(0x08);
	jsr(P_display_text_468A);
	lda_imm(0x04);
	sta(0x0A);
	lda_imm(0x0A);
	sta(0x08);
	ldx_imm(0x3B);
label(B_47CF)
	ldy_imm(0x06);
	lda_imm(0x05);
	sta(0x09);
	sta(0x00);
	lda(0x0A);
	sta(0x01);
	lda_imm(0x03);
	jsr(P_plot_472A);
	inc(0x09);
label(B_47E2)
	lda(0x09);
	sta(0x00);
	lda(0x0A);
	sta(0x01);
	lda_X(hiscore_buffer_6700);
	ora_imm(0x30);
	jsr(P_plot_472A);
	dex;
	inc(0x09);
	dey;
	bne(B_47E2);
  
	inc(0x0A);
	inc(0x0A);
	dec(0x08);
	bne(B_47CF);
  
	lda_imm(0x0A);
	sta(0x08);
	lda_imm(0x04);
	sta(0x0A);
	ldx_imm(0xC7);
label(B_480A)
	ldy_imm(0x14);
	lda_imm(0x12);
	sta(0x09);
label(B_4810)
	lda(0x09);
	sta(0x00);
	lda(0x0A);
	sta(0x01);
	lda_X(hiscore_name_buffer_6780);
	jsr(P_plot_472A);
	dex;
	inc(0x09);
	dey;
	bne(B_4810);
  
	inc(0x0A);
	inc(0x0A);
	dec(0x08);
	bne(B_480A);
  
	rts;
  
}
end_subroutine

subroutine(P_482D)
{

	ldx_imm(0x00);
	ldy_imm(0x05);
label(B_4831)
	lda_X(score_9610);
	cmp_Y(hiscore_buffer_6700);
	bcc(B_4841);
  
	bne(B_4847);
  
	inx;
	dey;
	cpx_imm(0x06);
	bne(B_4831);
  
label(B_4841)
	lda_imm(0x00);
	sta(0x0400);
	rts;

	// hiscore entry
label(B_4847)
	lda_imm(0x06);
	sta(0x09);
	ldx_imm(0x00);

	// plot the score
label(B_484D)
	lda_imm(0x1A);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(score_9610);
	ora_imm(0x30);
	jsr(P_plot_472A);
	inc(0x09);
	inx;
	cpx_imm(0x06);
	bne(B_484D);
  
	ldx_imm(0x13);
	lda_imm(0x20);
label(B_4868)
	sta_X(hiscore_name_buffer_6780);
	dex;
	bpl(B_4868);
  
	ldx_imm(0x27);
	stx(0x09);
label(B_4872)
	lda_imm(0x18);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x7D89);
	jsr(P_plot_472A);
	dec(0x09);
	dex;
	bpl(B_4872);
  
	lda_imm(0x00);
	sta(0x0400);
	sta(0x0403);
	lda_imm(0x12);
	sta(0x09);
	ldx_imm(0x26);
	stx(0x0402);
	lda_X(0xBF40);
	ora_imm(0x80);
	sta_X(0xBF40);
	jsr(P_enter_hiscore_name_489E);
	
}
end_subroutine

subroutine(P_enter_hiscore_name_489E)
{
	lda_imm(0x13);
	sec;
	sbc(0x0400);
	sta(0x0404);
	jsr(P_hiscore_enter_letter_4909);
	cmp_imm(0x7F);
	beq(B_48EF_rubout);
  
	cmp_imm(0x0D);
	beq(B_48D9); // done
  
	ldx(0x09);
	cpx_imm(0x26);
	bcs(P_enter_hiscore_name_489E);
  
	ldy(0x0404);
	sta_Y(hiscore_name_buffer_6780);
	sta(0x0401);
	lda(0x09);
	sta(0x00);
	lda_imm(0x1A);
	sta(0x01);
	lda(0x0401);
	and_imm(0x7F);
	jsr(P_plot_472A);
	inc(0x09);
	inc(0x0400);
	jmp(P_enter_hiscore_name_489E);
  
label(B_48D9)
	ldx_imm(0x00);
	ldy_imm(0x05);
label(B_48DD)
	lda_Y(score_9610);
	sta_X(hiscore_buffer_6700);
	dey;
	inx;
	cpx_imm(0x06);
	bne(B_48DD);
  
	lda_imm(0x01);
	sta(0x0400);

	flush_events();
	
	rts;
  
label(B_48EF_rubout)
	lda(0x0400);
	beq(P_enter_hiscore_name_489E);
  
	dec(0x0400);

	// JFF: bugfixed the original rubout bug
	
	lda_imm(0x13);
	sec;
	sbc(0x0400);
	tay;
	lda_imm(' ');
	sta_Y(hiscore_name_buffer_6780);


	dec(0x09);
	lda(0x09);
	sta(0x00);
	lda_imm(0x1A);
	sta(0x01);
	lda_imm(0x20);
	jsr(P_plot_472A);
	jmp(P_enter_hiscore_name_489E);
  
}
end_subroutine

subroutine(P_hiscore_enter_letter_4909)
{
    OricTimer tmr = timer_init(1,40);

    while (1)
    {
	update_controls();
	timer_tick(&tmr);
	if (!controls.fire_pressed)    break;
    }
    while (1)
    {
	update_controls();
	timer_tick(&tmr);
	if (controls.left_pressed)   {jmp(B_left_494A);}
	else if (controls.right_pressed)    {jmp(B_right_4966);}
	if (controls.fire_pressed)
	{
	    break;
	}
    }


  
	ldx(0x0402);
	lda_imm(0x01);
	sta(0x0403);
	lda_X(0xBF40); // what's under the cursor
	jsr(P_decode_char_4984);
	rts;
 
	label(B_left_494A)
	
	if (!peek(0x0402))
	{
	    jmp(P_hiscore_enter_letter_4909);
	}
  
	lda_X(0xBF40);
	and_imm(0x7F);
	sta_X(0xBF40);
	dex;
	lda_X(0xBF40);
	ora_imm(0x80);
	sta_X(0xBF40);
	stx(0x0402);
	jmp(P_hiscore_enter_letter_4909);

	
label(B_right_4966)
	ldx(0x0402);
	cpx_imm(0x27);
	beq(P_hiscore_enter_letter_4909);
  
	lda_X(0xBF40);
	and_imm(0x7F);
	sta_X(0xBF40);
	inx;
	lda_X(0xBF40);
	ora_imm(0x80);
	sta_X(0xBF40);
	stx(0x0402);
	jmp(P_hiscore_enter_letter_4909);
  
}
end_subroutine

// > A contains char code, 0xD if over, 0x7F if rubout

subroutine(P_decode_char_4984)
{

	and_imm(0x7F);
	cmp_imm(0x61);
	bcc(B_499B);

	// rub
	cmp_imm(0x72);
	beq(B_4999);
  
	cmp_imm(0x75);
	beq(B_4999);
  
	cmp_imm(0x62);
	beq(B_4999);
  
	lda_imm(0x0D);
	rts;
  
label(B_4999)
// rubout
	lda_imm(0x7F);
label(B_499B)
	rts;
  
}
end_subroutine

subroutine(P_499C)
{

	lda_imm(0x01);
	sta(0x0400);
label(J_49A1)
	inc(0x0400);
	lda(0x0400);
	cmp_imm(0x0B);
	beq(B_49C9);
  
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	jsr(P_0_times_1_4A41);
	ldy(0x02);
	ldx_imm(0x06);
label(B_49B8)
	lda_X(0x66FF);
	cmp_Y(0x66FF);
	bcc(B_49C9);
  
	bne(J_49A1);
  
	dex;
	dey;
	bne(B_49B8);
  
	jmp(J_49A1);
  
label(B_49C9)
	ldy_imm(0x06);
label(B_49CB)
	lda_Y(0x66FF);
	sta_Y(0x0480);
	dey;
	bne(B_49CB);
  
	dec(0x0400);
	lda(0x0400);
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	jsr(P_0_times_1_4A41);
	ldy_imm(0x06);
label(B_49E5)
	lda_Y(0x66FF);
	sta_Y(0x66F9);
	iny;
	cpy(0x02);
	beq(B_49E5);
  
	bcc(B_49E5);
  
	ldy(0x02);
	ldx_imm(0x06);
label(B_49F6)
	lda_X(0x0480);
	sta_Y(0x66FF);
	dey;
	dex;
	bne(B_49F6);
  
	lda(0x0400);
	sta(0x00);
	lda_imm(0x14);
	sta(0x01);
	jsr(P_0_times_1_4A41);
	ldy_imm(0x14);
label(B_4A0E)
	lda_Y(0x677F);
	sta_Y(0x0480);
	dey;
	bne(B_4A0E);
  
	ldy_imm(0x14);
label(B_4A19)
	lda_Y(0x677F);
	sta_Y(0x676B);
	iny;
	cpy(0x02);
	beq(B_4A19);
  
	bcc(B_4A19);
  
	ldy(0x02);
	ldx_imm(0x14);
label(B_4A2A)
	lda_X(0x0480);
	sta_Y(0x677F);
	dey;
	dex;
	bne(B_4A2A);
  
	rts;
  
 
}
end_subroutine


// in 0x0 * 0x1 + A = deek(2)

// < 0x0
// < 0x1 line increment (nb rows)
// > 0x2
// > 0x3
// < A ???
//
//
void P_0_times_1_4A41()
{
    doke(2,peek(0)*peek(1));
}

subroutine(P_0_times_1_4A41_old)
{
    clr(0x02);
    ldx_imm(0x08);
	
    // done 8 times
    label(B_4A47)
	
    lsr(0x00);
    bcc(B_4A4E);  // even: skip

    // count bits of 0x0
    
    clc;
    adc(0x01);

    label(B_4A4E)
    ror_A();
    ror(0x02);
    dex;
    bne(B_4A47);
    
    sta(0x03);
    rts;
  
}
end_subroutine

subroutine(P_sound_routine_4A57)
{
    rts;
}
end_subroutine

subroutine(P_sound_routine_4A8B)
{
    rts;
    
	lda(VIA_PA);
	pha;
	dec(0x03);
	lda(0x03);
	cmp_imm(0x03);
	bcs(B_4AAF);
  
	asl_A();
	sta(0x00);
	lda(0x04);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	inc(0x00);
	lda(0x05);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	pla;
	sta(VIA_PA);
	rts;
  
label(B_4AAF)
	lda_imm(0x06);
	sta(0x00);
	lda(0x04);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	pla;
	sta(VIA_PA);
	rts;
  
}
end_subroutine

subroutine(P_4ABF)
{
    rts;
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x08);
	sta(0x00);
label(B_4ACE)
	lda(0x05FD);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	inc(0x00);
	lda(0x00);
	cmp_imm(0x0B);
	bne(B_4ACE);
  
	rts;
  
}
end_subroutine

subroutine(P_4ADF)
{

	lda(0x6901);
	bne(B_4AE5);
  
	rts;
  
label(B_4AE5)
	lda_imm(0x01);
	sta(0x6901);
	inc(0x6900);
	lda(0x6900);
	cmp_imm(0x20);
	bcc(B_GEN_6098);
	jmpl(P_4AFE);
label(B_GEN_6098)
  
	sta(0x04);
	lda_imm(0x04);
	sta(0x03);
	jsr(P_sound_routine_4A8B);
	rts;
  
}
end_subroutine

subroutine(P_4AFE)
{

	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	ora_imm(0x08);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	clr(0x6901);
	rts;
  
}
end_subroutine

subroutine(P_animate_shoot_4B12)
{

	lda(is_ship_shooting_6904);
	bne(B_4B18);
  
	rts;
  
label(B_4B18)
	lda(is_ship_shoot_y_6905);
	sec;
	sbc_imm(0x28);
	sta(is_ship_shoot_y_6905);
	sta(0x04);
	lda(0x6906);
	sbc_imm(0x00);
	sta(0x6906);
	sta(0x05);
	beq(B_4B39);
  
label(J_4B2F)
	sta(0x05);
	lda_imm(0x02);
	sta(0x03);
	jsr(P_sound_routine_4A8B);
	rts;
  
label(B_4B39)
	lda(is_ship_shoot_y_6905);
	beq(B_4B41);
  
	jmp(J_4B2F);
  
label(B_4B41)
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	ora_imm(0x02);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	clr(is_ship_shooting_6904);
	rts;
  
}
end_subroutine

subroutine(P_4B55)
{

	lda_imm(0x90);
	sta(is_ship_shoot_y_6905);
	lda_imm(0x01);
	sta(0x6906);
	lda_imm(0x01);
	sta(is_ship_shooting_6904);
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	and_imm(0xFD);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	rts;
  
}
end_subroutine

subroutine(P_4B73)
{

	clr(0x0400);
label(B_4B78)
	lda_imm(0x0A);
	sta(0x0401);
label(B_4B7D)
	dec(0x0400);
	bne(B_4B7D);
  
	dec(0x0401);
	bne(B_4B7D);
  
	jsr(P_animate_shoot_4B12);
	lda(is_ship_shooting_6904);
	bne(B_4B78);
  
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	rts;
  
}
end_subroutine

subroutine(P_4B9B)
{

	lda(0x6907);
	cmp_imm(0xF7);
	beq(B_4BB8);
  
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xF7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x04);
	sta(0x03);
	clr(0x04);
	jsr(P_sound_routine_4A8B);
label(B_4BB8)
	rts;
  
}
end_subroutine

subroutine(P_4BB9)
{

	lda(0x6907);
	cmp_imm(0xFF);
	beq(B_4BB8);
  
	lda_imm(0xFF);
	sta(0x01);
	lda_imm(0x07);
	sta(0x00);
	jsr(P_sound_routine_4A57);
label(B_4BB8)
  
	rts;
  
}
end_subroutine

subroutine(P_4BCC)
{

	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xF7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda(0x0400);
	cmp_imm(0x0F);
	bne(B_4BE5);
  
	lda_imm(0x0F);
	sta(0x0400);
	bne(B_4BEA);
  
label(B_4BE5)
	lda_imm(0x1F);
	sta(0x0400);
label(B_4BEA)
	sta(0x04);
	lda_imm(0x04);
	sta(0x03);
	jsr(P_sound_routine_4A8B);
	rts;
  
}
end_subroutine

subroutine(P_4BF4)
{

	clr(0x96B1);
	clr(0x96C0);
	clr(0x96F3);
	clr(0x00);
	clr(0x96EF);
	clr(0x01);
	clr(0x96F0);
	lda_imm(0xBC);
	sta(0x02);
	sta(0x96F1);
	lda_imm(0x4A);
	sta(0x03);
	lda_imm(0x81);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	lda_imm(0x2A);
	sta(0x96F2);
	jsr(P_screen_copy_5172);
	lda_imm(0x3F);
	sta(0x96B0);
	rts;
  
}
end_subroutine

subroutine(P_4C30)
{

	clr(0x00);
	clr(0x96EF);
	lda_imm(0x80);
	sta(0x96F0);
	sta(0x01);
	lda_imm(0xB9);
	sta(0x96F1);
	sta(0x02);
	lda_imm(0x4A);
	sta(0x03);
	lda_imm(0x81);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda_imm(0x01);
	sta(0x96D0);
	sta(0x96D3);
	lda_imm(0x0F);
	sta(0x96D4);
	clr(0x96D1);
	lda_imm(0xD8);
	sta(0x96D5);
	lda_imm(0xFF);
	sta(0x96D6);
	sta(0x96D8);
	lda_imm(0x01);
	sta(0x96DA);
	lda_imm(0x64);
	sta(0x96D9);
	lda_imm(0x01);
	sta(0x96D7);
	lda_imm(0x2A);
	sta(0x96F2);
	rts;
  
}
end_subroutine

subroutine(P_4C94)
{

	rts;
  
}
end_subroutine

subroutine(P_4C95)
{

	lda(0x00);
	pha;
	lda(0x01);
	pha;
	lda(0x03);
	pha;
	lda(0x6908);
	bne(B_4CC5);
  
	clr(0x6909);
	clr(0x690A);
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xFE);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x01);
	sta(0x6908);
	pla;
	sta(0x03);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
label(B_4CC5)
	lda(0x6909);
	clc;
	adc_imm(0x08);
	sta(0x6909);
	sta(0x04);
	lda(0x690A);
	adc_imm(0x00);
	sta(0x690A);
	sta(0x05);
	lda_imm(0x01);
	sta(0x03);
	jsr(P_sound_routine_4A8B);
	pla;
	sta(0x03);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
}
end_subroutine

subroutine(P_4CEB)
{

	lda(0x00);
	pha;
	lda(0x01);
	pha;
	lda(0x03);
	pha;
	lda(0x04);
	pha;
	lda(0x05);
	pha;
	lda(0x690B);
	bne(B_4D1F);
  
	lda_imm(0x07);
	sta(0x00);
	lda_imm(0xC7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x01);
	sta(0x690B);
	pla;
	sta(0x05);
	pla;
	sta(0x04);
	pla;
	sta(0x03);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
label(B_4D1F)
	ldx_imm(0x06);
label(B_4D21)
lda(VIA_T1CL);  // random again
	eor(0x02);
	and_imm(0x1F);
	sta(0x04);
	stx(0x03);
	jsr(P_sound_routine_4A8B);
	dex;
	cpx_imm(0x03);
	bne(B_4D21);
  
	tax;
	pla;
	sta(0x05);
	pla;
	sta(0x04);
	pla;
	sta(0x03);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
}
end_subroutine

subroutine(P_4D45)
{

	ldx_imm(0x27);
	stx(0x09);
label(B_4D49)
	lda_imm(0x18);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x4D62); // space to play : esc to goto menu
	jsr(P_plot_472A);
	dec(0x09);
	dex;
	bpl(B_4D49);
  
	rts;
  
 
}

end_subroutine

subroutine(P_animate_birds_5000)
{

	lda(stage_9600);
	
	cmp_imm(0x02);
	bcc(J_5070); // stage 1: skip
  
	if (peek(nb_kamikaze_birds_96fe))
	{
	
	    // find a non-dead enemy kamikaze slot
	    ldx_imm(0x13);
	    label(B_500E)
	    lda_X(0x9014);
	    bne(B_501D);
  
	    dex;
	    dex;
	    dex;
	    dex;
	    dex;
	    bpl(B_500E);

	    jmp(J_5070);
   
	    label(B_501D)
	    dex;
	    lda_X(0x9014);
	    sta(0x00);
	    dex;
	    lda_X(0x9014);
	    clc;
	    adc_imm(0x50);
	    sta_X(0x9014);
	    sta(0x01);
	    dex;
	    lda_X(0x9014);
	    adc_imm(0x00);
	    sta_X(0x9014);
	    sta(0x02);
	    cmp_imm(0xBD);
	    bcs(B_5058);

	    move16(0x8345,0x3); // source

	    lda_imm(0x04);
	    sta(0x05);
	    lda_imm(0x40);
	    sta(0x06);
	    txa;
	    pha;
	    jsr(P_screen_copy_5172);
	    pla;
	    tax;
	    jmp(J_506C);
  
	    label(B_5058)
// (level 2) enemy has crashed on the ground
	    one_foe_killed(0);
	    sound_play("kamikaze");
	    sound_stop(falling_sound_id[r.x]);
	    falling_sound_id[r.x] = 0;
	    
	    inx;
	    inx;
	    inx;
	    lda_imm(0x00);
	    sta_X(0x9014);
	    dex;
	    jsr(P_5871);
	    dex;
	    dex;
	    dec(nb_kamikaze_birds_96fe);
	    label(J_506C)
	    dex;
	    dex;
	    bpl(B_500E);
	}

	/*if (!peek(nb_kamikaze_birds_96fe))
	{
	    // no kamikazes: stop falling sounds
	    stop_kamikaze_sounds();
	    }*/
label(J_5070)
	dec(0x041E);
	bne(B_5090);
  
	ldx(0x041D);
	lda_X(0x9400);
	sta(0x041E);
	dex;
	lda_X(0x9400);
	sta(0x0401);
	dex;
	lda_X(0x9400);
	sta(0x0404);
	dex;
	stx(0x041D);
label(B_5090)
	lda_imm(0x00);
	sta(0x0402);
	dec(0x0400);
	bne(B_50A6);
  
	lda(0x0401);
	sta(0x0400);
	lda(0x0404);
	sta(0x0402);
label(B_50A6)
	ldx_imm(0x13);
label(J_50A8)
	lda_X(0x9000);
	bne(B_50B5);
  
	dex;
	dex;
	dex;
	dex;
	dex;
	bpl(J_50A8);
  
	rts;
  
	label(B_50B5)
	jsr(P_51F2);
	cmp_imm(0x02);
	beq(B_50D6);
  
	lda(stage_9600);
	cmp_imm(0x02);
	bcc(B_50D6);
  
	move16(0x8345,3);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x40);
	sta(0x06);
	jmp(J_do_draw);
  
	label(B_50D6)
// enemy data
// source
	lda(0x9601);
	sta(0x03);
	lda(0x9602);
	sta(0x04);
	// size
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x6E);
	sta(0x06);
label(J_do_draw)
	dex;
	txa;
	pha;
	lda_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	sta(0x02);
	dex;
	lda_X(0x9000);
	pha;

	// draw the birds
	
	jsr(P_screen_copy_5172);	
	pla;
	tay;
	pla;
	tax;
	lda_X(0x9000);
	clc;
	adc_Y(0x9500);
	sta_X(0x9000);
	bpl(B_5133);
  
	clc;
	adc_imm(0x03);
	sta_X(0x9000);
	dex;
	lda_X(0x9000);
	sec;
	sbc_imm(0x01);
	sta_X(0x9000);
	dex;
	lda_X(0x9000);
	sbc_imm(0x00);
	sta_X(0x9000);
	inx;
	inx;
	clc;
	jmp(J_513D);
  
label(B_5133)
	cmp_imm(0x03);
	bcc(J_513D);
  
	lda_imm(0x00);
	sta_X(0x9000);
	sec;
label(J_513D)
	dex;
	dey;
	lda_X(0x9000);
	adc_Y(0x9500);
	sta_X(0x9000);
	dex;
	dey;
	lda_X(0x9000);
	adc_Y(0x9500);
	sta_X(0x9000);
	dex;
	lda_X(0x9000);
	sec;
	sbc(0x0402);
	cmp_imm(0xFF);
	bne(B_5161);
  
	lda_imm(0xEF);
label(B_5161)
	cmp_imm(0xF0);
	bcc(B_5167);
  
	lda_imm(0x02);
label(B_5167)
	sta_X(0x9000);
	dex;
	bpl(B_516E);
  
	rts;
  
label(B_516E)
	jmp(J_50A8);
  
	rts;
  
}
end_subroutine

/* copy to screen
 *
 * params:
 *
 * 0.b : frame number (if animated sprite)
 * 1.w : destination
 * 3.w : source
 * 5   : number of rows
 * 6.b : number of bytes
 */
void P_screen_copy_5172()
{
    int dest_address = deek(1);
    int source_address = deek(3);
    int frame_index = peek(0);
    int nb_bytes = peek(6);
    int nb_rows = peek(5);
    int i;

    source_address += frame_index * nb_bytes;
    int nb_lines = nb_bytes / nb_rows;
   
    if ((nb_rows > 0) && (dest_address >= 0xA000) && (dest_address < 0xC000))
//	&& (source_address + nb_bytes < 0xC000) && (dest_address + nb_lines*40 < 0xC000))
    {
	
	for (i = 0; i < nb_lines; i++)
	{
	    cpu_memcopy(source_address,dest_address,nb_rows);
	    
	    dest_address += 40;
	    source_address += nb_rows;	    
	}
    

	doke(1,dest_address);
    }
  
}

/*
subroutine(P_screen_copy_5172)
{

	lda(0x02);
	cmp_imm(0xA0);
	bcc(B_sc_rts); // not in hires screen
  
	cmp_imm(0xC0); // not in hires screen
	bcs(B_sc_rts);
  
	ldx(0x00);
	beq(B_5190);

label(B_5180)
	lda(0x03);
	clc;
	adc(0x06);
	sta(0x03);
	lda(0x04);
	adc_imm(0x00);
	sta(0x04);
	dex;
	bne(B_5180);
  
label(B_5190)
	ldy_imm(0x00);
label(B_5192)
	ldx(0x05);
label(B_5194)
	lda_ind_Y(0x03);
	sta_ind_Y(0x01);
	iny;
	dex;
	bne(B_5194);
  
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	lda(0x01);
	sec;
	sbc(0x05);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	cpy(0x06);
	bne(B_5192);
label(B_sc_rts)
  
	rts;
  
}
end_subroutine*/


// < 1.W: destination
// < 3.B: data to copy
// < 5.B: # of columns
// < 6.B: number of lines
void P_fill_zone_51BB_not_working()
{
    int dest = deek(1);
    unsigned char c = peek(3);
    int nb_columns = peek(5);
    int nb_lines = peek(6);
    if (nb_lines > 0x7F)
    {
	nb_lines = 0x100 - nb_lines;
	dest = dest - nb_lines * 40;
    }
    
    int i,j;

    for (i = 0; i < nb_lines; i++)
    {
	int dest_line = dest + i * 40;
	
	for (j = 0; j < nb_columns; j++)
	{
	    cpu_write_mem(c,dest_line++);
	}
    }
  
}

void P_fill_zone_51BB()
{

	lda(0x02);
	cmp_imm(0xA0);
	bcc(B_sc_rts);
  
	cmp_imm(0xBF);
	bcs(B_sc_rts);
  
	ldy_imm(0x00);
label(B_51C7)
	ldx_imm(0x00);
label(B_51C9)
	lda(0x03);
	sta_ind_Y(0x01);
	iny;
	inx;
	cpx(0x05);
	bne(B_51C9);
  
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	lda(0x01);
	sec;
	sbc(0x05);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	cpy(0x06);
	bne(B_51C7);
label(B_sc_rts)
  
	rts;
  
	}

subroutine(P_51F2)
{
	pha;
	lda(VIA_T1CL);
	cmp(0x0405);
	bcc(B_51FD);
  
	pla;
	rts;
  
label(B_51FD)
	lda(0x96C0);
	beq(B_5204);
  
	pla;
	rts;
  
label(B_5204)
	ldy_imm(0x01);
	sty(0x00);
	txa;
	pha;
	dex;
	lda_X(0x9000);
	clc;
	adc_imm(0x04);
	cmp_imm(0x06);
	bcc(B_521C);
  
	sec;
	sbc_imm(0x06);
	ldy_imm(0x02);
	sty(0x00);
label(B_521C)
	lsr_A();
	sta(0x96C1);
	dex;
	lda_X(0x9000);
	clc;
	adc(0x00);
	sta(0x96C2);
	dex;
	lda_X(0x9000);
	adc_imm(0x00);
	sta(0x96C3);
	lda_imm(0x01);
	sta(0x96C0);
	pla;
	tax;
	pla;
	rts;
  
}
end_subroutine

subroutine(P_523C)
{

	ldx_imm(0x1E);
	lda(0x900B);
	sta(0x07);
label(B_5243)
	lda_X(0x9014);
	bne(B_5253);
  
	dex;
	dex;
	dex;
	bpl(B_5243);
  
	lda(0x07);
	sta(0x900B);
	rts;
  
label(B_5253)
	lda(0x900B);
	cmp_imm(0x02);
	beq(B_52AD);
  
	dex;
	dec_X(0x9014);
	lda_X(0x9014);
	sta(0x00);
	bpl(B_52A4);
  
	lda_imm(0x05);
	sta_X(0x9014);
	sta(0x00);
	dex;
	dec_X(0x9014);
	lda_X(0x9014);
	sta(0x01);
	cmp_imm(0xC3);
	bcs(J_527D);
  
	lda_imm(0x02);
	sta(0x07);
label(J_527D)
	lda_imm(0xBC);
	sta(0x02);
	lda_imm(0xD6);
	sta(0x03);
	lda_imm(0x72);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x2A);
	sta(0x06);
	txa;
	pha;
	jsr(P_5633);
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(B_5243);
  
	lda(0x07);
	sta(0x900B);
	rts;
  
label(B_52A4)
	dex;
	lda_X(0x9014);
	sta(0x01);
	jmp(J_527D);
  
label(B_52AD)
	dex;
	inc_X(0x9014);
	lda_X(0x9014);
	sta(0x00);
	cmp_imm(0x06);
	bcc(B_52A4);
  
	lda_imm(0x00);
	sta_X(0x9014);
	sta(0x00);
	dex;
	inc_X(0x9014);
	lda_X(0x9014);
	sta(0x01);
	cmp_imm(0xE5);
	bcc(J_527D);
  
	lda_imm(0x01);
	sta(0x07);
	bne(J_527D);
  
}
end_subroutine

subroutine(P_52D4)
{

	lda(0x00);
	sta(0x07);
	lda(0x01);
	sta(0x08);
	sta(0x00);
	lda(0x02);
	sta(0x09);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x03);
	clc;
	adc_imm(0xC9);
	cmp_imm(0xE5);
	bcc(B_52F7);
  
	sec;
	sbc_imm(0x28);
label(B_52F7)
	sta(0x01);
	lda(0x07);
	sta(0x00);
	ldx_imm(0x1E);
label(B_52FF)
	lda_X(0x9014);
	bne(B_530B);
  
	dex;
	dex;
	dex;
	bpl(B_52FF);
  
	bmi(B_5336);
  
label(B_530B)
	dex;
	dex;
	lda_X(0x9014);
	sec;
	sbc_imm(0x02);
	cmp(0x01);
	beq(B_5342);
  
	clc;
	adc_imm(0x01);
	cmp(0x01);
	beq(B_5342);
  
	clc;
	adc_imm(0x01);
	cmp(0x01);
	beq(B_5342);
  
	clc;
	adc_imm(0x01);
	cmp(0x01);
	beq(B_5342);
  
	clc;
	adc_imm(0x01);
	cmp(0x01);
	beq(B_5342);
  
	dex;
	bpl(B_52FF);
  
label(B_5336)
	ldx_imm(0x1E);
label(B_5338)
	lda_X(0x9014);
	beq(B_534B);
  
	dex;
	dex;
	dex;
	bpl(B_5338);
  
label(B_5342)
	lda(0x08);
	sta(0x01);
	lda(0x09);
	sta(0x02);
	rts;
  
label(B_534B)
	lda_imm(0x01);
	sta_X(0x9014);
	dex;
	lda(0x00);
	sta_X(0x9014);
	dex;
	lda(0x01);
	sta_X(0x9014);
	lda_imm(0xBC);
	sta(0x02);
	lda_imm(0xD6);
	sta(0x03);
	lda_imm(0x72);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x2A);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda(0x08);
	sta(0x01);
	lda(0x09);
	sta(0x02);
	rts;
  
}
end_subroutine

subroutine(P_537C)
{

	ldy_imm(0x08);
	sec;
	lda(0x01);
	sbc(0x02);
label(B_5383)
	php;
	rol(0x04);
	asl(0x00);
	rol_A();
	plp;
	bcc(B_5391);
  
	sbc(0x02);
	jmp(J_5393);
  
label(B_5391)
	adc(0x02);
label(J_5393)
	dey;
	bne(B_5383);
  
	bcs(B_539B);
  
	adc(0x02);
	clc;
label(B_539B)
	rol(0x04);
	sta(0x03);
	rts;
  
}
end_subroutine

subroutine(P_53A0)
{

	lda(VIA_T1CL);
	cmp_imm(0xFA);
	bcc(B_53AA);
  
	jsr(P_55AD);
label(B_53AA)
	lda(0x9608);
	cmp_imm(0x02);
	beq(B_53FA);
  
	dec(0x9604);
	bpl(B_53BB);
  
	lda_imm(0x02);
	sta(0x9608);
label(B_53BB)
	dec(0x9605);
	lda(0x9605);
	sta(0x00);
	bpl(B_53ED);
  
	lda_imm(0x02);
	sta(0x9605);
	sta(0x00);
	dec(0x9606);
	lda(0x9606);
	sta(0x01);
	lda(0x9607);
	sta(0x02);
label(J_53D9)
	lda_imm(0x82);
	sta(0x03);
	lda_imm(0x75);
	sta(0x04);
	lda_imm(0x06);
	sta(0x05);
	lda_imm(0x48);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
label(B_53ED)
	lda(0x9606);
	sta(0x01);
	lda(0x9607);
	sta(0x02);
	jmp(J_53D9);
  
label(B_53FA)
	inc(0x9604);
	lda(0x9604);
	cmp_imm(0x50);
	bcc(B_5409);
  
	lda_imm(0x01);
	sta(0x9608);
label(B_5409)
	inc(0x9605);
	lda(0x9605);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_53ED);
  
	lda_imm(0x00);
	sta(0x9605);
	sta(0x00);
	inc(0x9606);
	lda(0x9606);
	sta(0x01);
	lda(0x9607);
	sta(0x02);
	jmp(J_53D9);
  
}
end_subroutine

subroutine(P_542C)
{

	lda(0x96F3);
	bne(B_5496);

	// shoot from paratron level
	
	if (!controls.fire_pressed)
	{
	    return;
	}
	else
	{
	    sound_play("shoot");
	lda(0x96EF);
	lsr_A();
	sta(0x96F4);
	tax;
	lda_imm(0x30);
	cpx_imm(0x00);
	beq(B_5453);
  
label(B_544E)
	lsr_A();
	lsr_A();
	dex;
	bne(B_544E);
  
label(B_5453)
	sta(0x96F7);
	lda(0x96F0);
	clc;
	adc_imm(0x01);
	sta(0x96F5);
	lda(0x96F1);
	adc_imm(0x00);
	sta(0x96F6);
	lda_imm(0x01);
	sta(0x96F3);
	lda(0x96F5);
	sec;
	sbc_imm(0xF0);
	sta(0x96F5);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x96F6);
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	and_imm(0xF7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x00);
	sta(0x6900);
	lda_imm(0x01);
	sta(0x6901);
	rts;
	}
	
label(B_5496)
	lda(0x96F5);
	sec;
	sbc_imm(0x50);
	sta(0x96F5);
	sta(0x01);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x96F6);
	sta(0x02);
	cmp_imm(0xA5);
	bcc(B_54C9);
  
	ldy_imm(0x00);
	lda_ind_Y(0x01);
	cmp_imm(0x40);
	bcc(B_54ED);
  
	cmp_imm(0x80);
	bcs(B_54ED);
  
	bit(0x96F7);
	beq(B_54ED);
  
	jsr(P_5506);
	jsr(P_5F5B);
	jsr(P_4B55);
label(B_54C9)
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x08);
	sta(0x06);
	lda(0x96F5);
	clc;
	adc_imm(0x50);
	sta(0x01);
	lda(0x96F6);
	adc_imm(0x00);
	sta(0x02);
	jsr(P_fill_zone_51BB);
	lda_imm(0x00);
	sta(0x96F3);
	rts;
  
label(B_54ED)
	lda(0x96F4);
	sta(0x00);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	lda_imm(0x2A);
	sta(0x03);
	lda_imm(0x83);
	sta(0x04);
	jsr(P_screen_copy_5172);
	rts;
	
}
end_subroutine

subroutine(P_5506)
{

	jsr(P_57A8);
	lda(0x96F5);
	sta(0x00);
	lda(0x96F6);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x03);
	sta(0x05);
	lda(0x04);
	sta(0x06);
	ldx_imm(0x0A);
label(B_5527)
	lda_X(0x9000);
	bne(B_5534);
  
	dex;
	dex;
	dex;
	dex;
label(B_5530)
	dex;
	bpl(B_5527);
  
	rts;
  
label(B_5534)
	dex;
	dex;
	dex;
	lda_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x05);
	cmp(0x03);
	bcc(B_5530);
  
	sec;
	sbc_imm(0x04);
	cmp(0x03);
	bcs(B_5530);
  
	lda(0x06);
	cmp(0x04);
	bcc(B_5530);
  
	sec;
	sbc_imm(0x1B);
	cmp(0x04);
	bcs(B_5530);
  
	inx;
	inx;
	inx;
	inx;
	lda_imm(0x00);
	sta_X(0x9000);
	dex;
	dex;
	dex;
	lda_X(0x9000);
	sec;
	sbc_imm(0x01);
	sta(0x01);
	dex;
	lda_X(0x9000);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x78);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	one_foe_killed(1);
	lda_imm(0x00);
	sta(0x961A);
	sta(0x961B);
	sta(0x961C);
	lda_imm(0x01);
	sta(0x961D);
	lda_imm(0x05);
	sta(0x961E);
	jsr(P_increase_score_641B);
	rts;
  
}
end_subroutine

subroutine(P_55AD)
{

	lda(0x96C0);
	beq(B_55B3);
  
	rts;
  
label(B_55B3)
	lda(0x9605);
	clc;
	adc_imm(0x01);
	cmp_imm(0x03);
	bcc(B_55C3);
  
	lda_imm(0x00);
	sta(0x96C1);
	sec;
label(B_55C3)
	lda(0x9606);
	adc_imm(0x23);
	sta(0x96C2);
	lda(0x9607);
	adc_imm(0x03);
	sta(0x96C3);
	lda_imm(0x01);
	sta(0x96C0);
	rts;
  
}
end_subroutine

subroutine(P_55D9)
{

	lda(0x96C0);
	bne(B_55DF);
  
	rts;
  
label(B_55DF)
	lda(0x96C2);
	clc;
	adc_imm(0x50);
	sta(0x96C2);
	sta(0x01);
	lda(0x96C3);
	adc_imm(0x00);
	sta(0x96C3);
	sta(0x02);
	cmp_imm(0xBB);
	beq(B_5611);
  
	lda(0x96C1);
	sta(0x00);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	lda_imm(0x8A);
	sta(0x04);
	lda_imm(0x2A);
	sta(0x03);
	jsr(P_screen_copy_5172);
	rts;
  
label(B_5611)
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	lda(0x01);
	sec;
	sbc_imm(0x50);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	jsr(P_fill_zone_51BB);
	lda_imm(0x00);
	sta(0x96C0);
	rts;
  
}
end_subroutine

subroutine(P_5633)
{

	lda(0x00);
	cmp_imm(0x05);
	beq(B_5643);
  
	lda(0x900B);
	cmp_imm(0x02);
	beq(B_568A);
  
	jmp(J_56D6);
  
label(B_5643)
	lda(0x00);
	pha;
	lda(0x01);
	pha;
	lda(0x02);
	pha;
	lda(0x03);
	pha;
	lda(0x04);
	pha;
	lda(0x05);
	pha;
	lda(0x06);
	pha;
	lda(0x01);
	sec;
	sbc_imm(0xEF);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x50);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pla;
	sta(0x06);
	pla;
	sta(0x05);
	pla;
	sta(0x04);
	pla;
	sta(0x03);
	pla;
	sta(0x02);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
label(B_568A)
	lda(0x00);
	beq(B_568F);
  
	rts;
  
label(B_568F)
	lda(0x00);
	pha;
	lda(0x01);
	pha;
	lda(0x02);
	pha;
	lda(0x03);
	pha;
	lda(0x04);
	pha;
	lda(0x05);
	pha;
	lda(0x06);
	pha;
	lda(0x01);
	sec;
	sbc_imm(0xF0);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pla;
	sta(0x06);
	pla;
	sta(0x05);
	pla;
	sta(0x04);
	pla;
	sta(0x03);
	pla;
	sta(0x02);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
label(J_56D6)
	lda(0x00);
	cmp_imm(0x04);
	beq(B_56DD);
  
	rts;
  
label(B_56DD)
	lda(0x00);
	pha;
	lda(0x01);
	pha;
	lda(0x02);
	pha;
	lda(0x03);
	pha;
	lda(0x04);
	pha;
	lda(0x05);
	pha;
	lda(0x06);
	pha;
	lda(0x01);
	sec;
	sbc_imm(0xEF);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pla;
	sta(0x06);
	pla;
	sta(0x05);
	pla;
	sta(0x04);
	pla;
	sta(0x03);
	pla;
	sta(0x02);
	pla;
	sta(0x01);
	pla;
	sta(0x00);
	rts;
  
}
end_subroutine

subroutine(P_handle_rescue_ship_5724)
{

	lda(0x90DC);
	bne(B_574B);

	// display rescue ship random
	
	lda(VIA_T1CL);
	cmp_imm(0x64);
	bne(B_5738);

	
	lda(0x0403);
	cmp(0x6608);
	bcc(B_5739);
  
	label(B_5738)
	rts;
  
	label(B_5739)
	// init:
// display "rescue" ship
	special_ship_sound = sound_play("special_ship");
	lda_imm(0x00);
	sta(0x90DD);
	sta(0x90DE);
	lda_imm(0xAA);
	sta(0x90DF);
	lda_imm(0x01);
	sta(0x90DC);
label(B_574B)
	inc(0x90DD);
	lda(0x90DD);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_5761);
  
	lda_imm(0x00);
	sta(0x90DD);
	sta(0x00);
	inc(0x90DE);
label(B_5761)
	lda(0x90DE);
	sta(0x01);
	cmp_imm(0x24);
	bcc(B_GEN_7722);
	//sound_stop(special_ship_sound);
	
	jmpl(P_remove_rescue_ship_5783);
label(B_GEN_7722)
  
	lda(0x90DF);
	sta(0x02);
	lda_imm(0xDB);
	sta(0x03);
	lda_imm(0x76);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x3C);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_remove_rescue_ship_5783)
{
	lda(0x90DE);
	sec;
	sbc_imm(0x01);
	sta(0x01);
	lda(0x90DF);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x06);
	sta(0x05);
	lda_imm(0x6C);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0x00);
	sta(0x90DC);
	rts;
  
  
}
end_subroutine

subroutine(P_57A8)
{

	lda(0x90DC);
	beq(B_57A7);
  
	lda(0x96F5);
	sta(0x00);
	lda(0x96F6);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x03);
	sta(0x05);
	lda(0x04);
	sta(0x06);
	lda(0x90DE);
	sta(0x00);
	lda(0x90DF);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x05);
	cmp(0x03);
	bcc(B_57A7);
  
	sec;
	sbc_imm(0x05);
	cmp(0x03);
	cmp(0x03);
	bcs(B_57A7);
  
	lda(0x06);
	cmp(0x04);
	bcc(B_57A7);
  
	sec;
	sbc_imm(0x14);
	cmp(0x04);
	bcs(B_57A7);
	
	sound_play("explosion");
sound_stop(special_ship_sound);
	jsr(P_remove_rescue_ship_5783);
	lda_imm(0x00);
	sta(0x961A);
	sta(0x961B);
	sta(0x961C);
	sta(0x961E);
	lda_imm(0x05);
	sta(0x961D);
	jsr(P_increase_score_641B);
	nop;
	ldx_imm(0x1E);
label(B_5815)
	lda_X(0x9014);
	bne(B_5820);
  
	dex;
	dex;
	dex;
	bpl(B_5815);
label(B_57A7)
	rts;

  
label(B_5820)
	lda_imm(0x00);
	sta_X(0x9014);
	dex;
	lda_X(0x9014);
	sta(0x09);
	dex;
	lda_X(0x9014);
	sta(0x01);
	sbc_imm(0xF0);
	sta(0x07);
	lda_imm(0xBC);
	sta(0x02);
	sbc_imm(0x00);
	sta(0x08);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x2A);
	sta(0x06);
	txa;
	pha;
	jsr(P_fill_zone_51BB);
	lda(0x09);
	cmp_imm(0x05);
	bne(B_586B);
  
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x2A);
	sta(0x06);
	jsr(P_fill_zone_51BB);
label(B_586B)
	pla;
	tax;
	dex;
	bpl(B_5815);
  
	rts;
  
}
end_subroutine

subroutine(P_5871)
{

	txa;
	pha;
	lda(0x9609);
	beq(B_587E);
  
	jsr(P_5981);
	pla;
	tax;
	pha;
label(B_587E)
	dex;
	lda_imm(0x08);
	sta(0x9609);
	lda_imm(0x00);
	sta(0x00);
	lda_X(0x9014);
	sec;
	sbc_imm(0x50);
	sta(0x01);
	sta(0x960A);
	sta(0x07);
	dex;
	lda_X(0x9014);
	sbc_imm(0x00);
	sta(0x02);
	sta(0x08);
	sta(0x960B);
	lda_imm(0x90);
	sta(0x03);
	lda_imm(0x76);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x4B);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	and_imm(0xEF);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x06);
	sta(0x03);
	lda_imm(0x0F);
	sta(0x04);
	jsr(P_sound_routine_4A8B);
	jmpl(P_58CE);
	
}
end_subroutine

subroutine(P_58CE)
{

	lda(0x07);
	sta(0x00);
	lda(0x08);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x03);
	sta(0x07);
	lda(0x96F0);
	sta(0x00);
	lda(0x96F1);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_537C);
	lda(0x03);
	clc;
	adc_imm(0x03);
	cmp(0x07);
	bcc(B_5914);
  
	lda(0x07);
	clc;
	adc_imm(0x05);
	cmp(0x03);
	bcc(B_5914);
  
	pla;
	jsr(P_ship_explodes_451C);
	lda_imm(0xFF);
	sta(0x041F);
	rts;
  
label(B_5914)
	pla;
	tax;
	rts;
  
}
end_subroutine

subroutine(P_5917)
{

	lda(0x9609);
	cmp_imm(0x01);
	beq(B_594F);
  
	cmp_imm(0x00);
	bne(B_5923);
  
	rts;
  
label(B_5923)
	dec(0x9609);
	lda(0x960A);
	sta(0x01);
	sta(0x07);
	lda(0x960B);
	sta(0x02);
	sta(0x08);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x90);
	sta(0x03);
	lda_imm(0x76);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x4B);
	sta(0x06);
	jsr(P_screen_copy_5172);
	pha;
	jmpl(P_58CE);
  
label(B_594F)
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	ora_imm(0x10);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda(0x960A);
	sta(0x01);
	sta(0x07);
	lda(0x960B);
	sta(0x02);
	sta(0x08);
	lda_imm(0x00);
	sta(0x03);
	sta(0x9609);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x4B);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pha;
	jmpl(P_58CE);
  
}
end_subroutine

subroutine(P_5981)
{

	lda(0x960A);
	sta(0x01);
	sta(0x07);
	lda(0x960B);
	sta(0x02);
	sta(0x08);
	lda_imm(0x00);
	sta(0x03);
	sta(0x9609);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x4B);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	rts;
  
}
end_subroutine

subroutine(P_59A2)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xBF);
	sta(0x02);
	lda_imm(0x68);
	sta(0x01);
	lda_imm(0x48);
	sta(0x03);
	lda_imm(0x64);
	sta(0x04);
	lda_imm(0x78);
	sta(0x05);
	lda_imm(0x78);
	sta(0x06);
	jsr(P_screen_copy_5172);
	jsr(P_update_player_status_5FDE);
	rts;
  
}
end_subroutine

subroutine(P_59C5)
{

	lda_imm(0xE8);
	sta(0x00);
	lda_imm(0xA8);
	sta(0x01);
	ldy_imm(0x00);
label(B_59CF)
	lda_imm(0x7F);
	sta_ind_Y(0x00);
	inc(0x00);
	bne(B_59D9);
  
	inc(0x01);
label(B_59D9)
	lda(0x01);
	cmp_imm(0xAB);
	bcc(B_59CF);
  
	lda(0x00);
	cmp_imm(0xB8);
	bcc(B_59CF);
  
	lda_imm(0x0F);
	sta(0x01);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x13);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0x01);
	sta(0x02);
	lda_imm(0xE8);
	sta(0x00);
	lda_imm(0xA8);
	sta(0x01);
	ldy_imm(0x00);
label(B_5A0A)
	lda(0x02);
	sta_ind_Y(0x00);
	inc(0x02);
	cmp_imm(0x08);
	bcc(B_5A18);
  
	lda_imm(0x01);
	sta(0x02);
label(B_5A18)
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	lda(0x01);
	cmp_imm(0xAB);
	bcc(B_5A0A);
  
	lda(0x00);
	cmp_imm(0xB8);
	bcc(B_5A0A);
  
	rts;
  
}
end_subroutine

// specific shoot handle at zorgons stage

subroutine(P_handle_ship_shots_zorgons_5A32)
{

    lda(0x96F3);
    bne(B_5AAE);
    
    if (!controls.fire_pressed)
    {
	return;
    }
    else
    {
	sound_play("shoot");
	
	ldx_imm(0x01);
	stx(0x00);
	lda(0x96EF);
	clc;
	adc_imm(0x04);
	cmp_imm(0x06);
	bcc(B_5A5B);
  
	sec;
	sbc_imm(0x06);
	ldx_imm(0x02);
	stx(0x00);
	label(B_5A5B)
	lsr_A();
	sta(0x96F4);
	tax;
	lda_imm(0x30);
	cpx_imm(0x00);
	beq(B_5A6B);
  
	label(B_5A66)
	lsr_A();
	lsr_A();
	dex;
	bne(B_5A66);
  
	label(B_5A6B)
	sta(0x96F7);
	lda(0x96F0);
	clc;
	adc(0x00);
	sta(0x96F5);
	lda(0x96F1);
	adc_imm(0x00);
	sta(0x96F6);
	lda_imm(0x01);
	sta(0x96F3);
	lda(0x96F5);
	sec;
	sbc_imm(0xF0);
	sta(0x96F5);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x96F6);
	lda_imm(0x07);
	sta(0x00);
	lda(0x6907);
	and_imm(0xF7);
	sta(0x01);
	jsr(P_sound_routine_4A57);
	lda_imm(0x00);
	sta(0x6900);
	lda_imm(0x01);
	sta(0x6901);
	rts;
    }
	
label(B_5AAE)
	lda(0x96F5);
	sec;
	sbc_imm(0x50);
	sta(0x96F5);
	sta(0x01);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x96F6);
	sta(0x02);
	cmp_imm(0xA2);
	bcc(B_5B28);
  
	ldy_imm(0x00);
	lda_ind_Y(0x01);
	cmp_imm(0x40);
	bcc(B_5B2F);
  
	bit(0x96F7);
	beq(B_5B2F);
  
	lda_imm(0x01);
	sta(0x07);
	lda(0x02);
	cmp_imm(0xAC);
	bcs(J_5AE6);
  
	cmp_imm(0xA8);
	bcc(J_5AE6);
  
	lda_imm(0x00);
	sta(0x07);
label(J_5AE6)
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	lda(0x96F5);
	sta(0x08);
	clc;
	adc_imm(0x50);
	sta(0x01);
	lda(0x96F6);
	sta(0x09);
	adc_imm(0x00);
	sta(0x02);
	jsr(P_fill_zone_51BB);
	jsr(P_4AFE);
	lda_imm(0x00);
	sta(0x96F3);
	lda(0x07);
	beq(B_5B48);
  
	lda(0x09);
	cmp_imm(0xA9);
	bcs(B_5B27);
  
	cmp_imm(0xA7);
	bcc(B_5B27);
  
	lda(0x08);
	cmp_imm(0x3F);
	bcc(B_5B27);
  
	jmpl(P_zorgon_ship_explodes_63A5);
  
label(B_5B27)
	rts;
  
label(B_5B28)
	lda_imm(0x01);
	sta(0x07);
	jmp(J_5AE6);
  
label(B_5B2F)
	lda(0x96F4);
	sta(0x00);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	lda_imm(0x2A);
	sta(0x03);
	lda_imm(0x83);
	sta(0x04);
	jsr(P_screen_copy_5172);
	rts;
  
label(B_5B48)
	sound_play("wall_break");
	lda(0x96F5);
	sec;
	sbc_imm(0xA1);
	sta(0x00);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
label(B_5B59)
	r.a = cpu_random_8_bit(); // wipe out the wall (zorgons) (lda(VIA_T1CL));
	adc(0x02);
	ora_imm(0x40);
	sta(0x02);
	lda_ind_Y(0x00);
	and(0x02);
	sta_ind_Y(0x00);
	tya;
	clc;
	adc_imm(0x28);
	tay;
	cmp_imm(0xF0);
	bne(B_5B59);
  
	lda(0x00);
	clc;
	adc_imm(0x01);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
label(B_5B80)
	lda(VIA_T1CL);
	adc(0x02);
	ora_imm(0x40);
	sta(0x02);
	lda_ind_Y(0x00);
	and(0x02);
	sta_ind_Y(0x00);
	tya;
	clc;
	adc_imm(0x28);
	tay;
	cmp_imm(0xF0);
	bne(B_5B80);
  
	lda(0x00);
	clc;
	adc_imm(0x01);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
label(B_5BA7)
	lda(VIA_T1CL);
	adc(0x02);
	ora_imm(0x40);
	sta(0x02);
	lda_ind_Y(0x00);
	and(0x02);
	sta_ind_Y(0x00);
	tya;
	clc;
	adc_imm(0x28);
	tay;
	cmp_imm(0xF0);
	bne(B_5BA7);
  
	rts;
  
}
end_subroutine

subroutine(P_5BC0)
{

	ldx_imm(0x00);
	lda_imm(0x00);
label(B_5BC4)
	sta_X(0x9000);
	dex;
	bne(B_5BC4);
  
	ldx_imm(0x10);
	lda_imm(0x00);
	sta(0x07);
	lda_imm(0x08);
	sta(0x08);
label(B_5BD4)
	lda(0x07);
	sta_X(0x9000);
	clc;
	adc_imm(0x15);
	sta(0x07);
	dex;
	lda_imm(0x00);
	sta_X(0x9000);
	sta(0x00);
	dex;
	lda(0x08);
	sta_X(0x9000);
	sta(0x01);
	clc;
	adc_imm(0x07);
	sta(0x08);
	dex;
	lda_imm(0xAC);
	sta_X(0x9000);
	sta(0x02);
	lda_imm(0x8F);
	sta(0x03);
	lda_imm(0x77);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x55);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(B_5BD4);
  
	rts;
  
}
end_subroutine

subroutine(P_move_zorgons_5C16)
{

	jsr(P_4C94);
	ldx_imm(0x10);
label(J_5C1B)
	inc_X(0x9000);
	lda_X(0x9000);
	cmp_imm(0x66);
	bcc(B_5C28);
  
	jmp(J_5C7D);
  
label(B_5C28)
	dex;
	inc_X(0x9000);
	lda_X(0x9000);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_5C6E);
  
	lda_imm(0x00);
	sta_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	clc;
	adc_imm(0x01);
	sta_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	adc_imm(0x00);
	sta_X(0x9000);
	sta(0x02);
label(J_5C53)
	lda_imm(0x8F);
	sta(0x03);
	lda_imm(0x77);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x55);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(J_5C1B);
  
	rts;
  
label(B_5C6E)
	dex;
	lda_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	sta(0x02);
	jmp(J_5C53);
  
label(J_5C7D)
	txa;
	pha;
	lda_imm(0x00);
	sta_X(0x9000);
	dex;
	sta_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	sta(0x01);
	clc;
	adc_imm(0xF7);
	sta_X(0x9000);
	dex;
	lda_X(0x9000);
	sta(0x02);
	adc_imm(0x00);
	sta_X(0x9000);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x55);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pla;
	tax;
	dex;
	lda_X(0x9000);
	sta(0x00);
	dex;
	lda_X(0x9000);
	sta(0x01);
	dex;
	lda_X(0x9000);
	sta(0x02);
	lda_imm(0x8F);
	sta(0x03);
	lda_imm(0x77);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x55);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bmi(B_5CE1);
  
	jmp(J_5C1B);
  
label(B_5CE1)
	rts;
  
}
end_subroutine

subroutine(P_draw_mothership_5CE2)
{

    jsr(P_draw_mothership_top_5D62);

/* draw mothership wings (green) and top (purple) */
    lda_imm(0x00);
    sta(0x00);
    move16(0xA28A,0x01); // destination
    move16(0x8405,0x03); // source

    lda_imm(0x13);
    sta(0x05);
    lda_imm(0xF7);
    sta(0x06);
    jsr(P_screen_copy_5172);

    // draw red part 1
	
    lda_imm(0x00);
    sta(0x00);
move16(0xA492,0x01);
move16(0x84FC,0x3);
    lda_imm(0x13);
    sta(0x05);
    lda_imm(0xF7);
    sta(0x06);
    jsr(P_screen_copy_5172);


    lda_imm(0x00);
    sta(0x00);
move16(0xA69A,0x1);
move16(0x85F3,0x3);
    lda_imm(0x13);
    sta(0x05);
    lda_imm(0xF7);
    sta(0x06);
    jsr(P_screen_copy_5172);
    
    lda_imm(0x00);
    sta(0x00);
    lda_imm(0xA2);
    sta(0x01);
    lda_imm(0xA8);
    sta(0x02);
    lda_imm(0xEA);
    sta(0x03);
    lda_imm(0x86);
    sta(0x04);
    lda_imm(0x13);
    sta(0x05);
    lda_imm(0x4C);
    sta(0x06);
    jsr(P_screen_copy_5172);
    rts;

}
end_subroutine

subroutine(P_draw_mothership_top_5D62)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xC9);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	lda_imm(0x48);
	sta(0x03);
	lda_imm(0x87);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xCD);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	lda_imm(0x54);
	sta(0x03);
	lda_imm(0x87);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_5DA1)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x8A);
	sta(0x01);
	lda_imm(0xA2);
	sta(0x02);
	lda_imm(0x8E);
	sta(0x03);
	lda_imm(0x78);
	sta(0x04);
	lda_imm(0x13);
	sta(0x05);
	lda_imm(0xF7);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x92);
	sta(0x01);
	lda_imm(0xA4);
	sta(0x02);
	lda_imm(0x85);
	sta(0x03);
	lda_imm(0x79);
	sta(0x04);
	lda_imm(0x13);
	sta(0x05);
	lda_imm(0xF7);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x9A);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x7C);
	sta(0x03);
	lda_imm(0x7A);
	sta(0x04);
	lda_imm(0x13);
	sta(0x05);
	lda_imm(0x4C);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_5DFF)
{

	lda_imm(0x02);
	sta(0x90D0);
	lda_imm(0x00);
	sta(0x90D1);
	sta(0x00);
	lda_imm(0x3F);
	sta(0x90D2);
	sta(0x01);
	lda_imm(0xA7);
	sta(0x02);
	lda_imm(0xC8);
	sta(0x03);
	lda_imm(0x7A);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_animate_zorgon_shots_5E2A)
{

	lda(0x90D0);
	cmp_imm(0x02);
	beq(B_5E6B);
  
	dec(0x90D1);
	lda(0x90D1);
	sta(0x00);
	bpl(B_5E92);
  
	lda_imm(0x02);
	sta(0x90D1);
	sta(0x00);
	dec(0x90D2);
	lda(0x90D2);
	sta(0x01);
	cmp_imm(0x3F);
	bcc(B_GEN_8664);
	jmpl(P_5E53);
label(B_GEN_8664)
  
	lda_imm(0x02);
	sta(0x90D0);
	jsr(P_5E53);
	rts;
	
label(B_5E6B)
	inc(0x90D1);
	lda(0x90D1);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_5E92);
  
	lda_imm(0x00);
	sta(0x90D1);
	sta(0x00);
	inc(0x90D2);
	lda(0x90D2);
	sta(0x01);
	cmp_imm(0x46);
	bcc(B_5E91);
  
	lda_imm(0x01);
	sta(0x90D0);
label(B_5E91)
  
	jmps(P_5E53);
  
label(B_5E92)
	lda(0x90D2);
	sta(0x01);
	jmps(P_5E53);

}
end_subroutine

subroutine(P_5E53)
{

	lda_imm(0xA7);
	sta(0x02);
	lda_imm(0xC8);
	sta(0x03);
	lda_imm(0x7A);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
  
}
end_subroutine

subroutine(P_decide_zorgon_shoot_5e9a)
{

	ldx_imm(0x10);
	
	label(B_5E9C)
	
	lda(VIA_T1CL);  // random shoot
	cmp_imm(0x0A);
	bcs(B_5EA6);
  
	jsr(P_zorgon_shoot_5ead);
label(B_5EA6)
	dex;
	dex;
	dex;
	dex;
	bpl(B_5E9C);
  
	rts;
  
}
end_subroutine

subroutine(P_zorgon_shoot_5ead)
{

	ldy_imm(0x08);
	txa;
	pha;
label(B_5EB1)
	lda_Y(0x9020);
	beq(B_5EBF);
  
	dey;
	dey;
	dey;
	dey;
	bpl(B_5EB1);
  
	pla;
	tax;
	rts;
  
label(B_5EBF)
	lda_imm(0x01);
	sta_Y(0x9020);
	dex;
	dey;
	lda_X(0x9000);
	sta_Y(0x9020);
	dex;
	dey;
	lda_X(0x9000);
	clc;
	adc_imm(0xAA);
	sta_Y(0x9020);
	dex;
	dey;
	lda_X(0x9000);
	adc_imm(0x02);
	sta_Y(0x9020);
	pla;
	tax;
	rts;
  
}
end_subroutine

subroutine(P_animate_zorgon_ship_5EE4)
{

	ldx_imm(0x08);
label(B_5EE6)
	lda_X(0x9020);
	bne(B_5EF2);
  
	dex;
	dex;
	dex;
	dex;
	bpl(B_5EE6);
  
	rts;
  
label(B_5EF2)
	dex;
	lda_X(0x9020);
	sta(0x00);
	dex;
	lda_X(0x9020);
	clc;
	adc_imm(0x50);
	sta_X(0x9020);
	sta(0x01);
	dex;
	lda_X(0x9020);
	adc_imm(0x00);
	sta_X(0x9020);
	sta(0x02);
	cmp_imm(0xBC);
	bcs(B_5F2E);
  
	lda_imm(0x2A);
	sta(0x03);
	lda_imm(0x8A);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	txa;
	pha;
	jsr(P_screen_copy_5172);
	pla;
	tax;
	dex;
	bpl(B_5EE6);
  
	rts;
  
label(B_5F2E)
	txa;
	pha;
	inx;
	inx;
	inx;
	lda_imm(0x00);
	sta_X(0x9020);
	dex;
	lda(0x01);
	sec;
	sbc_imm(0x50);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	pla;
	tax;
	dex;
	bpl(B_5EE6);
  
	rts;
  
}
end_subroutine

subroutine(P_5F5B)
{

	lda(0x90E0);
	beq(B_5F79);
  
	lda(0x90E2);
	sta(0x01);
	lda(0x90E3);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x34);
	sta(0x06);
	jsr(P_fill_zone_51BB);
label(B_5F79)
	lda_imm(0x0F);
	sta(0x90E0);
	lda(0x96F4);
	sta(0x90E1);
	lda(0x96F5);
	sec;
	sbc_imm(0xF2);
	sta(0x90E2);
	lda(0x96F6);
	sbc_imm(0x00);
	sta(0x90E3);
	rts;
  
}
end_subroutine

subroutine(P_anim_foe_death_5F96)
{

	lda(0x90E0);
	beq(B_5FC3);
  
	dec(0x90E0);
	beq(B_5FC4);
  
	lda(0x90E1);
	sta(0x00);
	lda(0x90E2);
	sta(0x01);
	lda(0x90E3);
	sta(0x02);
	lda_imm(0xEC);
	sta(0x03);
	lda_imm(0x7A);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x34);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
label(B_5FC3)
	rts;
  
label(B_5FC4)
	lda(0x90E2);
	sta(0x01);
	lda(0x90E3);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x34);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	rts;
  
}
end_subroutine

subroutine(P_update_player_status_5FDE)
{
    ldx_imm(0x05);
    label(B_5FE0)
    lda_X(score_9610);
    ora_imm(0x30);
    sta_X(0xBF92);
    dex;
    bpl(B_5FE0);
  
    ldx_imm(0x05);
    label(B_5FED)
    lda_X(score_9610);
    ora_imm(0x30);
    sta_X(0xBF92);
    dex;
    bpl(B_5FED);
    
    // display number of lives
    lda(nb_lives_9616);
    ora_imm(0x30);
    sta(0xBFA3);
    
    lda(skill_level_9619);
    ora_imm(0x30);
    sta(0xBFC7);
    lda(0x9617);
    ora_imm(0x30);
    sta(0xBFD4);
    lda(0x9618);
    ora_imm(0x30);
    sta(0xBFD5);
    ldx_imm(0x06);
    ldy_imm(0x00);
    label(B_601C)
    lda_X(0x6735);
    ora_imm(0x30);
    sta_Y(0xBFAF);
    iny;
    dex;
    bne(B_601C);
  
    rts;
  
}
end_subroutine

subroutine(P_lose_a_life_6029)
{

	dec(nb_lives_9616);
	beq(B_6033);
  
	lda_imm(0x00);
	sta(0x041F);
label(B_6033)
	jsr(P_update_player_status_5FDE);
	rts;
  
}
end_subroutine

subroutine(P_6037)
{

	lda(0x96E3);
	bne(B_6038);
	jmpl(P_6090);
label(B_6038)
  
	dec(0x96E0);
	bne(B_6063);
  
	lda_imm(0x07);
	sta(0x96E0);
	lda(0x96E1);
	cmp_imm(0x8A);
	beq(B_6059);
  
	lda_imm(0x8A);
	sta(0x96E1);
	lda_imm(0x87);
	sta(0x96E2);
	bne(B_6063);
  
label(B_6059)
	lda_imm(0xDA);
	sta(0x96E1);
	lda_imm(0x88);
	sta(0x96E2);
label(B_6063)
	lda(0x96EF);
	sta(0x00);
	lda(0x96F0);
	clc;
	adc_imm(0xF8);
	sta(0x01);
	lda(0x96F1);
	adc_imm(0x02);
	sta(0x02);
	lda(0x96E1);
	sta(0x03);
	lda(0x96E2);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	sta(0x96E4);
	lda_imm(0x38);
	sta(0x06);
	jsr(P_screen_copy_5172);
	rts;
  
}
end_subroutine

subroutine(P_6090)
{

	lda(0x96E4);
	beq(B_60B9);
  
	lda(0x96F0);
	clc;
	adc_imm(0xF8);
	sta(0x01);
	lda(0x96F1);
	adc_imm(0x02);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x3C);
	sta(0x06);
	jsr(P_fill_zone_51BB);
	lda_imm(0x00);
	sta(0x96E4);
	rts;
  
label(B_60B9)
	ldx_imm(0x64);
label(B_60BB)
	dex;
	bne(B_60BB);
  
	rts;
  
}
end_subroutine

subroutine(P_60BF)
{

	lda_imm(0x00);
	ldx_imm(0x00);
label(B_60C3)
	sta_X(0x9000);
	dex;
	bne(B_60C3);
  
	sta(0x9609);
	lda_imm(0x00);
	sta(nb_kamikaze_birds_96fe);
	lda_imm(0xEF);
	sta(0x9000);
	lda_imm(0x00);
	sta(0x9003);
	lda_imm(0xFE);
	sta(0x9002);
	lda_imm(0xAD);
	sta(0x9001);
	lda_imm(0x02);
	sta(0x9004);
	lda_imm(0x77);
	sta(0x9005);
	lda_imm(0x00);
	sta(0x9008);
	lda_imm(0xF5);
	sta(0x9007);
	lda_imm(0xAD);
	sta(0x9006);
	lda_imm(0x02);
	sta(0x9009);
	lda_imm(0xB3);
	sta(0x900A);
	lda_imm(0x00);
	sta(0x900D);
	lda_imm(0x32);
	sta(0x900C);
	lda_imm(0xB2);
	sta(0x900B);
	lda_imm(0x02);
	sta(0x900E);
	lda_imm(0x3B);
	sta(0x900F);
	lda_imm(0x00);
	sta(0x9012);
	lda_imm(0xC2);
	sta(0x9011);
	lda_imm(0xA9);
	sta(0x9010);
	lda_imm(0x02);
	sta(0x9013);
	lda_imm(0x02);
	sta(0x0400);
	sta(0x0401);
	lda_imm(0xFA);
	sta(0x0404);
	lda_imm(0x01);
	sta(0x041E);
	lda_imm(0xFF);
	sta(0x041D);
	lda_imm(0x00);
	sta(0x9609);
	rts;
  
}
end_subroutine

subroutine(P_6152)
{
    // parachute?
	lda_imm(0x00);
	sta(number_of_foes_961f);
	lda(0x9004);
	sta(0x961B);
	lda(0x9009);
	sta(0x961C);
	lda(0x900E);
	sta(0x961D);
	lda(0x9013);
	sta(0x961E);
	ldx_imm(0x00);
	lda_imm(0x00);
label(B_6173)
	sta_X(0x9000);
	dex;
	bne(B_6173);
  
	sta(0x9609);
	lda(0x961B);
	beq(B_61AF);
  
	lda_imm(0xEF);
	sta(0x9000);
	lda_imm(0x00);
	sta(0x9003);
	lda_imm(0xFE);
	sta(0x9002);
	lda_imm(0xAD);
	sta(0x9001);
	lda(0x961B);
	sta(0x9004);
	one_foe_spawned();
	cmp_imm(0x01);
	beq(B_61AF);
  
	lda(stage_9600);
	cmp_imm(0x01);
	beq(B_61AF);
  
	one_foe_spawned();
	one_foe_spawned();
label(B_61AF)
	lda(0x961C);
	beq(B_61E2);
  
	lda_imm(0x77);
	sta(0x9005);
	lda_imm(0x00);
	sta(0x9008);
	lda_imm(0xF5);
	sta(0x9007);
	lda_imm(0xAD);
	sta(0x9006);
	lda(0x961C);
	sta(0x9009);
	one_foe_spawned();
	cmp_imm(0x01);
	beq(B_61E2);
  
	lda(stage_9600);
	cmp_imm(0x01);
	beq(B_61E2);
  
	one_foe_spawned();
	one_foe_spawned();
label(B_61E2)
	lda(0x961D);
	beq(B_6215);
  
	lda_imm(0xB3);
	sta(0x900A);
	lda_imm(0x00);
	sta(0x900D);
	lda_imm(0x32);
	sta(0x900C);
	lda_imm(0xB2);
	sta(0x900B);
	lda(0x961D);
	sta(0x900E);
	one_foe_spawned();
	cmp_imm(0x01);
	beq(B_6215);
  
	lda(stage_9600);
	cmp_imm(0x01);
	beq(B_6215);
  
	one_foe_spawned();
	one_foe_spawned();
label(B_6215)
	lda(0x961E);
	beq(B_6248);
  
	lda_imm(0x3B);
	sta(0x900F);
	lda_imm(0x00);
	sta(0x9012);
	lda_imm(0xC2);
	sta(0x9011);
	lda_imm(0xA9);
	sta(0x9010);
	lda(0x961E);
	sta(0x9013);
	one_foe_spawned();
	cmp_imm(0x01);
	beq(B_6248);
  
	lda(stage_9600);
	cmp_imm(0x01);
	beq(B_6248);
  
	one_foe_spawned();
	one_foe_spawned();
label(B_6248)
	lda_imm(0x02);
	sta(0x0400);
	sta(0x0401);
	lda_imm(0xFA);
	sta(0x0404);
	lda_imm(0x01);
	sta(0x041E);
	lda_imm(0xFF);
	sta(0x041D);
	lda_imm(0x00);
	rts;
  
}
end_subroutine

subroutine(P_6262)
{

	lda(0xA832);
	cmp_imm(0x07);
	beq(B_626D);
  
	lda_imm(0x07);
	bne(B_626F);
  
label(B_626D)
	lda_imm(0x04);
label(B_626F)
	sta(0xA832);
	sta(0xA85A);
	sta(0xA882);
	lda(0x96E6);
	sta(0x00);
	lda_imm(0x5C);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x36);
	sta(0x03);
	lda_imm(0x87);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda(0x96E6);
	clc;
	adc(0x96E7);
	sta(0x96E6);
	cmp_imm(0x02);
	bne(B_62AD);
  
	lda_imm(0xFF);
	sta(0x96E7);
	bne(B_62B6);
  
label(B_62AD)
	cmp_imm(0x00);
	bne(B_62B6);
  
	lda_imm(0x01);
	sta(0x96E7);
label(B_62B6)
	lda(0x96E8);
	sta(0x00);
	lda_imm(0x6B);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x36);
	sta(0x03);
	lda_imm(0x87);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda(0x96E8);
	clc;
	adc(0x96E9);
	sta(0x96E8);
	cmp_imm(0x02);
	bne(B_62EB);
  
	lda_imm(0xFF);
	sta(0x96E9);
	bne(B_62F4);
  
label(B_62EB)
	cmp_imm(0x00);
	bne(B_62F4);
  
	lda_imm(0x01);
	sta(0x96E9);
label(B_62F4)
	lda(0x96EA);
	sta(0x00);
	lda_imm(0xCB);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	lda_imm(0x5A);
	sta(0x03);
	lda_imm(0x87);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda(0x96EA);
	clc;
	adc(0x96EB);
	sta(0x96EA);
	cmp_imm(0x03);
	bne(B_6329);
  
	lda_imm(0xFF);
	sta(0x96EB);
	bne(B_6332);
  
label(B_6329)
	cmp_imm(0x00);
	bne(B_6332);
  
	lda_imm(0x01);
	sta(0x96EB);
label(B_6332)
	rts;
  
}
end_subroutine

subroutine(P_6333)
{

	lda(0x96EF);
	sta(0x00);
	lda(0x96F0);
	sta(0x01);
	sta(0x07);
	lda(0x96F1);
	sta(0x02);
	sta(0x08);
	lda_imm(0xD2);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x48);
	sta(0x06);
	jsr(P_screen_copy_5172);
	lda_imm(0x0A);
	sta(0x03);
label(B_635D)
	lda(0x07);
	sta(0x00);
	lda(0x08);
	sta(0x01);
	ldy_imm(0x04);
	ldx_imm(0x12);
	OricTimer tmr = timer_init(3,20);
	sound_play("yell");
label(B_6369)
r.a = cpu_random_8_bit(); // wipe out the man (lda(VIA_T1CL));
	adc(0x02);
	ora_imm(0x40);
	sta(0x02);

	timer_tick(&tmr);
	
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	bcc(B_6388);
  
	and(0x02);
	sta_ind_Y(0x00);
label(B_6388)
	dey;
	bne(B_6369);
  
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x04);
	jsr(P_4C95);
	dex;
	bpl(B_6369);
  
	dec(0x03);
	bne(B_635D);
  
	rts;
  
}
end_subroutine

subroutine(P_zorgon_ship_explodes_63A5)
{
    sound_play("metallic_crash");
	lda_imm(0x0A);
	sta(0x04);
label(B_63A9)
	lda_imm(0xA2);
	sta(0x01);
	lda_imm(0x80);
	sta(0x00);
	lda_imm(0xA8);
	sta(0x03);
	lda_imm(0xE8);
	sta(0x02);
	jsr(P_63D7);

  
	dec(0x04);
	bne(B_63A9);

	// end of zorgon level
	
	poke_41F_80();
	
	lda_imm(0xFF);
	sta(0x01);
	jsr(P_63D7);
	rts;
  
}
end_subroutine

subroutine(P_63D7)
{

	lda(0x01);
	cmp_imm(0xFF);
	beq(B_6409);
  
	lda(0x00);
	sta(0x05);
	lda(0x01);
	sta(0x06);
	ldy_imm(0x00);

	
       label(B_63E7)
       
       r.a = cpu_random_8_bit();       
       //lda(VIA_T1CL);   // was used for random number generation
       
	adc(0x02);
	ora_imm(0x40);
	sta(0x02);
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	bcc(B_63FA);      
	
	and(0x02);
	sta_ind_Y(0x00);
label(B_63FA)
	dey;
	bne(B_63E7);
	
	SDL_Delay(20);
	
	jsr(P_4CEB);
	inc(0x01);
	lda(0x01);
	cmp(0x03);
	bne(B_63E7);
  
	rts;
  
label(B_6409)
	ldy_imm(0x00);
label(B_640B)
	lda_imm(0x00);
	sta_ind_Y(0x05);
	dey;
	bne(B_640B);
  
	inc(0x06);
	lda(0x06);
	cmp(0x03);
	bne(B_640B);
  
	rts;
  
}
end_subroutine

subroutine(P_increase_score_641B)
{

	pha;
	txa;
	pha;
	tya;
	pha;
	php;
	ldx_imm(0x04);
label(B_6423)
	lda_X(score_9610);
	clc;
	adc_X(0x961A);
	cmp_imm(0x0A);
	bcs(B_643E);
  
label(J_642E)
	sta_X(score_9610);
	dex;
	bne(B_6423);
  
	jsr(P_update_player_status_5FDE);
	plp;
	pla;
	tay;
	pla;
	tax;
	pla;
	rts;
  
label(B_643E)
	sbc_imm(0x0A);
	dex;
	inc_X(score_9610);
	inx;
	jmp(J_642E);
  
	
}
end_subroutine

