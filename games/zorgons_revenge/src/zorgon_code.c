// converted by asm2c (C) JOTD 2008

#include "oric_timer.h"
#include "opcodes_legacy.h" // 2008 asm conversions use this file
#include "zorgon_custom.h"
#include "constants.h"
#include "zorgon_code.h"
#include "oric_input.h"
#include "oric_sound.h"
#include "oric_basic.h"

#define crosser_bit_position_13DA 0x13DA
#define crosser_byte_position_13D9 0x13D9
#define crosser_direction_13DB 0x13DB
#define jump_flag_13E4 0x13E4
#define jump_counter_13E0 0x13E0
#define number_of_lives_9616 0x9616
#define hero_pos_13D4 0x13D4
#define hero_pos_13D4_plus_1 0x13D5
#define laser_beam_index_3E9F 0x3E9F
#define elevator_direction_13D2 0x13D2
#define materialize_bridge_part_flag_617A 0x617A


#define LOOP_STOP(sid) sound_stop(sid##_sound_id); sid##_sound_id = -1
#define LOOP_PLAY(sname) if (sname##_sound_id == -1) sname##_sound_id = sound_play(#sname)

// for loops/samples that we need to interrupt

int falling_sound_id = -1;
int laser_hum_sound_id = -1;
int lower_bridge_sound_id = -1;
int run_sound_id = -1;
int elevator_sound_id = -1;
int laser_bridge_sound_id = -1;

// trainer
//#define START_AT_LAST_STAGE
//#define NO_RANDOM_LEVELS
//#define VERY_EASY_GAME
//#define START_AT_BRIDGE_LEVEL

void P_test_jump_height();
void P_draw_river_crosser_1005();
void J_last_level_0981();
void J_1967();
void J_0A54();
void J_1A10();
void P_blit_to_screen_5285();
void P_clear_text_screen_5A57();
void P_select_mission_screen_6429();
void J_09DE();
void J_0F7F();
void J_0E90();
void J_0BF1();
void J_1B22();
void J_1DAB();
void J_24CD();
void B_2A04();
void J_4377();
void B_handle_ship_disappearing_2C5B();
void B_2C81();
void J_2EB7();
void J_2F90();
void J_3053();
void J_2F04();
void J_3120();
void J_3A3A();
void J_4D61();
void J_37D5();
void J_076C();
void J_5F25();
void P_5305(int to_xx);
void P_quadnog_red_and_white_shots_37EA(int to_3865,int to_3891);
void J_3A0E();
void J_4D9C();
void J_4E8B();
void J_4E4A();
void J_5756();
void J_58BE();
void J_378E();
void B_523E();
void J_37CD();
void E_0732();
void P_hero_crosses_screen_66C8();
void P_move_hero_left_0F36();
void P_test_fire_65C8();
void P_main_menu_59D4();
void P_select_mission_6477();
void P_delay_099F();
void P_set_hero_initial_pos_09B2();
void P_09F2();
void P_move_elevator_0A36();
void P_0AE6();
void P_0B29();
void P_handle_jump_0C22();
void P_jump_up_0D70();
void P_0D8A();
void P_jump_down_0DA0();
void P_0DD5(int goto_0E24);
void P_0E58();
void P_man_on_elevator_0EB5();
void P_move_hero_right_0EEA();
void P_0FA0();
void P_move_river_crosser_0FB0();
void P_104C();
void P_randomize_levels_110A();
void P_1138();
void P_1150();
void P_119F();
void P_11BF();
void P_11E2();
void P_1203();
void P_1285();
void P_deadly_fall_12A7();
void P_1316();
void P_1345();
void P_1360();
void P_fall_sound_139B();
void P_13B8();
void P_spider_level_1400();
void P_161A();
void P_1621();
void P_1748();
void P_178E();
void P_17CB();
void P_17DD();
void P_180C();
void P_181F();
void P_1858();
void P_detect_spider_button_186C();
void P_cut_monster_laser_beam_18A6();
void P_handle_hero_on_rope_18DA();
void P_1930();
void P_197F();
void P_19E9();
void P_1A26();
void P_1A47();
void P_1A7C();
void P_1A89();
void P_1ABC();
void P_1AE3();
void P_1B0A();
void P_1B36();
void P_draw_laser_cage_1B8F();
void P_1BFE();
void P_1CE5();
void P_1CF1();
void P_1D06();
void P_1D1E();
void P_1D3B();
void P_1D50();
void P_1D79();
void P_1DC3();
void P_bridge_level_1F00();
void P_bridge_button_test_2037();
void P_2044();
void P_2265();
void P_22B1();
void P_23CE();
void P_240F();
void P_2444();

// controle a finir
void P_lower_bridge_part_247C();
void P_24B9();
void P_24E1();
void P_250C();
void P_2524();
void P_handle_bird_stones_2544();
void P_animate_bird_stones_25BD();
void P_2649();
void P_26A0();
void P_move_attack_birds_26B4();
void P_ship_level_2800();
void P_handle_shoot_29C0();
void P_2B8E();
void P_2BB4();
void P_2C0D();
void P_2C38();
void P_move_and_display_ship_2CAB();
void P_ship_collision_detection_2D28();
void P_2D93();
void P_2DF2();
void P_2E8E();
void P_2EF5();
void P_2F8E();
void P_308F();
void P_30EF();
void P_3134();
void P_31FB();
void P_321E();
void P_325E();
void P_3281();
void P_32AC();
void P_32CB();
void P_32FF();
void P_reptile_level_1400();
void P_3631();
void P_3658();
void P_3676();
void P_3683();
void P_36A7();
void P_36B4();
void P_36CC();
void P_36E4();
void P_375E();
void P_quadnog_red_and_white_shots_37EA();
void P_3865();
void P_38CF();
void P_destroy_quadnog_3900();
void P_3935();
void P_quadnog_green_shots_39D5();
void P_3A07();
void P_3A98();
void P_3AEF();
void P_3B20();
void P_3B51();
void P_3B6D();
void P_3BCF();
void P_erase_beam_3C10();
void P_3C44();
void P_3C7E();
void P_3CC4();
void P_3D0A();
void P_3D19();
void P_3D5F();
void P_3E69();
void P_bird_level_3F00();
void P_bird_level_specifics_4099();
void P_40A6();
void P_40E0();
void P_412B();
void P_416A();
void P_catch_bird_test_4178();
void P_41EB();
void P_anim_color_bridge_422F();
void P_4269();
void P_animate_bird_429A(int *noise_count);
void P_4340();
void P_439C();
void P_create_falling_stones_4468();
void P_raining_stones_4499();
void P_zorgon_king_level_4600();
void P_king_button_test_48A0();
void P_48AC();
void P_48E4();
void P_4909();
void P_4BF9();
void P_4C3F();
void P_handle_ant_stings_4C57();
void P_handle_ant_moves_4CF6();
void P_4D5A();
void P_animate_zorgon_king_4D83();
void P_4DD8();
void P_init_shoot_left_4E2D();
void P_4E6E();
void P_anim_right_diag_shoot_4EAF();
void P_4F14();
void P_anim_left_diag_shoot_4F4E();
void P_4FB3();
void P_king_falls_4FF7();
void P_5066();
void P_sound_routine_50AD();
void P_50C3();
void P_50E0();
void P_5285();
void P_draw_rectangle_52CE();
void P_5305();
void P_5356();
void P_5385();
void P_53BF();
void P_screen_routine_53D5();
void P_541C();
void P_5432();
void P_sound_routine_5456();
void P_sound_routine_5482();
void P_54AC();
void P_54C1();
void P_init_hiscores_54C8();
void P_init_54EB();
void P_display_bridge_part_5501();
void P_materialize_bridge_part_5521();
void P_5578();
void P_55D3();
void P_update_player_status_5614();
void P_564F();
void P_5686();
void P_56B9();
void P_56C7();
void P_576A();
void P_5787();
void P_57A0();
void P_5824();
void P_591C();
void P_5930();
void P_593C();
void P_5948();
void P_animate_quadnog_1_5954();
void P_5977();
void P_59B3();
void P_59D4();
void P_animate_menu_5A0C();
void P_5A57();
void P_5A9D();
void P_5B19();
void P_enter_hiscore_5B90();
void P_enter_hiscore_letter_5C74();
void P_decode_hiscore_entry_5CEF();
void P_5D07();
void P_5DA0();
void P_5E4E();
void P_adjust_skill_5E57();
void P_5E9D();
void P_5EF8();
void P_5FCD();
void P_read_keyboard_matrix_5FDE();
void P_5FEE();
void P_6027();
void P_6200();
void P_6250();
void P_62BA();
void P_6357();
void P_63F6();
void P_6429();
void P_6477();
void P_6500();
void P_65C8();
void P_65ED();
void P_6628();
void P_6653();
void P_6699();
void P_66C8();
void P_66DD();
void P_66FD();
void P_6749();
void P_6778();
void P_94B7();

static void stop_loop_sounds()
{
	LOOP_STOP(run);
	LOOP_STOP(falling);
	LOOP_STOP(laser_hum);
	LOOP_STOP(lower_bridge);
	LOOP_STOP(elevator);
	LOOP_STOP(laser_bridge);
}

static inline void P_handle_hero()
{
    jsr(P_0B29);
    jsr(P_1345);
    jsr(P_1150);
}


typedef struct
{
    int esc_pressed;
    int fire_pressed;
    int left_pressed;
    int right_pressed;
    int fire_was_pressed;
    int up_pressed;
    int down_pressed;
} Controls;

Controls controls;

static void play_kill_sound()
{

    LOOP_STOP(run);
    LOOP_STOP(falling);

    const char *to_play = "ouch";

    int rn = cpu_random_8_bit();
    if (rn > 512/3)
    {
	to_play = "male_scream";
    }
    else if (rn > 256/3)
    {
	to_play = "male_scream_2";
    }
    
    sound_play(to_play);
    
}

static int handle_escape()
{
    int rval = controls.esc_pressed;

    if (rval)
    {
	poke(0x41F,0);
	stop_loop_sounds();
    }

    return rval;
}

static void move_ff_41f()
{
    
    lda_imm(0xFF);
    sta(0x041F);
}


static void set_jump_flag(int value)
{
    switch (value)	
    {
      case 0:
	LOOP_STOP(falling);
	break;
      case 1:
 	LOOP_STOP(run);
	sound_play("jump_up");
	break;
      case 2:
	sound_play("hop_elevator");	

	break;
      case 3:
	// hanging on the moving rope (spider)
	break;
      default:
	printf("value %d jum pflag\n",value);fflush(stdout);
	break;
    }
    poke(jump_flag_13E4,value);
}

static void update_controls()
{
    
    SDL_PumpEvents();

    const Uint8 *keys = get_keyboard_state();

    controls.right_pressed = keys[SDLK_RIGHT];
    controls.left_pressed = keys[SDLK_LEFT];
    controls.up_pressed = keys[SDLK_UP];
    controls.down_pressed = keys[SDLK_DOWN];
    controls.fire_was_pressed = controls.fire_pressed;
    controls.fire_pressed = keys[SDLK_RCTRL] || keys[SDLK_LCTRL];
    controls.esc_pressed = keys[SDLK_ESCAPE];

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
    int vert = joystick_get_axis(0,1);
    
    if (vert > 16384)
    {
	controls.up_pressed = 1;
    }
    else if (vert < -16384)
    {
	controls.down_pressed = 1;
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
	      controls.fire_pressed = 1;
	      
	      break;
	  }
	}
    }
    
}

static const char *sounds[] =
{ "jump_up", "falling", "jump_down", "crash",
  "ouch", "male_scream", "male_scream_2", "female_scream", "hop_elevator",
  "quadnog_dies", "ship_appears", "hit_ceiling", "bridge_appears",
  "ship_explodes",  "deadly_beam",  "shoot", "cannon",
  "bird", "bridge_laid"  };

static void load_sounds()
{
    unsigned int i = 0;
    for (i = 0; i < (sizeof(sounds)/sizeof(char *)); i++)
    {
	sound_load("snd",sounds[i],"wav",0);
    }

    sound_load("snd","run","wav",1); // loops
    sound_load("snd","elevator","wav",1); // loops
    sound_load("snd","laser_hum","wav",1); // loops
    sound_load("snd","lower_bridge","wav",1); // loops
    sound_load("snd","laser_bridge","wav",1);
}


subroutine(J_0748)
{
	lda_imm(0x00);
	sta(0x021F);
	lda_imm(0x5A);
	sta(0x00);
	lda_imm(0x07);
	sta(0x01);
	lda_imm(0x01);
	sta(0x03);
	rts;
  

}
end_subroutine

subroutine(E_0713)
{
    // init sounds

    load_sounds();
    
    move16(0x732,0x405);

    lda_imm(0x0A);
    sta(0x026A);
    
    lda_imm(0x02);
    sta(0x041F);
    jsr(P_6500);
    lda(0x041F);
    cmp_imm(0x02);
    beq(out);
    jmps(E_0732);
out:
    rts;
}
end_subroutine

subroutine(E_0732)
{
	jsr(P_54C1);
	lda_imm(0x09);
	sta(0x9619);
	sta(0x0410);
	lda_imm(0x02);
	sta(0x04FD);
	sta(0x04FF);
	jsr(P_54AC);
	jsr(J_0748);
}
end_subroutine


void J_0769()
{
    P_init_54EB();
    /*ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);*/
	jmpl(J_076C);
}


subroutine(J_076C)
{
	lda_imm(0x00);
	ldx_imm(0x04);
B_0777:
	sta_X(0x61AE);
	dex;
	bpl(B_0777);
  
	sta(0x13F5);
	jsr(P_54AC);
	jsr(P_randomize_levels_110A);
	jsr(P_94B7);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x00);
	sta(0x04);
	sta(0x05);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	ldy_imm(0xFE);
	jsr(P_sound_routine_5456);
	lda_imm(0xB6);
	sta(0x00);
	lda_imm(0x07);
	sta(0x01);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x7B);
	sta(0x04);
	lda_imm(0x78);
	sta(0x05);
	lda_imm(0x26);
	sta(0x06);
	rts;
  

}
end_subroutine

subroutine(E_075A)
{

	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0x0A);
	sta(0x026A);
	jsr(P_main_menu_59D4);
	jsr(J_0769);
}
end_subroutine

  
subroutine(J_08B2)
{
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x00);
	sta(0x04);
	sta(0x05);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	ldy_imm(0xFE);
	jsr(P_sound_routine_5456);
	lda_imm(0xDF);
	sta(0x00);
	lda_imm(0x08);
	sta(0x01);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0xC7);
	sta(0x04);
	lda_imm(0x78);
	sta(0x05);
	lda_imm(0x18);
	sta(0x06);
	rts;
  

}
end_subroutine

subroutine(E_07B6)
{

	lda_imm(0xC8);
	sta(0x00);
	lda_imm(0x07);
	sta(0x01);
	lda_imm(0x02);
	sta(0x03);
	lda_imm(0x01);
	sta(0x021F);
	rts;
}
end_subroutine

subroutine(E_07C8)
{
	LOOP_STOP(run);
	
	lda_imm(0x0A);
	sta(0x026A);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0x00);
	sta(0x041F);
	jsr(P_6200);
	lda(0x041F);
	bpl(B_07E4);
  
	jmps(J_0748);
  
	B_07E4:
#ifdef START_AT_BRIDGE_LEVEL
	jmp(B_bridge_level);
#endif
#ifdef START_AT_LAST_STAGE
	jmps(J_last_level_0981);
#endif
#ifndef NO_RANDOM_LEVELS
	oric_wait(300);
#endif
	lda(0x0411);
	cmp_imm(0x01);
	bne(B_07F7);

	// level
	jsr(P_spider_level_1400);
	jmp(B_bridge_test);
  
B_07F7:
	lda(0x0411);
	cmp_imm(0x02);
	bne(B_080A);
  
	jsr(P_ship_level_2800);

B_bridge_test:	
	LOOP_STOP(run);
	
	lda(0x041F);
	cmp_imm(0x40);
	
	beq(B_goto_bridge_0849);
  
	bne(B_next_level_0829);
  
B_080A:
	lda(0x0411);
	cmp_imm(0x03);
	
	bne(B_081D);
  
	jsr(P_reptile_level_1400);	
	jmp(B_bridge_test)
  
	B_081D:
	
	jsr(P_bird_level_3F00);
	jmp(B_bridge_test)
  
B_next_level_0829:
	lda(0x041F);
	beq(B_0846);
	
	dec(number_of_lives_9616);
	jsr(P_update_player_status_5614);
	lda(number_of_lives_9616);
	beq(J_08B2);
  
	jsr(P_delay_099F);
	jmp(E_07C8);
  
	B_0846:
	jmps(J_0748);
  
	B_goto_bridge_0849:

	
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda(0x041E);
	beq(B_085D);
  
	dec(number_of_lives_9616);
	beq(J_08B2);
  
	jsr(P_update_player_status_5614);
B_085D:
	lda(0x617E);
	sta(0x961C);
	lda(0x617F);
	sta(0x961D);
	lda(0x6180);
	sta(0x961E);
	jsr(P_564F);
	jsr(P_delay_099F);

B_bridge_level:
	jsr(P_bridge_level_1F00);
	lda(0x041F);
	cmp_imm(0x80);
	beq(B_next_level_0829);
  
	lda(0x617E);
	sta(0x961C);
	lda(0x617F);
	sta(0x961D);
	lda(0x6180);
	sta(0x961E);
	jsr(P_564F);
	jsr(P_delay_099F);
	lda(0x041F);
	cmp_imm(0x40);
	beq(B_last_level_08AF);
  
	ldx(0x0411);
	dex;
	lda_X(0x61B2);
	tax;
	dex;
	lda_imm(0xFF);
	sta_X(0x61AE);
	jmp(E_07C8);
  
B_last_level_08AF:
	jmps(J_last_level_0981);

J_08B2:
J_08B2();
}
end_subroutine

subroutine(E_08DF)
{

	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	jsr(P_delay_099F);
	lda_imm(0x00);
	sta(0x021F);
	lda_imm(0xFB);
	sta(0x00);
	lda_imm(0x08);
	sta(0x01);
	lda_imm(0x01);
	sta(0x03);
	rts;
  

}
end_subroutine

subroutine(E_08FB)
{

	lda_imm(0x0A);
	sta(0x026A);
	jsr(P_5A9D);
	lda(0x041F);
	bpl(B_GEN_1);
	jmps(J_0769);
B_GEN_1:
	bne(B_GEN_2);
	jmps(J_0748);
B_GEN_2:
	jsr(P_init_54EB);
	lda_imm(0x00);
	sta(0x041F);
	lda_imm(0x24);
	sta(0x00);
	lda_imm(0x09);
	sta(0x01);
	lda_imm(0x02);
	sta(0x03);
	lda_imm(0x01);
	sta(0x021F);
	rts;
  

}
end_subroutine

subroutine(E_0924)
{

    move16(0x937,0x405); // imm jump


	lda_imm(0x0A);
	sta(0x026A);
	
	jsr(P_6500);
	rts;
  

}
end_subroutine

subroutine(E_0937)
{

	lda(0x041F);
	beq(B_0941);
  
	bpl(B_0944);
  
	jmps(J_0748);
  
B_0941:
	jmps(J_0769);
  
B_0944:
	jmps(J_08B2);
  
J_0947:
	jmps(J_0748);
  
B_094A:
	dec(0x9619);
	bpl(B_0954);
  
	lda_imm(0x00);
	sta(0x9619);
B_0954:
	lda_imm(0x6C);
	sta(0x00);
	lda_imm(0x07);
	sta(0x01);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x00);
	sta(0x04);
	sta(0x05);
	jsr(P_sound_routine_5482);
	lda_imm(0x03);
	sta(0x03);
	ldx_imm(0x07);
	ldy_imm(0xFE);
	jsr(P_sound_routine_5456);
	lda_imm(0xD5);
	sta(0x04);
	lda_imm(0x77);
	sta(0x05);
	lda_imm(0x23);
	sta(0x06);
	rts;
}
end_subroutine

subroutine(J_last_level_0981)
{
    	set_game_context(GAME_ZORGON);

	jsr(P_zorgon_king_level_4600);
	
	lda(0x041F);
	cmp_imm(0x40);
	beq(B_094A);
  
	if (peek(0x41F) == 0)
	{
	    // ESC
	    jmp(B_099C);
	}
	dec(number_of_lives_9616);
	jsr(P_update_player_status_5614);
	lda(number_of_lives_9616);
	beq(B_099C);
  
	jsr(P_delay_099F);
	jmp(J_last_level_0981);


B_099C:
	jmps(J_08B2);
// duplicated
B_094A:
	// end of level
	dec(0x9619);
	bpl(B_0954);
  
	lda_imm(0x00);
	sta(0x9619);
B_0954:
	lda_imm(0x6C);
	sta(0x00);
	lda_imm(0x07);
	sta(0x01);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x00);
	sta(0x04);
	sta(0x05);
	jsr(P_sound_routine_5482);
	lda_imm(0x03);
	sta(0x03);
	ldx_imm(0x07);
	ldy_imm(0xFE);
	jsr(P_sound_routine_5456);
	lda_imm(0xD5);
	sta(0x04);
	lda_imm(0x77);
	sta(0x05);
	lda_imm(0x23);
	sta(0x06);
	rts;
  
}
end_subroutine

subroutine(P_delay_099F)
{
    oric_wait(200);
}
end_subroutine

subroutine(P_set_hero_initial_pos_09B2)
{

	lda_imm(0x02);
	sta(0x0402);
	lda_imm(0x00);
	sta(0x13DC);
	lda_imm(0x19);
	sta(0x13D6);
	lda_imm(0x01);
	sta(0x13D8);
	lda_imm(0x00);
	sta(0x00);
	sta(0x13D3);
	
	set_jump_flag(0);

	// hero start location
	// alias hero initial position
	
	int pos_addr = 0xB9F0;

	// pos_addr += 0x20;  // fast bird catch
	// pos_addr -= (70*0x28); // fast bird fall
	//§pos_addr -= (113*0x28) - 10; // top location spider
	
	move16(pos_addr,1);
	move16(pos_addr,hero_pos_13D4);
	
	jsr(J_09DE);
}
end_subroutine

subroutine(J_09DE)
{
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x80);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_09F2)
{

	lda_imm(0x27);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x11);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0xC8);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x23);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0xC8);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x01);
	sta(elevator_direction_13D2);
	lda_imm(0x9C);
	sta(0x13D0);
	sta(0x01);
	lda_imm(0xBE);
	sta(0x13D1);
	sta(0x02);
	jmps(J_0A54);
  

}
end_subroutine
subroutine(J_0A54)
{
	lda_imm(0x28);
	sta(0x03);
	lda_imm(0x82);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0F);
	sta(0x06);
	lda_imm(0x00);
	sta(0x00);
	jsr(P_blit_to_screen_5285);
	rts;

}
end_subroutine

subroutine(B_0A6C)
{
	lda(jump_flag_13E4);
	cmp_imm(0x02);
	beq(B_0A93);
  
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0F);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x74);
	sta(0x13D0);
	sta(0x01);
	lda_imm(0xBE);
	sta(0x13D1);
	sta(0x02);
	jmps(J_0A54);
  
B_0A93:

ill;
	ora_ind_X(0xFF);
	sta(0x041F);
	rts;
}
end_subroutine

subroutine(B_0A99)
{
	lda(0x13D0);
	clc;
	adc_imm(0x28);
	sta(0x13D0);
	sta(0x01);
	lda(0x13D1);
	adc_imm(0x00);
	sta(0x13D1);
	sta(0x02);
	cmp_imm(0xBE);
	beq(B_0AB5);
  
	jmps(J_0A54);
  
B_0AB5:
	lda(0x13D0);
	cmp_imm(0x74);
	beq(B_0ABF);
  
	jmps(J_0A54);
  
B_0ABF:
	lda(jump_flag_13E4);
	cmp_imm(0x02);
	bne(B_GEN_1);

	ora_ind_X(0xFF);
	sta(0x041F);
	rts;
B_GEN_1:
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0F);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x1C);
	sta(0x13D0);
	sta(0x01);
	lda_imm(0xA3);
	sta(0x13D1);
	sta(0x02);
	jmps(J_0A54);
  

}
end_subroutine

subroutine(P_move_elevator_0A36)
{

	lda(elevator_direction_13D2);
	bpl(B_GEN_0);
	
	jmps(B_0A99);
B_GEN_0:
	lda(0x13D0);
	sec;
	sbc_imm(0x28);
	sta(0x13D0);
	sta(0x01);
	lda(0x13D1);
	sbc_imm(0x00);
	sta(0x13D1);
	sta(0x02);
	cmp_imm(0xA3);
	bcs(B_GEN_1);
	jmps(B_0A6C);
B_GEN_1:
	J_0A54();
}
end_subroutine


subroutine(P_0AE6)
{

	lda(hero_pos_13D4);
	sta(0x00);
	lda(hero_pos_13D4_plus_1);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_5432);
	lda(0x03);
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	jsr(P_541C);
	lda(0x13D8);
	bmi(B_0B14);
  
	lda(0x02);
	clc;
	adc(0x13D3);
	sta(0x13DD);
	rts;
  
B_0B14:
	lda_imm(0x05);
	sec;
	sbc(0x13D3);
	sta(0x03);
	lda(0x02);
	clc;
	adc(0x03);
	sta(0x13DD);
	rts;
  
 

}
end_subroutine

subroutine(P_0B29)
{

	jsr(P_0AE6);
	lda(jump_flag_13E4);
	cmp_imm(0x03);
	beq(B_0B46);
  
	jsr(P_handle_jump_0C22);
	lda(jump_flag_13E4);
	beq(B_0B43);
  
	cmp_imm(0x01);
	beq(B_0B42);
  
	jsr(P_man_on_elevator_0EB5);
B_0B42:
	rts;
  
B_0B43:
	jmp(J_0BDA);
  
B_0B46:
	lda_imm(0x04);
	ldx_imm(0xFE);
	jsr(P_read_keyboard_matrix_5FDE);
	beq(B_0B72);
  
	lda(0x13DE);
	bne(B_0B77);
  
	lda_imm(0x00);
	sta(0x13DE);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFE);
	tay;
	jsr(P_sound_routine_5456);
	jsr(P_handle_jump_0C22);
	lda_imm(0x06);
	sta(jump_counter_13E0);
	lda_imm(0x01);
	sta(0x13DC);
	rts;
  
B_0B72:
	lda_imm(0x00);
	sta(0x13DE);
B_0B77:
	lda(0x13D8);
	bmi(B_0BB2);
  
	lda(crosser_bit_position_13DA);
	ldx_imm(0x00);
B_0B81:
	inx;
	asl_A();
	bcc(B_0B81);
  
	dex;
	dex;
	dex;
	txa;
	clc;
	adc_imm(0x02);
	tax;
	cmp_imm(0x06);
	bcc(B_0B96);
  
	sec;
	sbc_imm(0x06);
	tax;
	sec;
B_0B96:
	stx(0x00);
	stx(0x13D3);
	lda_imm(0xD9);
	adc(crosser_byte_position_13D9);
	sta(0x01);
	sta(hero_pos_13D4);
	lda_imm(0xB6);
	adc_imm(0x00);
	sta(0x02);
	sta(hero_pos_13D4_plus_1);
	jsr(P_0D8A);
	rts;
  
B_0BB2:
	lda(crosser_bit_position_13DA);
	ldx_imm(0x06);
B_0BB7:
	dex;
	asl_A();
	bcc(B_0BB7);
  
	txa;
	inx;
	inx;
	stx(0x00);
	stx(0x13D3);
	lda_imm(0xDA);
	adc(crosser_byte_position_13D9);
	sta(0x01);
	sta(hero_pos_13D4);
	lda_imm(0xB6);
	adc_imm(0x00);
	sta(0x02);
	sta(hero_pos_13D4_plus_1);
	jsr(P_0D8A);
	rts;
  
J_0BDA:
	lda_imm(0x04);
	ldx_imm(0xBF);
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_0C06);
  
	lda_imm(0x04);
	ldx_imm(0xDF);
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_0C12);

	LOOP_STOP(run);
	
	lda(0x13D3);
	sta(0x00);

	jsr(J_0BF1);
	rts;
	
B_0C06:
	lda(0x13DD);
	cmp(0x13E3);
	bcs(B_0C1A);

	LOOP_PLAY(run);
	jsr(P_move_hero_right_0EEA);
	rts;
 
B_0C12:
	lda(0x13DD);
	beq(B_0C1A);
  
	LOOP_PLAY(run);
	jsr(P_move_hero_left_0F36);
B_0C1A:
	lda(0x13D3);
	sta(0x00);
	jmps(J_0BF1);
  

}
end_subroutine

subroutine(J_0BF1)
{
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda(0x13D8);
	bmi(B_0C03);
  
	jmps(J_09DE);
  
B_0C03:
	jmps(J_0F7F);
}
end_subroutine
subroutine(P_handle_jump_0C22)
{

    if (peek(jump_flag_13E4) != 1)
    {

	// jump (bird, spider)
	lda_imm(0x04);
	ldx_imm(0xFE);
	jsr(P_read_keyboard_matrix_5FDE);
	beq(B_0C8A);
  
	lda(0x13DE);
	bne(B_0C89);
  
	jsr(P_119F);
	lda_imm(0x01);
	sta(0x13DE);
	lda_imm(0x00);
	sta(0x13DF);
	lda_imm(0x14);
	sta(jump_counter_13E0);
	lda_imm(0x05);
	sta(0x13D6);
	lda_imm(0x01);
	sta(0x13D7);

	set_jump_flag(1);
	
	lda_imm(0x04);
	ldx_imm(0xDF);
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_0C90);
  
	lda_imm(0x04);
	ldx_imm(0xBF);
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_0C9B);
  
	lda(0x13D8);
	bpl(J_0C7C);
  
J_0C6F:
	lda_imm(0xC9);
	sta(0x13E1);
	lda_imm(0x89);
	sta(0x13E2);
	jmp(J_0D5E);
  
J_0C7C:
	lda_imm(0xA1);
	sta(0x13E1);
	lda_imm(0x87);
	sta(0x13E2);
	jmp(J_0D5E);
  
B_0C89:
	rts;
  
B_0C8A:
	lda_imm(0x00);
	sta(0x13DE);
	rts;
  
B_0C90:
	lda_imm(0xFF);
	sta(0x13DF);
	sta(0x13D8);
	jmp(J_0C6F);
  
B_0C9B:
	lda_imm(0x01);
	sta(0x13DF);
	sta(0x13D8);
	jmp(J_0C7C);
    }
    
    // manage during jump
	
    dec(jump_counter_13E0);
    lda(jump_counter_13E0);

    if (r.n)
    {
	if (r.a == 0xff)
	{
	    sound_play("jump_down");
	}
	if (r.a < 0x96)
	{
	    inc(jump_counter_13E0);
	}
	jsr(P_jump_down_0DA0);

    }
    else
    {
	jsr(P_jump_up_0D70);
    }
    
    if (peek(jump_flag_13E4) != 1)
    {
	return;
    }
    
    dec(0x13D7);
    if (!r.z) { return; }
  
    lda(0x0402);
    sta(0x13D7);
    lda(0x0402);
    cmp_imm(0x02);
    beq(B_0CE4);
    
    lda_imm(0x02);
    sta(0x0402);
    jmp(J_0CE9);
    
B_0CE4:
	lda_imm(0x03);
	sta(0x0402);
J_0CE9:
	lda(0x13DF);
	bpl(B_0D22);
  
	lda(0x13DD);
	bne(B_0CF9);
  
	lda_imm(0x00);
	sta(0x13DF);
	rts;
  
B_0CF9:
	inc(0x13D3);
	lda(0x13D3);
	cmp_imm(0x06);
	bcc(J_0D5E);
  
	lda_imm(0x00);
	sta(0x13D3);
	sta(0x00);
	lda(hero_pos_13D4);
	sec;
	sbc_imm(0x01);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sbc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmpl(P_0D8A);
  
B_0D22:
	bne(B_0D25);
  
	rts;
  
B_0D25:
	lda(0x13DD);
	cmp(0x13E3);
	bcc(B_0D33);
  
	lda_imm(0x00);
	sta(0x13DF);
	rts;
  
B_0D33:
	inc(0x13D3);
	lda(0x13D3);
	sta(0x00);
	cmp_imm(0x06);
	bcc(J_0D5E);
  
	lda_imm(0x00);
	sta(0x13D3);
	sta(0x00);
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x01);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmpl(P_0D8A);
  
J_0D5E:
	lda(0x13D3);
	sta(0x00);
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	jmpl(P_0D8A);
  

}
end_subroutine

subroutine(P_jump_up_0D70)
{

	lda(0x13D3);
	sta(0x00);
	lda(hero_pos_13D4);
	sec;
	sbc_imm(0x28);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sbc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jsr(P_0D8A);
}
end_subroutine

subroutine(P_0D8A)
{

	lda(0x13E1);
	sta(0x03);
	lda(0x13E2);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_jump_down_0DA0)
{

	lda(hero_pos_13D4);
	clc;
	adc_imm(0x98);
	sta(0x00);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x03);
	sta(0x01);
	P_0DD5(0);
	bpl(B_GEN_1);
	P_0DD5(1); // AKA bmi(B_0E24);
	rts;
B_GEN_1:
	if (r.a != 1)
	{
  
	    lda(0x13D3);
	    sta(0x00);
	    lda(hero_pos_13D4);
	    clc;
	    adc_imm(0x28);
	    sta(hero_pos_13D4);
	    sta(0x01);
	    lda(hero_pos_13D4_plus_1);
	    adc_imm(0x00);
	    sta(hero_pos_13D4_plus_1);
	    sta(0x02);
	    jmpl(P_0D8A);
	}
	
	jsr(P_0E58);

	set_jump_flag(2);
	
	lda(0x94A8);
	sta(0x13D6);
	lda(0x9F00);
	sta(0x9F03);

	jsr(P_test_jump_height);
}
end_subroutine

void P_test_jump_height()
{
	lda(jump_counter_13E0);
	if (r.n)
	{
  
	    cmp_imm(0xE1);
	    if (!r.c)
	    {
                // too high: deadly fall

		move_ff_41f();
	    }
	}
}

void P_0DD5(int goto_0E24)
{

    if (!goto_0E24)
    {
	lda(0x13DF);
	bmi(B_0E10);
  
	cmp_imm(0x01);
	bcs(B_0E1A);
  
	lda_imm(0x01);
	sta(0x02);
	lda_imm(0x04);
	sta(0x03);
B_0DE6:
	ldy(0x02);
B_0DE8:
	lda_ind_Y(0x00);
	cmp_imm(0x80);
	bcs(B_0E0A);
  
	cmp_imm(0x7F);
	beq(B_0E04);
  
	cmp(0x13E5);
	beq(B_0E04);
  
	cmp(0x13E6);
	beq(B_0E07);
  
	iny;
	cpy(0x03);
	bne(B_0DE8);
  
	lda_imm(0x00);
	rts;
  
B_0E04:
	lda_imm(0xFF);
	rts;
  
B_0E07:
	lda_imm(0x01);
	rts;
  
B_0E0A:

move_ff_41f();
//	lda_imm(0xFF);
//	sta(0x041F);
	rts;
  
B_0E10:
	lda_imm(0x01);
	sta(0x02);
	lda_imm(0x03);
	sta(0x03);
	jmp(B_0DE6); // was bne, always branches
  
B_0E1A:
	lda_imm(0x02);
	sta(0x02);
	lda_imm(0x04);
	sta(0x03);
	jmp(B_0DE6);
    }
    
    B_0E24:
    
    set_jump_flag(0);
    lda_imm(0x00);
    sta(0x13DC);
    lda_imm(0x19);
    sta(0x13D6);
    jmp(J_0E49);
  
 
B_0E35:
	jsr(P_0E58);

	set_jump_flag(2);
	
	lda(0x94A8);
	sta(0x13D6);
	lda(0x9F00);
	sta(0x9F03);
J_0E49:
P_test_jump_height();


}


subroutine(P_0E58)
{

	lda_imm(0xFF);
	sta(0x13D8);
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x13D0);
	sec;
	sbc_imm(0x99);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(0x13D1);
	sbc_imm(0x03);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0x04);
	sta(0x13D3);
	jsr(J_0E90);
}
end_subroutine

subroutine(J_0E90)
{
	lda_imm(0x00);
	sta(0x00);
	lda(0x01);
	clc;
	adc_imm(0x01);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0xF1);
	sta(0x03);
	lda_imm(0x8B);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x2E);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts; 

}
end_subroutine

subroutine(P_man_on_elevator_0EB5)
{

	lda(elevator_direction_13D2);
	bmi(B_0ED2);

	// up the elevator
	
	lda(hero_pos_13D4);
	sec;
	sbc_imm(0x28);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sbc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmps(J_0E90);
  
	B_0ED2:

        // down the elevator
	
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x28);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmps(J_0E90);
  

}
end_subroutine

subroutine(P_move_hero_right_0EEA)
{
    
	jsr(P_1138);
	lda(0x13D8);
	bmi(B_0F23);
  
	lda(0x13D3);
	clc;
	adc_imm(0x01);
	sta(0x13D3);
	sta(0x00);
	cmp_imm(0x06);
	bcs(B_0F04);
  
	jmps(J_0BF1);
  
B_0F04:
	lda_imm(0x00);
	sta(0x13D3);
	sta(0x00);
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x01);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmps(J_09DE);
  
B_0F23:
	lda_imm(0x01);
	sta(0x13D8);
	lda_imm(0x05);
	sec;
	sbc(0x13D3);
	sta(0x00);
	sta(0x13D3);
	jmps(J_0BF1);
  
}
end_subroutine


subroutine(P_move_hero_left_0F36)
{
	jsr(P_1138);
	lda(0x13D8);
	bpl(B_0F6C);
  
	lda(0x13D3);
	clc;
	adc_imm(0x01);
	sta(0x13D3);
	sta(0x00);
	cmp_imm(0x06);
	bcc(J_0F93);
  
	lda_imm(0x00);
	sta(0x13D3);
	sta(0x00);
	lda(hero_pos_13D4);
	sec;
	sbc_imm(0x01);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sbc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmps(J_0F7F);
  
B_0F6C:
	lda_imm(0xFF);
	sta(0x13D8);
	lda_imm(0x05);
	sec;
	sbc(0x13D3);
	sta(0x00);
	sta(0x13D3);
J_0F93:
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	jmps(J_0F7F);
     
}
end_subroutine

subroutine(J_0F7F)
{
	lda_imm(0x37);
	sta(0x03);
	lda_imm(0x82);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
}
end_subroutine

subroutine(P_0FA0)
{

	lda_imm(0x00);
	sta(crosser_byte_position_13D9);
	lda_imm(0x01);
	sta(crosser_bit_position_13DA);
	sta(crosser_direction_13DB);

	jmps(P_draw_river_crosser_1005);
  

}
end_subroutine


subroutine(P_move_river_crosser_0FB0)
{    
	ldx_imm(0x0C);
	ldy(crosser_byte_position_13D9);

	move16(0xB5C4,0);
	lda(crosser_bit_position_13DA);
	eor_imm(0xFF);
	sta(0x02);

       	
B_0FC4:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_0FCC);
  
	lda_imm(0x40);
	
	B_0FCC:
	
	and(0x02);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_0FC4);
  
	lda(crosser_direction_13DB);	    
	cmp_imm(0x01);
	bpl(B_1030);

	// move crosser to the left
	
	asl(crosser_bit_position_13DA);
	lda(crosser_bit_position_13DA);
	cmp_imm(0x40);
	bcc(J_1005);
  
	lda_imm(0x01);
	sta(crosser_bit_position_13DA);
	dec(crosser_byte_position_13D9);
	lda(crosser_byte_position_13D9);
	cmp_imm(0x01);
	bcs(J_1005);
  
	lda_imm(0x01);
	sta(crosser_direction_13DB);
	
	J_1005:
	jmps(P_draw_river_crosser_1005);
  
	B_1030:
// move crosser to the right
	lsr(crosser_bit_position_13DA);
	bcc(J_1005);

	// reset pixel to the left
	lda_imm(0x20);
	sta(crosser_bit_position_13DA);
	// increase position
	inc(crosser_byte_position_13D9);
	lda(crosser_byte_position_13D9);
	cmp_imm(0x0A);
	bcc(J_1005);
  
	lda_imm(0xFF);
	sta(crosser_direction_13DB);
	jmps(P_draw_river_crosser_1005);
  

}
end_subroutine
subroutine(P_draw_river_crosser_1005)
{
	ldx_imm(0x0C);
	ldy(crosser_byte_position_13D9);

	move16(0xB5C4,0);

	// draw rod on 12 pixels height
B_1012:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_101A);
  
	lda_imm(0x40);
B_101A:
	ora(crosser_bit_position_13DA);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_1012);
  
	rts;
}
end_subroutine

subroutine(P_104C)
{

	cmp_imm(0x0C);
	beq(B_10AF);
  
	cmp_imm(0x1F);
	bcs(B_1057);
  
	jmp(J_10D1);
  
B_1057:
	pha;
	lda(0x00);
	pha;
	lda(0x01);
	sta(0x00);
	lda_imm(0xF0);
	sta(0x01);
	jsr(P_541C);
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
B_1096:
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
	bne(B_1096);
  
	rts;
  
B_10AF:
	ldy_imm(0x00);
	sty(0x00);
	lda_imm(0xA0);
	sta(0x01);
B_10B7:
	lda_imm(0x40);
	sta_ind_Y(0x00);
	dey;
	bne(B_10B7);
  
	inc(0x01);
	lda(0x01);
	cmp_imm(0xBF);
	bne(B_10B7);
  
	ldx_imm(0x3F);
B_10C8:
	lda_imm(0x00);
	sta_X(0xBF00);
	dex;
	bpl(B_10C8);
  
	rts;
  
J_10D1:
	pha;
	lda(0x00);
	pha;
	lda(0x01);
	sta(0x00);
	lda_imm(0xF0);
	sta(0x01);
	jsr(P_541C);
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
B_10F3:
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
	bne(B_10F3);
  
	rts;
  

}
end_subroutine

subroutine(P_randomize_levels_110A)
{

	ldx_imm(0x03);
	lda_imm(0x00);
B_110E:
	sta_X(0x13E7);
	dex;
	bpl(B_110E);

	// randomize levels
	
	ldx_imm(0x03);
	B_1116:
	
	lda(VIA_T2CL);

#ifdef NO_RANDOM_LEVELS
r.a = 0x55;
#endif

	adc(0x6189);
	adc(0x618A);
	sta(0x6189);
	and_imm(0x03);
	tay;
	lda_Y(0x13E7);
	bne(B_1116);
  
	lda_imm(0xFF);
	sta_Y(0x13E7);
	iny;
	tya;
	sta_X(0x61B2);
	dex;
	bpl(B_1116);
  
	rts;
  

}
end_subroutine

subroutine(P_1138)
{

	lda(0x13D3);
	and_imm(0x01);
	bne(B_114F);
  
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFE);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x02);
	sta(0x13F8);
B_114F:
	rts;
  

}
end_subroutine

subroutine(P_1150)
{

	lda(jump_flag_13E4);
	cmp_imm(0x01);
	bne(B_1186);
  
	lda_imm(0x01);
	sta(0x03);
	lda(jump_counter_13E0);
	bpl(B_1173);
  
	lda(0x13F4);
	clc;
	adc_imm(0x05);
	sta(0x13F4);
	sta(0x04);
	lda_imm(0x00);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  
B_1173:
	lda(0x13F4);
	sec;
	sbc_imm(0x05);
	sta(0x13F4);
	sta(0x04);
	lda_imm(0x00);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  
B_1186:
	lda(0x13F3);
	bne(B_119E);
  
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x01);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x01);
	sta(0x13F3);
	jsr(P_11BF);
B_119E:
	rts;
  

}
end_subroutine

subroutine(P_119F)
{

	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x96);
	sta(0x04);
	sta(0x13F4);
	lda_imm(0x00);
	sta(0x05);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFE);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x00);
	sta(0x13F3);
	rts;
  

}
end_subroutine

subroutine(P_11BF)
{

	lda(0x04FD);
	lsr_A();
	adc_imm(0x00);
	tay;
	ldx_imm(0x08);
	jsr(P_sound_routine_5456);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0xE8);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  

}
end_subroutine

subroutine(P_11E2)
{

	lda(elevator_direction_13D2);
	bpl(B_1202);
  
	lda(0x617B);
	sbc_imm(0xE0);
	sta(0x00);
	lda(0x617C);
	sbc_imm(0x01);
	sta(0x01);
	ldy_imm(0x03);
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	bcs(B_1202);
  
	lda_imm(0x40);
	sta(0x041F);
B_1202:
	rts;
  

}
end_subroutine

subroutine(P_1203)
{

	lda(jump_flag_13E4);
	bne(B_1223);
  
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x96);
	sta(0x00);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x03);
	sta(0x01);
	ldy_imm(0x04);
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	beq(B_1224);
  
	cmp_imm(0x11);
	bcc(B_1224);
  
B_1223:
	rts;
  
B_1224:
	jsr(P_1360);
	lda_imm(0x01);
	sta(0x041E);
	lda_imm(0x0F);
	sta(0x13D6);
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x13D8);
	bmi(B_1271);
  
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x02);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0xA1);
	sta(0x13ED);
	lda_imm(0x90);
	sta(0x13EE);
	jmpl(P_1285);
  
B_1271:
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0xF1);
	sta(0x13ED);
	lda_imm(0x8B);
	sta(0x13EE);
	jsr(P_1285);
}
end_subroutine

subroutine(P_1285)
{

	lda_imm(0x00);
	sta(0x00);
	lda(0x13ED);
	sta(0x03);
	lda(0x13EE);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x2E);
	sta(0x06);
	lda_imm(0x17);
	sta(0x07);
	lda_imm(0x00);
	sta(0x08);
	P_5305(0);
	rts;
  

}
end_subroutine

subroutine(P_deadly_fall_12A7)
{
	lda(0x13EF);
	bne(B_12E0);
  
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x98);
	sta(0x00);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x03);
	sta(0x01);
	ldy_imm(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x11);
	bcc(B_12E0);
  
	cmp_imm(0x40);
	beq(B_12E0);
  
	lda(0x00);
	sta(0x0400);
	lda(0x01);
	sta(0x0401);

	LOOP_STOP(falling);
		
	lda_imm(0x01);
	sta(0x13EF);
	lda_imm(0x13);
	sta(0x13F0);
	lda_imm(0x1E);
	sta(0x13D6);
	
B_12E0:
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x28);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	lda(0x041E);
	cmp_imm(0x01);
	beq(B_1302);
  
	jsr(P_1316);
	jmp(J_1305);
  
B_1302:
	jsr(P_1285);
J_1305:
	lda(0x13EF);
	bne(B_130B);
  
	rts;
  
B_130B:
	dec(0x13F0);
	bne(B_1315);
  
	lda_imm(0x80);
	sta(0x041F);
B_1315:
	rts;
  

}
end_subroutine

subroutine(P_1316)
{

	lda(0x13D3);
	sta(0x00);
	lda(0x13D8);
	bmi(B_132B);
  
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x80);
	sta(0x04);
	jmp(J_1333);
  
B_132B:
	lda_imm(0x37);
	sta(0x03);
	lda_imm(0x82);
	sta(0x04);
J_1333:
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	lda_imm(0x00);
	sta(0x07);
	sta(0x08);
P_5305(0);
	rts;
  

}
end_subroutine

subroutine(P_1345)
{

	lda(jump_flag_13E4);
	bne(B_135F);
  
	lda(0x13F8);
	beq(B_135F);
  
	dec(0x13F8);
	bne(B_135F);
  
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x01);
	tay;
	jsr(P_sound_routine_5456);
B_135F:
	rts;
  

}
end_subroutine

subroutine(P_1360)
{
	lda(0x041E);
	bne(B_139A);
  
	lda_imm(0xE8);
	sta(0x04);
	sta(0x13F1);
	lda_imm(0x01);
	sta(0x05);
	sta(0x13F2);
	lda_imm(0x02);
	sta(0x03);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	ldy_imm(0xFD);
	jsr(P_sound_routine_5456);
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_139A:
	rts;
  

}
end_subroutine

subroutine(P_fall_sound_139B)
{

	lda_imm(0x02);
	sta(0x03);
	lda(0x13F1);
	clc;
	adc_imm(0x32);
	sta(0x13F1);
	sta(0x04);
	lda(0x13F2);
	adc_imm(0x00);
	sta(0x13F2);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  

}
end_subroutine

subroutine(P_13B8)
{
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x7E);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x05);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  
 

}
end_subroutine

subroutine(P_spider_level_1400)
{

	jsr(P_11BF);
	lda_imm(0x88);
	sta(0x0400);
	lda_imm(0xBD);
	sta(0x0401);
	lda_imm(0x00);
	sta(materialize_bridge_part_flag_617A);
	lda_imm(0x99);
	sta(0x617B);
	lda_imm(0xBC);
	sta(0x617C);
	lda_imm(0xD7);
	sta(0x13E3);
	lda_imm(0x00);
	sta(0x041F);
	sta(0x041E);
	sta(0x13EF);
	lda_imm(0x07);
	sta(0x617E);
	lda_imm(0x09);
	sta(0x617F);
	lda_imm(0x00);
	sta(0x6180);
	lda_imm(0x01);
	sta(0x617D);
	sta(0x618D);
	jsr(P_56B9);
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x16);
	sta(0x618B);
	lda_imm(0x00);
	sta(0x618C);
	jsr(P_55D3);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xAA);
	sta(0x01);
	lda_imm(0xA7);
	sta(0x02);
	lda_imm(0x56);
	sta(0x03);
	lda_imm(0x8C);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x87);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x1E24);
	lda_imm(0x14);
	sta(0x13E5);
	lda_imm(0x15);
	sta(0x13E6);
	jsr(P_09F2);
	jsr(P_161A);
	jsr(P_set_hero_initial_pos_09B2);
	jsr(P_1930);
	jsr(P_19E9);
	jsr(P_178E);
	jsr(P_0FA0);
	jsr(P_1B0A);
	jsr(P_1BFE);

	OricTimer tmr = timer_init(5,20);
	set_game_context(GAME_SPIDER);

	LOOP_PLAY(elevator);
	LOOP_PLAY(laser_hum);

	int just_died = 1;
	
	while(1)
	{
	    lda(VIA_T2CL);
	    adc(0x6189);
	    sta(0x618A);
	    eor(0x618A);
	    ldx(0x94A2);

	    update_controls();
	    timer_tick(&tmr);
  
	    dec(0x9F08);
	    bne(B_14C0);
  
	    lda_imm(0x32);
	    sta(0x9F08);
	    // laser beam freezing the red monster
	    jsr(P_draw_laser_cage_1B8F);
	    B_14C0:
	    lda(0x041E);
	    bne(B_14DC);
  
	    lda(0x1E37);
	    beq(B_14DC);
  
	    cmp_imm(0x02);
	    beq(B_14DC);
  
	    dec(0x9F09);
	    bne(B_14DC);
  
	    lda(0x94AA);
	    sta(0x9F09);
	    jsr(P_1B36);
	    B_14DC:
	    dec(0x1E25);
	    bne(B_14EF);
  
	    lda(0x6188);
	    sta(0x1E25);
	    lda(0x1E24);
	    bne(B_14EF);
  
	    jsr(P_1D79);
	B_14EF:
	    lda(0x1E24);
	    beq(B_1502);
  
	    dec(0x9F0A);
	    bne(B_1502);
  
	    lda(0x94A0);
	    sta(0x9F0A);
	    // bird projectile drops
#ifndef VERY_EASY_GAME
	    jsr(P_1DC3);
#endif
	B_1502:
	    if (peek(materialize_bridge_part_flag_617A))
	    {
  
		dec(0x9F0B);
		if (r.z)
		{
		    lda_imm(0x14); // display speed
		    sta(0x9F0B);
		    jsr(P_materialize_bridge_part_5521);
		}
	    }
	    
	    lda(0x041E);
	    bne(B_1533);
  
	    dec(0x9F03);
	    bne(B_1533);
  
	    lda(0x13D6);
	    sta(0x9F03);

	    // move the main character
	
	    P_handle_hero();

	    jsr(P_1748);
	    jsr(P_180C);
	    B_1533:
	    lda(0x041E);
	    beq(B_154F);
  
	    dec(0x9F0C);
	    bne(B_154F);
  
	    lda(0x13D6);
	    sta(0x9F0C);
	    if (just_died)
	    {
		just_died = 0;
		play_kill_sound();
	    }
	
	    jsr(P_deadly_fall_12A7);
	    // jsr(P_fall_sound_139B);
	    jsr(P_1748);
	    // jsr(P_fall_sound_139B);
	    B_154F:
	    lda(0x617D);
	    beq(B_155C);
  
	    dec(0x9F0D);
	    bne(B_155C);
  
	    jsr(P_5686);
	B_155C:
	    dec(0x9F07);
	    bne(B_156A);
  
	    lda(0x94B4);
	    sta(0x9F07);
	
	    jsr(P_move_river_crosser_0FB0);
	    B_156A:
	    lda(0x041E);
	    bne(B_157D);
  
	    dec(0x9F00);
	    bne(B_157D);
  
	    lda(0x94A8);
	    sta(0x9F00);

	
	    jsr(P_move_elevator_0A36);
	B_157D:
	    dec(0x9F02);
	    bne(B_158A);
  
	    lda_imm(0x14);
	    sta(0x9F02);
	    jsr(P_17DD);
	B_158A:
	    dec(0x9F04);
	    bne(B_1598);
  
	    lda(0x94A1);
	    sta(0x9F04);
	    jsr(P_197F);
	B_1598:
	    dec(0x9F06);
	    bne(B_15C7);
  
	    lda_imm(0x32);
	    sta(0x9F06);
	    lda(0x1E2C);
	    sta(0x00);
	    lda(0x1E33);
	    cmp_imm(0x86);
	    beq(B_15BA);
  
	    lda_imm(0xA2);
	    sta(0x1E32);
	    lda_imm(0x86);
	    sta(0x1E33);
	    bne(B_15C4);
  
	    B_15BA:
	    lda_imm(0xA3);
	    sta(0x1E32);
	    lda_imm(0x85);
	    sta(0x1E33);
	B_15C4:
	    jsr(P_1A7C);
	B_15C7:
	    dec(0x9F05);
	    bne(B_15D5);
  
	    lda(0x94B5);
	    sta(0x9F05);
	    jsr(P_1A26);
	    B_15D5:
	    lda(0x041E);
	    cmp_imm(0x02);
	    beq(B_15F0);
  
	    lda(0x041F);
	    cmp_imm(0xFF);
	    bne(B_15F0);
  
	    jsr(P_1360);
	    lda_imm(0x02);
	    sta(0x041E);
	    lda_imm(0x01);
	    sta(0x041F);
	    B_15F0:
	
	    lda(0x041F);

	    if (r.a == 0x80 || r.a == 0x40)
	    {
		break;
	    }

	    if (handle_escape()) break;
	    
	    
	}
  
	
	LOOP_STOP(elevator);
	LOOP_STOP(laser_hum);
	
	/*ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	rts;*/
  

}
end_subroutine

subroutine(P_161A)
{

	jsr(P_1621);
	jsr(P_17DD);
	rts;
  

}
end_subroutine

subroutine(P_1621)
{

	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x0C);
	sta(0x01);
	lda_imm(0xBE);
	sta(0x02);
	lda_imm(0xFF);
	sta(0x03);
	lda_imm(0x0B);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x89);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x0B);
	sta(0x05);
	lda_imm(0x79);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x9F);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x0C);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x9F);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xC8);
	sta(0x01);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xB8);
	sta(0x01);
	lda_imm(0xB5);
	sta(0x02);
	lda_imm(0x06);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xD4);
	sta(0x01);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x0B);
	sta(0x05);
	lda_imm(0x42);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xE0);
	sta(0x01);
	lda_imm(0xAB);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xE0);
	sta(0x01);
	lda_imm(0xAB);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x24);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x17);
	sta(0x03);
	lda_imm(0x28);
	sta(0x05);
	lda_imm(0xF0);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xF0);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x28);
	sta(0x05);
	lda_imm(0xF0);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x07);
	sta(0xBF3A);
	lda_imm(0xFF);
	sta(0xBF3B);
	sta(0xBF3C);
	sta(0xBF3D);
	sta(0xBF3E);
	lda_imm(0x10);
	sta(0xA8A0);
	rts;
  

}
end_subroutine

subroutine(P_1748)
{

	lda_imm(0x03);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xAB);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x9F);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  

}
end_subroutine

subroutine(P_178E)
{

	lda_imm(0x08);
	sta(0x09);
	lda_imm(0xE0);
	sta(0x07);
	lda_imm(0xA1);
	sta(0x08);
B_179A:
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x1F);
	sta(0x03);
	lda_imm(0x8C);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x37);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	sec;
	sbc_imm(0x23);
	sta(0x07);
	lda(0x08);
	sbc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_179A);
  
	rts;
  

}
end_subroutine

subroutine(P_17CB)
{

	lda(0x1E37);
	beq(B_17D1);
  
	rts;
  
B_17D1:
	jsr(P_display_bridge_part_5501);
	jsr(P_cut_monster_laser_beam_18A6);
	lda_imm(0xFF);
	sta(elevator_direction_13D2);
	rts;
  

}
end_subroutine

subroutine(P_17DD)
{

    ldx_imm(0x07);
    lda_imm(0x34);
    sta(0x01);
    lda_imm(0xBE);
    sta(0x02);
    ldy_imm(0x00);
    
    B_17E9:
    lda(0x9F01);
    and_imm(0x01);
    if (!r.z)
    {
  
	lda_imm(0x11);
	B_17F2:
	sta_ind_Y(0x01);
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	inc(0x9F01);
	dex;
	bne(B_17E9);
    }
    else
    {
	lda_imm(0x13);
	bne(B_17F2);
	jsr(P_180C);
    }
}
end_subroutine

subroutine(P_180C)
{

	jsr(P_handle_hero_on_rope_18DA);
	jsr(P_detect_spider_button_186C);
	jsr(P_1203);
	jsr(P_181F);
	jsr(P_1858);
	jsr(P_11E2);
	rts;
  

}
end_subroutine

subroutine(P_181F)
{

	lda(0x1E29);
	clc;
	adc_imm(0x80);
	sta(0x00);
	lda(0x1E2A);
	adc_imm(0x02);
	sta(0x01);
	ldy_imm(0x01);
B_1830:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_1852);
  
	iny;
	cpy_imm(0x05);
	bne(B_1830);
  
	lda(0x1E29);
	sta(0x00);
	lda(0x1E2A);
	sta(0x01);
	ldy_imm(0x03);
	lda_ind_Y(0x00);
	cmp_imm(0x08);
	bcc(B_1851);
  
	cmp_imm(0x60);
	bne(B_1852);
  
B_1851:
	rts;
  
B_1852:
move_ff_41f();

}
end_subroutine

subroutine(P_1858)
{

	lda(hero_pos_13D4_plus_1);
	cmp_imm(0xA4);
	bcs(B_186B);
  
	lda(hero_pos_13D4);
	cmp_imm(0x60);
	bcs(B_186B);

	move_ff_41f();
B_186B:
	rts;
  

}
end_subroutine

subroutine(P_detect_spider_button_186C)
{

	lda(0x1E37);
	bne(B_1882);
  
	lda(0xA8A0);
	cmp_imm(0x10);
	beq(B_GEN_2620);
	jmpl(P_cut_monster_laser_beam_18A6);
B_GEN_2620:
  
	lda(0xA5AB);
	cmp_imm(0x41);
	bcc(B_1882);
  
	jsr(P_17CB);
B_1882:
	lda(0x1E37);
	cmp_imm(0x01);
	bne(B_18A5);
  
	lda(0x1E35);
	clc;
	adc_imm(0xCE);
	sta(0x00);
	lda(0x1E36);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_18A5);

	move_ff_41f();
B_18A5:
	rts;
  

}
end_subroutine

subroutine(P_cut_monster_laser_beam_18A6)
{
    LOOP_STOP(laser_hum);
    
    
    lda_imm(0x01);
    sta(0x1E37);
    lda_imm(0x81);
    sta(0x01);
    lda_imm(0xA7);
    sta(0x02);
    lda_imm(0x10);
    sta(0x03);
    lda_imm(0x01);
    sta(0x05);
    lda_imm(0x1C);
    sta(0x06);
    jsr(P_draw_rectangle_52CE);
    lda_imm(0xE0);
    sta(0x01);
    lda_imm(0xA1);
    sta(0x02);
    lda_imm(0x00);
    sta(0x03);
    lda_imm(0x01);
    sta(0x05);
    lda_imm(0x28);
    sta(0x06);
    jsr(P_draw_rectangle_52CE);
    rts;
  

}
end_subroutine

subroutine(P_handle_hero_on_rope_18DA)
{
    if (peek(jump_flag_13E4) != 3 && !peek(0x13DC))
    { 
	lda(crosser_bit_position_13DA);
	ora_imm(0x40);
	sta(0x02);

	move16(0xB86C,0);	
	ldy(crosser_byte_position_13D9);

	// get data under the crossing rope
	
	lda_ind_Y(0x00);
	
	if (r.a > 0x40)
	{
	    // hero is on rope
	    
	    set_jump_flag(3);

	    lda(0x94B4);
	    sta(0x13D6);
	    lda(0x9F03);
	    clc;
	    adc_imm(0x01);
	    sta(0x9F07);
	    lda(hero_pos_13D4);
	    sta(0x01);
	    lda(hero_pos_13D4_plus_1);
	    sta(0x02);
	    lda_imm(0x40);
	    sta(0x03);
	    lda_imm(0x04);
	    sta(0x05);
	    lda_imm(0x5C);
	    sta(0x06);
	    jsr(P_draw_rectangle_52CE);
	}
    }
}
end_subroutine

subroutine(P_1930)
{

	lda_imm(0xFF);
	sta(0x1E2B);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xEC);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x4F);
	sta(0x03);
	lda_imm(0x85);
	sta(0x04);
	lda_imm(0x07);
	sta(0x05);
	lda_imm(0x54);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x1E28);
	lda_imm(0xCE);
	sta(0x01);
	sta(0x1E29);
	lda_imm(0xAE);
	sta(0x02);
	sta(0x1E2A);
	J_1967();
}
end_subroutine

subroutine(J_1967)
{
	lda_imm(0x5F);
	sta(0x03);
	lda_imm(0x84);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  
 

}
end_subroutine

subroutine(P_197F)
{

	inc(0x1E28);
	lda(0x1E28);
	cmp_imm(0x03);
	bcc(B_198B);
  
	lda_imm(0x00);
B_198B:
	sta(0x1E28);
	sta(0x00);
	lda(0x1E2B);
	bpl(B_19BF);
  
	lda(0x1E29);
	clc;
	adc_imm(0x28);
	sta(0x01);
	sta(0x1E29);
	lda(0x1E2A);
	adc_imm(0x00);
	sta(0x02);
	sta(0x1E2A);
	cmp_imm(0xB7);
	beq(B_19B1);
  
	jmps(J_1967);
  
B_19B1:
	lda(0x01);
	cmp_imm(0xDD);
	bcc(B_19BC);
  
	lda_imm(0x01);
	sta(0x1E2B);
B_19BC:
	jmps(J_1967);
  
B_19BF:
	lda(0x1E29);
	sec;
	sbc_imm(0x28);
	sta(0x01);
	sta(0x1E29);
	lda(0x1E2A);
	sbc_imm(0x00);
	sta(0x02);
	sta(0x1E2A);
	cmp_imm(0xAE);
	beq(B_19DB);
  
	jmps(J_1967);
  
B_19DB:
	lda(0x01);
	cmp_imm(0xCD);
	bcc(B_19E6);
  
	lda_imm(0xFF);
	sta(0x1E2B);
B_19E6:
	jmps(J_1967);
  

}
end_subroutine

subroutine(P_19E9)
{

	lda_imm(0x01);
	sta(0x1E30);
	lda_imm(0xA2);
	sta(0x1E32);
	lda_imm(0x86);
	sta(0x1E33);
	lda_imm(0x00);
	sta(0x00);
	sta(0x1E2C);
	sta(0x1E2F);
	lda_imm(0xD9);
	sta(0x01);
	sta(0x1E2D);
	lda_imm(0xAE);
	sta(0x02);
	sta(0x1E2E);
}
end_subroutine

subroutine(J_1A10)
{
	lda(0x1E32);
	sta(0x03);
	lda(0x1E33);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x55);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
}
end_subroutine

subroutine(P_1A26)
{

	lda(0x1E30);
	bmi(B_1A31);
  
	jsr(P_1A47);
	jmp(J_1A34);
  
B_1A31:
	jsr(P_1A89);
J_1A34:
	lda(0x1E31);
	bmi(B_1A3D);
  
	jsr(P_1AE3);
	rts;
  
B_1A3D:
	jsr(P_1ABC);
	rts;
  
B_1A41:
	lda_imm(0xFF);
	sta(0x1E30);
	rts;
  

}
end_subroutine

subroutine(P_1A47)
{

	inc(0x1E2F);
	lda(0x1E2F);
	cmp_imm(0x48);
	bcc(B_GEN_1);
	lda_imm(0xFF);
	sta(0x1E30);
	rts;
B_GEN_1:
	inc(0x1E2C);
	lda(0x1E2C);
	sta(0x00);
	cmp_imm(0x03);
	bcs(B_GEN_2868);
	jmpl(P_1A7C);
B_GEN_2868:
  
	lda_imm(0x00);
	sta(0x00);
	sta(0x1E2C);
	lda(0x1E2D);
	clc;
	adc_imm(0x01);
	sta(0x1E2D);
	sta(0x01);
	lda(0x1E2E);
	adc_imm(0x00);
	sta(0x1E2E);
	sta(0x02);
	jmps(J_1A10);
  

}
end_subroutine

subroutine(P_1A7C)
{

	lda(0x1E2D);
	sta(0x01);
	lda(0x1E2E);
	sta(0x02);
	jmps(J_1A10);
  

}
end_subroutine

subroutine(P_1A89)
{

	dec(0x1E2F);
	beq(B_1AB7);
  
	dec(0x1E2C);
	lda(0x1E2C);
	sta(0x00);
	bmi(B_GEN_2896);
	jmpl(P_1A7C);
B_GEN_2896:
  
	lda_imm(0x02);
	sta(0x00);
	sta(0x1E2C);
	lda(0x1E2D);
	sec;
	sbc_imm(0x01);
	sta(0x1E2D);
	sta(0x01);
	lda(0x1E2E);
	sbc_imm(0x00);
	sta(0x1E2E);
	sta(0x02);
	jmps(J_1A10);
  
B_1AB7:
	lda_imm(0x01);
	sta(0x1E30);
	jsr(P_1ABC);
}
end_subroutine

subroutine(P_1ABC)
{

	lda(0x1E2C);
	sta(0x00);
	lda(0x1E2D);
	clc;
	adc_imm(0x28);
	sta(0x1E2D);
	sta(0x01);
	lda(0x1E2E);
	adc_imm(0x00);
	sta(0x1E2E);
	cmp_imm(0xB2);
	bcs(B_1ADD);
  
	sta(0x02);
	jmps(J_1A10);
  
B_1ADD:
	lda_imm(0x01);
	sta(0x1E31);
	rts;
  

}
end_subroutine

subroutine(P_1AE3)
{

	lda(0x1E2C);
	sta(0x00);
	lda(0x1E2D);
	sec;
	sbc_imm(0x28);
	sta(0x1E2D);
	sta(0x01);
	lda(0x1E2E);
	sbc_imm(0x00);
	sta(0x1E2E);
	cmp_imm(0xAF);
	bcc(B_1B04);
  
	sta(0x02);
	jmps(J_1A10);
  
B_1B04:
	lda_imm(0xFF);
	sta(0x1E31);
	rts;
  

}
end_subroutine

subroutine(P_1B0A)
{

	lda_imm(0x00);
	sta(0x00);
	sta(0x1E37);
	sta(0x1E34);
	lda_imm(0xAA);
	sta(0x01);
	sta(0x1E35);
	lda_imm(0xA7);
	sta(0x02);
	sta(0x1E36);
	jsr(J_1B22);
}
end_subroutine

subroutine(J_1B22)
{
	lda_imm(0x56);
	sta(0x03);
	lda_imm(0x8C);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x87);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_1B36)
{

	inc(0x1E34);
	lda(0x1E34);
	sta(0x00);
	cmp_imm(0x03);
	bcc(B_1B82);
  
	lda_imm(0x00);
	sta(0x00);
	sta(0x1E34);
	lda(0x1E35);
	clc;
	adc_imm(0x01);
	cmp_imm(0xC6);
	bcs(B_1B7C);
  
	sta(0x1E35);
	sta(0x01);
	sta(0x07);
	lda(0x1E36);
	adc_imm(0x00);
	sta(0x1E36);
	sta(0x02);
	sta(0x08);
	ldy_imm(0x00);
	lda(0x07);
	clc;
	adc_imm(0xCD);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	lda_imm(0x7F);
	sta_ind_Y(0x07);
	jmps(J_1B22);
  
B_1B7C:
	lda_imm(0x00);
	sta(0x1E34);
	rts;
  
B_1B82:
	lda(0x1E35);
	sta(0x01);
	lda(0x1E36);
	sta(0x02);
	jmps(J_1B22);
  

}
end_subroutine

subroutine(P_draw_laser_cage_1B8F)
{

	lda(0x1E37);
	beq(B_1B95);
  
	rts;
  
B_1B95:
	lda_imm(0xA7);
	sta(0x01);
	lda_imm(0x81);
	sta(0x00);
	ldx_imm(0x1C);
	ldy_imm(0x00);
B_1BA1:
	lda(0x1E38);
	sta_ind_Y(0x00);
	ldy_imm(0x07);
	lda_imm(0x10);
	sta_ind_Y(0x00);
	ldy_imm(0x00);
	dec(0x1E38);
	lda(0x1E38);
	cmp_imm(0x12);
	bcs(B_1BBD);
  
	lda_imm(0x17);
	sta(0x1E38);
B_1BBD:
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_1BA1);
  
	lda_imm(0xA1);
	sta(0x01);
	lda_imm(0xE0);
	sta(0x00);
	ldx_imm(0x28);
	ldy_imm(0x00);
B_1BD9:
	lda(0x1E39);
	sta_ind_Y(0x00);
	dec(0x1E39);
	lda(0x1E39);
	cmp_imm(0x02);
	bcs(B_1BED);
  
	lda_imm(0x07);
	sta(0x1E39);
B_1BED:
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_1BD9);
  
	rts;
  

}
end_subroutine

subroutine(P_1BFE)
{

	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x0B);
	sta(0x05);
	lda_imm(0xF2);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xE4);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x24);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x02);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0xF0);
	sta(0x03);
	lda_imm(0x8D);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x30);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x59);
	sta(0x00);
	lda_imm(0xA7);
	sta(0x01);
	jsr(P_1CE5);
	jsr(P_1CF1);
	lda_imm(0x5D);
	sta(0x00);
	lda_imm(0xA7);
	sta(0x01);
	jsr(P_1CE5);
	jsr(P_1D3B);
	lda_imm(0x09);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x07);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x46);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x91);
	sta(0x01);
	lda_imm(0xA4);
	sta(0x02);
	lda_imm(0x20);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x08);
	sta(0xA495);
	sta(0xA4BD);
	sta(0xA4E5);
	sta(0xA50D);
	lda_imm(0x23);
	sta(0x00);
	lda_imm(0xA1);
	sta(0x01);
	ldy_imm(0x00);
	ldx_imm(0x16);
B_1CCE:
	lda_ind_Y(0x00);
	ora_imm(0x08);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_1CCE);
  
	rts;
  

}
end_subroutine

subroutine(P_1CE5)
{

	lda_imm(0x3F);
	sta(0x9E01);
	sta(0x9E02);
	sta(0x9E03);
	rts;
  

}
end_subroutine

subroutine(P_1CF1)
{

	lda_imm(0x24);
	sta(0x07);
B_1CF5:
	jsr(P_1D1E);
	lda(0x07);
	and_imm(0x01);
	beq(B_1D01);
  
	jsr(P_1D06);
B_1D01:
	dec(0x07);
	bne(B_1CF5);
  
	rts;
  

}
end_subroutine

subroutine(P_1D06)
{

	ldx_imm(0x03);
B_1D08:
	lsr_X(0x9E00);
	bcs(B_1D11);
  
	dex;
	bne(B_1D08);
  
	rts;
  
B_1D11:
	dex;
	lda_X(0x9E00);
	ora_imm(0x40);
	sta_X(0x9E00);
	txa;
	bne(B_1D08);
  
	rts;
  

}
end_subroutine

subroutine(P_1D1E)
{

	ldy_imm(0x00);
	ldx_imm(0x03);
B_1D22:
	lda_X(0x9E00);
	ora_imm(0x40);
	sta_ind_Y(0x00);
	iny;
	dex;
	bne(B_1D22);
  
	lda(0x00);
	sec;
	sbc_imm(0x28);
	sta(0x00);
	lda(0x01);
	sbc_imm(0x00);
	sta(0x01);
	rts;
  

}
end_subroutine

subroutine(P_1D3B)
{

	lda_imm(0x24);
	sta(0x07);
B_1D3F:
	jsr(P_1D1E);
	lda(0x07);
	and_imm(0x01);
	beq(B_1D4B);
  
	jsr(P_1D50);
B_1D4B:
	dec(0x07);
	bne(B_1D3F);
  
	rts;
  

}
end_subroutine

subroutine(P_1D50)
{

	clc;
	php;
	ldx_imm(0x01);
B_1D54:
	plp;
	rol_X(0x9E00);
	lda_X(0x9E00);
	and_imm(0x40);
	bne(B_1D68);
  
	inx;
	clc;
	php;
	cpx_imm(0x04);
	bne(B_1D54);
  
	plp;
	rts;
  
B_1D68:
	lda_X(0x9E00);
	and_imm(0x3F);
	sta_X(0x9E00);
	inx;
	sec;
	php;
	cpx_imm(0x04);
	bne(B_1D54);
  
	plp;
	rts;
  

}
end_subroutine

subroutine(P_1D79)
{

	lda(0x1E2D);
	clc;
	adc_imm(0x73);
	sta(0x00);
	lda(0x1E2E);
	adc_imm(0x03);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcc(B_1D91);
  
	rts;
  
B_1D91:
	lda(0x1E2D);
	clc;
	adc_imm(0xAB);
	sta(0x1E26);
	sta(0x01);
	lda(0x1E2E);
	adc_imm(0x02);
	sta(0x1E27);
	sta(0x02);
	lda_imm(0x01);
	sta(0x1E24);
	jsr(J_1DAB);
}
end_subroutine

subroutine(J_1DAB)
{
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x7E);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x05);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_1DC3)
{

	lda(0x1E26);
	clc;
	adc_imm(0xF0);
	sta(0x00);
	lda(0x1E27);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x14);
	beq(B_1DF6);
  
	cmp_imm(0x41);
	bcs(B_1DF6);
  
	lda(0x1E26);
	clc;
	adc_imm(0x28);
	sta(0x1E26);
	sta(0x01);
	lda(0x1E27);
	adc_imm(0x00);
	sta(0x1E27);
	sta(0x02);
	jmps(J_1DAB);
  
B_1DF6:
	lda_imm(0x00);
	sta(0x1E24);
	lda(0x1E26);
	sta(0x01);
	lda(0x1E27);
	sta(0x02);
	cmp_imm(0xBC);
	bcc(B_1E19);
  
B_1E09:
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  
B_1E19:
	cmp_imm(0xB6);
	bcc(B_1E09);
  
	move_ff_41f();
	jmp(B_1E09);
  
	ora_ind_X(0x31);
	//0x8F;   // STRANGE!!!

}
end_subroutine

subroutine(E_1E3F)
{

	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFE);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x02);
	sta(0x13F8);
	rts;
  

}
end_subroutine

subroutine(E_1E50)
{

	lda(jump_flag_13E4);
	cmp_imm(0x01);
	bne(B_1E86);
  
	lda_imm(0x01);
	sta(0x03);
	lda(jump_counter_13E0);
	bpl(B_1E73);
  
	lda(0x13F4);
	clc;
	adc_imm(0x05);
	sta(0x13F4);
	sta(0x04);
	lda_imm(0x00);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  
B_1E73:
	lda(0x13F4);
	sec;
	sbc_imm(0x05);
	sta(0x13F4);
	sta(0x04);
	lda_imm(0x00);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  
B_1E86:
	lda(0x13F3);
	bne(B_1E9E);
  
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x01);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x01);
	sta(0x13F3);
	jsr(P_11BF);
B_1E9E:
	rts;
  

}
end_subroutine

subroutine(E_1E9F)
{

	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x96);
	sta(0x04);
	sta(0x13F4);
	lda_imm(0x00);
	sta(0x05);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFE);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x00);
	sta(0x13F3);
	rts;
  

}
end_subroutine

subroutine(E_1EBF)
{

	lda(0x04FD);
	lsr_A();
	adc_imm(0x00);
	tay;
	ldx_imm(0x08);
	jsr(P_sound_routine_5456);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0xE8);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  

}
end_subroutine


subroutine(P_bridge_level_1F00)
{

	jsr(P_11BF);
	lda_imm(0x03);
	sta(0x618C);
	lda_imm(0x14);
	sta(0x618B);
	jsr(P_55D3);
	lda_imm(0x02);
	sta(0x617E);
	lda_imm(0x00);
	sta(0x617F);
	sta(0x6180);
	jsr(P_56B9);
	lda_imm(0x01);
	sta(0x617D);
	lda_imm(0x00);
	sta(0x041F);
	sta(0x041E);
	sta(0x13EF);
	lda_imm(0x65);
	sta(0x13E3);
	ldx_imm(0x00);
	lda_imm(0x00);
B_1F39:
	sta_X(0x9E00);
	dex;
	bne(B_1F39);
  
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x15);
	sta(0x13E5);
	lda_imm(0xFF);
	sta(0x13E6);
	jsr(P_2044);
	jsr(P_set_hero_initial_pos_09B2);
	jsr(P_22B1);
	jsr(P_24B9);
	jsr(P_2649);
	jsr(P_23CE);
	lda_imm(0x29);
	sta(0x01);
	lda_imm(0xA2);
	sta(0x02);
	jsr(P_250C);
	jsr(P_2524);

	OricTimer tmr = timer_init(4,15);

	set_game_context(GAME_BRIDGE);
	int just_died = 1;
	int noise_count = 0;
	
	while(1)
	{
/*
	ldx(0x94A3);
B_1F71:
	dex;
	bne(B_1F71);
*/

	timer_tick(&tmr);
	update_controls();

	noise_count++;
	if (noise_count == 50)
	{
	    if (cpu_random_8_bit() == 0)
	    {
		sound_play("female_scream");
	    }
	
	    noise_count = 0;
	}
	
	lda(0x041E);
	bne(B_1F90);
  
	dec(0x9F01);
	bne(B_1F90);
  
	lda(0x13D6);
	sta(0x9F01);
	
	P_handle_hero();
	
	jsr(P_bridge_button_test_2037);
B_1F90:
	dec(0x9F02);
	bne(B_1F9D);
  
	lda_imm(0x96);
	sta(0x9F02);
	jsr(P_24E1);
B_1F9D:
	dec(0x9F03);
	bne(B_1FAE);
  
	lda(0x94AB);
	sta(0x9F03);

	jsr(P_move_attack_birds_26B4);
#ifndef VERY_EASY_GAME
	// decide when to drop stones
	jsr(P_handle_bird_stones_2544);
#endif
B_1FAE:
	lda(0x2760);
	beq(B_1FC0);
  
	dec(0x9F04);
	bne(B_1FC0);
  
	lda_imm(0x64);
	sta(0x9F04);
	
	jsr(P_lower_bridge_part_247C);

B_1FC0:
	dec(0x9F05);
	bne(B_1FCE);
  
	lda(0x94AC);
	sta(0x9F05);
	jsr(P_animate_bird_stones_25BD);
B_1FCE:
	lda(0x041E);
	beq(B_1FE4);

	// dying
	
	dec(0x9F06);
	bne(B_1FE4);
  
	lda(0x13D6);
	sta(0x9F06);

	if (just_died)
	{
	    just_died = 0;
	    play_kill_sound();
	}
	
	jsr(P_deadly_fall_12A7);
	jsr(P_fall_sound_139B);
B_1FE4:
	lda(0x617D);
	beq(B_1FF1);
  
	dec(0x9F07);
	bne(B_1FF1);
  
	jsr(P_5686);

	B_1FF1:
	
	if (handle_escape()) break;

  
B_2005:
	lda(0x041E);
	cmp_imm(0x02);
	beq(B_2020);
  
	lda(0x041F);
	cmp_imm(0xFF);
	bne(B_2020);
  
	jsr(P_1360);
	lda_imm(0x02);
	sta(0x041E);
	lda_imm(0x01);
	sta(0x041F);
	B_2020:
	lda(0x041F);
	if (r.a == 0x20 || r.a == 0x40 || r.a == 0x80) break;
	}
  
	  

}
end_subroutine

subroutine(P_bridge_button_test_2037)
{

	lda(0xB822);
	cmp_imm(0x41);
	bcc(B_2043);

	// button reached
	
	LOOP_PLAY(lower_bridge);
	
	lda_imm(0x01);
	sta(0x2760);
B_2043:
	rts;
  

}
end_subroutine

subroutine(P_2044)
{

	lda_imm(0xBD);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x96);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xC2);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x96);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xBE);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x0A);
	sta(0x05);
	lda_imm(0x78);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xC0);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x18);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xC4);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x18);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x9E);
	sta(0x01);
	lda_imm(0xA7);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x6F);
	sta(0x01);
	lda_imm(0xAA);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x6D);
	sta(0x01);
	lda_imm(0xAA);
	sta(0x02);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x60);
	sta(0x01);
	lda_imm(0xAB);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x5E);
	sta(0x01);
	lda_imm(0xAB);
	sta(0x02);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x51);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x6F);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x4F);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0xA7);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xFA);
	sta(0x01);
	lda_imm(0xB8);
	sta(0x02);
	lda_imm(0x11);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xDB);
	sta(0x01);
	lda_imm(0xBA);
	sta(0x02);
	lda_imm(0x70);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x03);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xD8);
	sta(0x01);
	lda_imm(0xBA);
	sta(0x02);
	lda_imm(0x07);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x03);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xFC);
	sta(0x01);
	lda_imm(0xB8);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x17);
	sta(0x05);
	lda_imm(0x17);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x9F);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x08);
	sta(0x05);
	lda_imm(0x58);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x6C);
	sta(0x07);
	lda_imm(0xAD);
	sta(0x08);
	lda_imm(0x03);
	sta(0x09);
	jsr(P_2265);
	lda_imm(0xFB);
	sta(0x07);
	lda_imm(0xB3);
	sta(0x08);
	lda_imm(0x03);
	sta(0x09);
	jsr(P_2265);
	lda_imm(0x90);
	sta(0x07);
	lda_imm(0xA8);
	sta(0x08);
	lda_imm(0x01);
	sta(0x09);
	jsr(P_2265);
	lda_imm(0x95);
	sta(0x07);
	lda_imm(0xA8);
	sta(0x08);
	lda_imm(0x01);
	sta(0x09);
	jsr(P_2265);
	rts;
  

}
end_subroutine

subroutine(P_2265)
{

	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x38);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
B_2284:
	lda(0x07);
	clc;
	adc_imm(0xF0);
	sta(0x07);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	sta(0x02);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x44);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	dec(0x09);
	bne(B_2284);
  
	rts;
  

}
end_subroutine

subroutine(P_22B1)
{

	lda_imm(0x08);
	sta(0x07);
	lda_imm(0x1C);
	sta(0x08);
	lda_imm(0xB6);
	sta(0x09);
B_22BD:
	lda_imm(0x00);
	sta(0x00);
	lda(0x08);
	sta(0x01);
	lda(0x09);
	sta(0x02);
	lda_imm(0x50);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x08);
	clc;
	adc_imm(0xF0);
	sta(0x08);
	lda(0x09);
	adc_imm(0x00);
	sta(0x09);
	dec(0x07);
	bne(B_22BD);
  
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xFE);
	sta(0x01);
	lda_imm(0xB7);
	sta(0x02);
	lda_imm(0x5C);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x09);
	sta(0x07);
	lda_imm(0xFF);
	sta(0x08);
	lda_imm(0xB7);
	sta(0x09);
B_2318:
	lda_imm(0x00);
	sta(0x00);
	lda(0x08);
	sta(0x01);
	lda(0x09);
	sta(0x02);
	lda_imm(0x68);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x08);
	clc;
	adc_imm(0x01);
	sta(0x08);
	lda(0x09);
	adc_imm(0x00);
	sta(0x09);
	dec(0x07);
	bne(B_2318);
  
	lda_imm(0x00);
	sta(0x00);
	lda(0x08);
	sta(0x01);
	lda(0x09);
	sta(0x02);
	lda_imm(0x6E);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x07);
	sta(0xB5F1);
	lda_imm(0x4F);
	sta(0xB5F2);
	lda_imm(0x7F);
	sta(0xB5F3);
	sta(0xB5F4);
	sta(0xB5F5);
	lda_imm(0x48);
	sta(0xB61A);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x40);
	sta(0x01);
	lda_imm(0xB6);
	sta(0x02);
	lda_imm(0x20);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x08);
	sta(0xB643);
	sta(0xB66B);
	sta(0xB693);
	sta(0xB6BB);
	lda_imm(0x03);
	sta(0xB630);
	sta(0xB658);
	sta(0xB680);
	sta(0xB6A8);
	lda_imm(0x06);
	sta(0xB644);
	sta(0xB66C);
	sta(0xB694);
	sta(0xB6BC);
	jsr(P_240F);
	rts;
  

}
end_subroutine

subroutine(P_23CE)
{

	lda(0x13F5);
	bne(B_23D4);
  
	rts;
  
B_23D4:
	sta(0x09);
	lda_imm(0x9F);
	sta(0x07);
	lda_imm(0xBD);
	sta(0x08);
B_23DE:
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x95);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0x02);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_23DE);
  
	rts;
  

}
end_subroutine

subroutine(P_240F)
{

	lda_imm(0x00);
	sta(0x2760);
	lda_imm(0x15);
	sta(0x2763);
	lda_imm(0xEE);
	sta(0x01);
	lda_imm(0xB8);
	sta(0x02);
	ldx(0x13F5);
	beq(B_2436);
  
B_2426:
	lda(0x01);
	clc;
	adc_imm(0x02);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	dex;
	bne(B_2426);
  
B_2436:
	lda(0x01);
	sta(0x07);
	sta(0x2761);
	lda(0x02);
	sta(0x08);
	sta(0x2762);
	jsr(P_2444);
}
end_subroutine

subroutine(P_2444)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x74);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1B);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0x68);
	sta(0x01);
	lda(0x08);
	adc_imm(0x01);
	sta(0x02);
	lda_imm(0x83);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_lower_bridge_part_247C)
{

	lda(0x2761);
	clc;
	adc_imm(0x28);
	sta(0x2761);
	sta(0x01);
	sta(0x07);
	lda(0x2762);
	adc_imm(0x00);
	sta(0x2762);
	sta(0x02);
	sta(0x08);
	jsr(P_2444);
	dec(0x2763);
	beq(B_249E);
  
	rts;
  
B_249E:
	LOOP_STOP(lower_bridge);
	sound_play("bridge_laid");
	
	lda_imm(0x00);
	sta(0x2760);
	lda_imm(0x20);
	sta(0x041F);
	inc(0x13F5);

	if (peek(0x13F5) == 4)
	{
	    // all missions done
	    poke(0x41F,0x40);
	}  

}
end_subroutine

subroutine(P_24B9)
{

	lda_imm(0x01);
	sta(0x275F);
	sta(0x275E);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xEC);
	sta(0x01);
	lda_imm(0xA2);
	sta(0x02);
	jsr(J_24CD);
}
end_subroutine

subroutine(J_24CD)
{
	lda_imm(0x8F);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x55);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_24E1)
{

	lda(0x275E);
	clc;
	adc(0x275F);
	sta(0x275E);
	sta(0x00);
	beq(B_24FE);
  
	cmp_imm(0x02);
	beq(B_2505);
  
B_24F3:
	lda_imm(0xEC);
	sta(0x01);
	lda_imm(0xA2);
	sta(0x02);
	jmps(J_24CD);
  
B_24FE:
	lda_imm(0x01);
	sta(0x275F);
	bne(B_24F3);
  
B_2505:
	lda_imm(0xFF);
	sta(0x275F);
	bne(B_24F3);
  

}
end_subroutine

subroutine(P_250C)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x8E);
	sta(0x03);
	lda_imm(0x8F);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x42);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_2524)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x22);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0xD0);
	sta(0x03);
	lda_imm(0x8F);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x3C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_handle_bird_stones_2544)
{

	dec(0x2764);
	bpl(B_254E);
  
	lda_imm(0x02);
	sta(0x2764);
B_254E:
	lda(0x2764);
	sta(0x08);
	ldx_imm(0x00);
	ldy_imm(0x14);
	cmp_imm(0x00);
	beq(B_2566);
  
B_255B:
	inx;
	inx;
	inx;
	inx;
	iny;
	iny;
	iny;
	dec(0x08);
	bne(B_255B);
  
B_2566:
	lda_Y(0x9E00);
	bne(B_25BC);
  
	lda_X(0x9E00);
	cmp_imm(0x36);
	bcs(B_25BC);
  
	lda_imm(0x01);
	sta_Y(0x9E00);
	inx;
	inx;
	iny;
	lda_X(0x9E00);
	clc;
	adc_imm(0xFB);
	sta_Y(0x9E00);
	sta(0x00);
	inx;
	iny;
	lda_X(0x9E00);
	adc_imm(0x02);
	sta_Y(0x9E00);
	sta(0x01);
	tya;
	pha;
	ldy_imm(0x00);
	ldx_imm(0x06);
B_2597:
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	beq(B_25A1);
  
	cmp_imm(0x08);
	bcs(B_25B3);
  
B_25A1:
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_2597);
  
	pla;
	rts;
  
B_25B3:
	pla;
	tay;
	dey;
	dey;
	lda_imm(0x00);
	sta_Y(0x9E00);
B_25BC:
	rts;
  

}
end_subroutine

subroutine(P_animate_bird_stones_25BD)
{

	ldx_imm(0x14);
J_25BF:
	lda_X(0x9E00);
	bne(B_25CC);
  
	inx;
	inx;
	inx;
	cpx_imm(0x1D);
	bne(J_25BF);
  
	rts;
  
B_25CC:
	inx;
	lda_X(0x9E00);
	clc;
	adc_imm(0xF0);
	sta(0x00);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	beq(B_25E9);
  
	cmp_imm(0x08);
	bcs(B_260D);
  
B_25E9:
	dex;
	lda_X(0x9E00);
	clc;
	adc_imm(0x28);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
	txa;
	pha;
	jsr(P_13B8);
	pla;
	tax;
	inx;
	cpx_imm(0x1D);
	bne(J_25BF);
  
	rts;
  
B_260D:
	dex;
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	cmp_imm(0xB7);
	bcc(B_262D);
  
	cmp_imm(0xBC);
	bcs(B_262D);

	move_ff_41f();
B_262D:
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	txa;
	pha;
	jsr(P_draw_rectangle_52CE);
	pla;
	tax;
	inx;
	cpx_imm(0x1D);
	bne(B_2646);
  
	rts;
  
B_2646:
	jmp(J_25BF);
  

}
end_subroutine

subroutine(P_2649)
{

	lda_imm(0x19);
	sta(0x9E00);
	lda_imm(0x00);
	sta(0x00);
	sta(0x9E01);
	lda_imm(0x70);
	sta(0x01);
	sta(0x9E02);
	lda_imm(0xB0);
	sta(0x02);
	sta(0x9E03);
	jsr(P_26A0);
	lda_imm(0x2A);
	sta(0x9E04);
	lda_imm(0x01);
	sta(0x00);
	sta(0x9E05);
	lda_imm(0xA6);
	sta(0x01);
	sta(0x9E06);
	lda_imm(0xAD);
	sta(0x02);
	sta(0x9E07);
	jsr(P_26A0);
	lda_imm(0x4A);
	sta(0x9E08);
	lda_imm(0x02);
	sta(0x00);
	sta(0x9E09);
	lda_imm(0x60);
	sta(0x01);
	sta(0x9E0A);
	lda_imm(0xB2);
	sta(0x02);
	sta(0x9E0B);
	jmpl(P_26A0);
  

}
end_subroutine

subroutine(P_26A0)
{

	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x91);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x64);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_move_attack_birds_26B4)
{

	ldx_imm(0x00);
J_26B6:
	dec_X(0x9E00);
	beq(B_270A);
  
	inx;
	dec_X(0x9E00);
	lda_X(0x9E00);
	sta(0x00);
	bpl(B_26F1);
  
	lda_imm(0x02);
	sta(0x00);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	sec;
	sbc_imm(0x01);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sbc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
	txa;
	pha;
	jsr(P_26A0);
	pla;
	tax;
	inx;
	cpx_imm(0x0C);
	bne(J_26B6);
  
	rts;
  
B_26F1:
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	txa;
	pha;
	jsr(P_26A0);
	pla;
	tax;
	inx;
	cpx_imm(0x0C);
	bne(J_26B6);
  
	rts;
  
B_270A:
	txa;
	pha;
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x64);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	pla;
	tax;
	lda_imm(0x54);
	sta_X(0x9E00);
	inx;
	lda_imm(0x00);
	sta(0x00);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	clc;
	adc_imm(0x1C);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
	txa;
	pha;
	jsr(P_26A0);
	pla;
	tax;
	inx;
	cpx_imm(0x0C);
	bne(B_275B);
  
	rts;
  
B_275B:
	jmp(J_26B6);
  
 

}
end_subroutine

subroutine(E_2766)
{

	lda_imm(0xA7);
	sta(0x01);
	jsr(P_1CE5);
	jsr(P_1CF1);
	lda_imm(0x5D);
	sta(0x00);
	lda_imm(0xA7);
	sta(0x01);
	jsr(P_1CE5);
	jsr(P_1D3B);
	lda_imm(0x09);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x07);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x46);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x91);
	sta(0x01);
	lda_imm(0xA4);
	sta(0x02);
	lda_imm(0x20);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x08);
	sta(0xA495);
	sta(0xA4BD);
	sta(0xA4E5);
	sta(0xA50D);
	lda_imm(0x23);
	sta(0x00);
	lda_imm(0xA1);
	sta(0x01);
	ldy_imm(0x00);
	ldx_imm(0x16);
B_27CE:
	lda_ind_Y(0x00);
	ora_imm(0x08);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_27CE);
  
	rts;
  

}
end_subroutine

subroutine(E_27E5)
{

	lda_imm(0x3F);
	sta(0x9E01);
	sta(0x9E02);
	sta(0x9E03);
	rts;
  
 

}
end_subroutine

subroutine(P_ship_level_2800)
{

	jsr(P_54AC);
	lda_imm(0xE8);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x01);
	sta(0x03);
	jsr(P_sound_routine_5482);
	lda_imm(0xEA);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x02);
	sta(0x03);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	ldy_imm(0xFC);
	jsr(P_sound_routine_5456);
	lda_imm(0x11);
	sta(0x618B);
	lda_imm(0x03);
	sta(0x618C);
	jsr(P_55D3);
	lda_imm(0x01);
	sta(0x617E);
	lda_imm(0x00);
	sta(0x617F);
	sta(0x6180);
	sta(0x617D);
	jsr(P_56B9);
	lda_imm(0x00);
	sta(0x041F);
	sta(0x041E);
	sta(0x0A93);
	sta(0x332E);
	sta(0x3334);
	lda_imm(0x00);
	ldx_imm(0x00);
B_285D:
	sta_X(0x9E00);
	dex;
	bne(B_285D);
  
	sta(0x6195);
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0xD8);
	sta(0x0400);
	lda_imm(0xBD);
	sta(0x0401);
	jsr(P_2E8E);
	jsr(P_2B8E);

	OricTimer tmr = timer_init(4,30);
	set_game_context(GAME_SHIP);

	while(1)
	{
	
	    timer_tick(&tmr);
	    update_controls();

	    /*
	      ldx(0x94A4);
	      B_2888:
	      dex;
	      bne(B_2888);
	    */
	
	    dec(0x9F00);
	    bne(B_28A5);
  
	    lda_imm(0x03);
	    sta(0x9F00);
	    jsr(P_move_and_display_ship_2CAB);
#ifndef VERY_EASY_GAME
	    jsr(P_ship_collision_detection_2D28);
#endif
	    dec(0x332C);
	    bne(B_28A5);
  
	    lda_imm(0x07);
	    sta(0x332C);
	    B_28A5:
	    dec(0x9F01);
	    bne(B_28D3);
  
	    lda_imm(0x0A);
	    sta(0x9F01);
	    jsr(P_2EF5);
	    lda(0x6195);
	    bne(B_28D3);
  
	    lda(VIA_T2CL);
	    sta(0x618A);
	    cmp_imm(0x08);
	    bcs(B_28D3);
  
	    lda(VIA_T2CL);
	    adc(0x6189);
	    sta(0x6189);
	    adc(VIA_T2CL);
	    sta(0x618A);
	    jsr(P_56C7);
	    B_28D3:
	    lda(0x6195);
	    beq(B_28E6);
  
	    dec(0x9F02);
	    bne(B_28E6);
  
	    lda(0x94A9);
	    sta(0x9F02);
	    jsr(P_57A0);
	    B_28E6:
	    lda(0x332E);
	    bne(B_28FB);
  
	    lda(0x0A93);
	    cmp(0x94AF);
	    bne(B_28FB);
  
	    jsr(P_30EF);
	    lda_imm(0x00);
	    sta(0x0A93);
	    B_28FB:
	    dec(0x9F03);
	    bne(B_2925);
  
	    lda(VIA_T2CL);
	    sta(0x618A);
	    cmp_imm(0x08);
	    bcs(B_291C);
  
	    lda(VIA_T2CL);
	    adc(0x6189);
	    sta(0x6189);
	    adc(VIA_T2CL);
	    sta(0x618A);
	    jsr(P_308F);
	B_291C:
	    lda(0x94AF);
	    sta(0x9F03);
	    jsr(P_2F8E);
	B_2925:
	    lda(VIA_T2CL);
	    sta(0x618A);
	    cmp_imm(0x02);
	    bcs(B_2941);
  
	    lda(VIA_T2CL);
	    adc(0x6189);
	    sta(0x6189);
	    adc(VIA_T2CL);
	    sta(0x618A);
	    jsr(P_2D93);
	    B_2941:
	    dec(0x9F04);
	    bne(B_2951);
  
	    lda_imm(0x05);
	    sta(0x9F04);
	    jsr(P_handle_shoot_29C0);
	    jsr(P_32CB);
	    B_2951:
	    dec(0x9F05);
	    bne(B_295F);
  
	    lda(0x94AD);
	    sta(0x9F05);
	    jsr(P_2DF2);
	    B_295F:
	    lda(0x332E);
	    beq(J_299C);
  
	    dec(0x9F06);
	    bne(J_299C);
  
	    lda_imm(0x05);
	    sta(0x9F06);
	    jsr(P_3134);
	    lda(VIA_T2CL);
	    sta(0x618A);
	    cmp_imm(0x28);
	    bcs(J_299C);
  
	    lda(VIA_T2CL);
	    adc(0x6189);
	    sta(0x6189);
	    adc(VIA_T2CL);
	    sta(0x618A);
	    lda(0x6189);
	    bpl(B_2997);
  
	    lda_imm(0xFF);
	    sta(0x3333);
	    jmp(J_299C);
  
	    B_2997:
	    lda_imm(0x01);
	    sta(0x3333);
	    J_299C:
	    if (peek(0x041F)) break;

	    if (handle_escape()) break;
	
	}
  
	rts;
  
B_29B9:
	lda_imm(0x00);
	sta(0x332D);
	bne(B_out);
	jsr(B_2A04);
B_out:
	rts;
}
end_subroutine

subroutine(P_handle_shoot_29C0)
{

	lda_imm(0x04);
	ldx_imm(0xFE);
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_GEN_1);

	lda_imm(0x00);
	sta(0x332D);
	bne(B_out);
	jsr(B_2A04);
B_out:
	rts;
B_GEN_1:
	lda(0x332D);
	beq(B_GEN_2);
	jmps(B_2A04);
B_GEN_2:
	sound_play("shoot");
	lda_imm(0x01);
	sta(0x332D);
	ldx_imm(0x00);
B_29D5:
	lda_X(0x9E00);
	beq(B_29E3);
  
	inx;
	inx;
	inx;
	inx;
	cpx_imm(0x0C);
	bne(B_29D5);
  
	rts;
  
B_29E3:
	lda_imm(0x01);
	sta_X(0x9E00);
	inx;
	lda(0x3326);
	clc;
	adc_imm(0x6C);
	sta_X(0x9E00);
	inx;
	lda(0x3327);
	adc_imm(0x01);
	sta_X(0x9E00);
	inx;
	lda_imm(0x01);
	sta_X(0x9E00);
	jsr(P_32AC);
	jsr(B_2A04);
}
end_subroutine

subroutine(B_2A04)
{
	ldx_imm(0x00);
J_2A06:
	lda_X(0x9E00);
	bne(B_2A14);
  
	inx;
	inx;
	inx;
	inx;
	cpx_imm(0x0C);
	bne(J_2A06);
  
	rts;
  
B_2A14:
	txa;
	pha;
	inx;
	lda_imm(0x00);
	sta(0x00);
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x67);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	inx;
	lda_X(0x9E00);
	sta(0x05);
	sta(0x06);
	ldy_imm(0x00);
	lda(0x332C);
	sta_ind_Y(0x01);
	tya;
	clc;
	adc_imm(0x28);
	adc(0x06);
	tay;
B_2A44:
	lda_ind_Y(0x01);
	cmp_imm(0x41);
	bcs(B_2AC9);
  
	dey;
	cpy_imm(0x28);
	bne(B_2A44);
  
	lda(0x01);
	clc;
	adc_imm(0x01);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	jsr(P_blit_to_screen_5285);
	pla;
	tax;
	inx;
	inx;
	inx;
	inc_X(0x9E00);
	lda_X(0x9E00);
	cmp_imm(0x18);
	bcs(J_2A9C);
  
	dec(0x332B);
	beq(B_2A79);
  
	inx;
	cpx_imm(0x0C);
	bne(J_2A06);
  
	rts;
  
B_2A79:
	dex;
	dex;
	lda_imm(0x05);
	sta(0x332B);
	lda_X(0x9E00);
	clc;
	adc_imm(0x01);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta_X(0x9E00);
	inx;
	inx;
	cpx_imm(0x0C);
	bne(B_2A99);
  
	rts;
  
B_2A99:
	jmp(J_2A06);
  
J_2A9C:
	txa;
	pha;
	dex;
	dex;
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	inx;
	lda_X(0x9E00);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	pla;
	tax;
	inx;
	cpx_imm(0x0C);
	bne(B_2A99);
  
	rts;
  
B_2AC9:
	pla;
	txa;
	pha;
	tya;
	clc;
	adc(0x01);
	sta(0x05);
	sta(0x00);
	lda(0x02);
	adc_imm(0x00);
	sta(0x06);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_5432);
	lda(0x03);
	sta(0x07);
	lda(0x332E);
	beq(B_2AF7);
  
	lda(0x3334);
	bne(B_2AF7);
  
	jsr(P_2BB4);
B_2AF7:
	ldx_imm(0x10);
B_2AF9:
	lda_X(0x9E00);
	cmp_imm(0x01);
	beq(B_2B0E);
  
	txa;
	clc;
	adc_imm(0x06);
	tax;
	cpx_imm(0x28);
	bne(B_2AF9);
  
	pla;
	tax;
	jmp(J_2A9C);
  
B_2B0E:
	inx;
	inx;
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x00);
	inx;
	lda_X(0x9E00);
	sta(0x01);
	lda(0x06);
	cmp(0x01);
	bcc(B_2B46);
  
	bne(B_2B2B);
  
	lda(0x05);
	cmp(0x00);
	bcc(B_2B46);
  
B_2B2B:
	lda(0x00);
	clc;
	adc_imm(0x80);
	sta(0x00);
	lda(0x01);
	adc_imm(0x02);
	sta(0x01);
	lda(0x06);
	cmp(0x01);
	bcc(B_2B50);
  
	bne(B_2B46);
  
	lda(0x05);
	cmp(0x00);
	bcc(B_2B50);
  
B_2B46:
	inx;
	cpx_imm(0x28);
	bne(B_2AF9);
  
	pla;
	tax;
	jmp(J_2A9C);
  
B_2B50:
	lda(0x01);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	txa;
	pha;
	jsr(P_5432);
	pla;
	tax;
	lda(0x07);
	cmp(0x03);
	bcc(B_2B46);
  
	clc;
	adc_imm(0x03);
	cmp(0x03);
	bcc(B_2B46);
  
	txa;
	sec;
	sbc_imm(0x05);
	tax;
	lda_imm(0xFF);
	sta_X(0x9E00);
	pla;
	tax;
	inc(0x0A93);
	lda_imm(0x01);
	sta(0x961D);
	lda_imm(0x05);
	sta(0x961E);
	jsr(P_564F);
	jmp(J_2A9C);
  

}
end_subroutine

subroutine(P_2B8E)
{

	lda_imm(0x00);
	sta(0x00);
	sta(0x332D);
	lda_imm(0xA1);
	sta(0x3326);
	lda_imm(0xAF);
	sta(0x3327);
	lda_imm(0x05);
	sta(0x332B);
	lda_imm(0x03);
	sta(0x332C);
	lda_imm(0x19);
	sta(0x3328);
	lda_imm(0x01);
	sta(0x041E);
	rts;
  

}
end_subroutine

subroutine(P_2BB4)
{

	lda(0x3330);
	sta(0x00);
	lda(0x3331);
	sta(0x01);
	lda(0x06);
	cmp(0x01);
	bcc(B_2BE7);
  
	bne(B_2BCC);
  
	lda(0x05);
	cmp(0x00);
	bcc(B_2BE7);
  
B_2BCC:
	lda(0x00);
	clc;
	adc_imm(0x80);
	sta(0x00);
	lda(0x01);
	adc_imm(0x02);
	sta(0x01);
	lda(0x06);
	cmp(0x01);
	bcc(B_2BE8);
  
	bne(B_2BE7);
  
	lda(0x05);
	cmp(0x00);
	bcc(B_2BE8);
  
B_2BE7:
	rts;
  
B_2BE8:
	lda(0x01);
	sec;
	sbc_imm(0xA0);
	sta(0x01);
	lda_imm(0x28);
	sta(0x02);
	jsr(P_5432);
	lda(0x07);
	cmp(0x03);
	bcc(B_2BE7);
  
	clc;
	adc_imm(0x03);
	cmp(0x03);
	bcc(B_2BE7);
  
	lda_imm(0x01);
	sta(0x3334);
	lda_imm(0x00);
	sta(0x3335);

}
end_subroutine

subroutine(P_2C0D)
{

	lda_imm(0x00);
	sta(0x00);
	lda(0x3326);
	sta(0x01);
	lda(0x3327);
	sta(0x02);
	lda_imm(0x0C);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x40);
	sta(0x06);
	lda_imm(0x08);
	sta(0x07);
	lda(0x3328);
	sta(0x08);
P_5305(0);
	rts;
  

}
end_subroutine

subroutine(P_2C38)
{

	lda(0x3326);
	sta(0x01);
	lda(0x3327);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x40);
	sta(0x06);
	lda_imm(0x08);
	sta(0x07);
	lda(0x3328);
	sta(0x08);
	jsr(P_screen_routine_53D5);
	rts;
}
end_subroutine

subroutine(B_handle_ship_disappearing_2C5B)
{
	jsr(P_2C38);
	inc(0x3328);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x1F);
	sec;
	sbc(0x3328);
	sta(0x04);
	jsr(P_sound_routine_5482);
	lda(0x3328);
	cmp_imm(0x19);
	beq(B_2C7B);
  
	jsr(P_2C0D);
	rts;
  
B_2C7B:
	lda_imm(0x80);
	sta(0x041F);
	rts;
}
end_subroutine
	
subroutine(B_handle_ship_appearing_2C81)
{
	dec(0x9F0F);
	beq(B_2C87);
  
	rts;
  
B_2C87:
	sound_play("ship_appears");
	// ship appears
	lda_imm(0x02);
	sta(0x9F0F);
	jsr(P_2C38);
	dec(0x3328);
	jsr(P_2C0D);
	lda(0x3328);
	beq(B_2C9B);
  
	rts;
  
B_2C9B:
	lda_imm(0x00);
	sta(0x041E);
	sta(0x3328);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	rts;
  

}
end_subroutine

subroutine(P_move_and_display_ship_2CAB)
{

    int state = peek(0x41E);

    if ((state > 0) && (state < 0x80))
    {
	jsr(B_handle_ship_appearing_2C81);
    }
    else if (state > 0x7f)
    {	
	jsr(B_handle_ship_disappearing_2C5B);
    }
    else
    {
	// normal game
	
	lda_imm(0x00);
	ldx_imm(0xBF);
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_2CC5);
  
	lda_imm(0x04);
	ldx_imm(0xFF); // was DF but I changed the controls for the ship!!
	jsr(P_read_keyboard_matrix_5FDE);
	bne(B_2D01);
  
	rts;
  
B_2CC5:
	lda(0x3327);
	cmp_imm(0xA0);
	beq(B_2CF9);
  
B_2CCC:
	lda(0x3326);
	sec;
	sbc_imm(0x28);
	sta(0x3326);
	sta(0x01);
	lda(0x3327);
	sbc_imm(0x00);
	sta(0x3327);
	sta(0x02);
J_2CE1:
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x0C);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x40);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  
B_2CF9:
	lda(0x3326);
	cmp_imm(0x51);
	bcs(B_2CCC);
  
	rts;
  
B_2D01:
	lda(0x3327);
	cmp_imm(0xBB);
	bcs(B_2D20);
  
B_2D08:
	lda(0x3326);
	clc;
	adc_imm(0x28);
	sta(0x3326);
	sta(0x01);
	lda(0x3327);
	adc_imm(0x00);
	sta(0x3327);
	sta(0x02);
	jmp(J_2CE1);
  
B_2D20:
	lda(0x3326);
	cmp_imm(0x31);
	bcc(B_2D08);
    }
  

}
end_subroutine

subroutine(P_ship_collision_detection_2D28)
{

	lda(0x041E);
	beq(B_2D33);
  
	lda_imm(0x07);
	sta(0x9F00);
	rts;
  
B_2D33:
	lda(0x3326);
	sec;
	sbc_imm(0x28);
	sta(0x00);
	lda(0x3327);
	sbc_imm(0x00);
	sta(0x01);
	ldy_imm(0x04);
B_2D44:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_2D82);
  
	dey;
	bne(B_2D44);
  
	ldy_imm(0x04);
	ldx_imm(0x12);
B_2D51:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_2D82);
  
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_2D51);
  
	lda(0x3326);
	clc;
	adc_imm(0x80);
	sta(0x00);
	lda(0x3327);
	adc_imm(0x02);
	sta(0x01);
	ldy_imm(0x04);
B_2D78:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_2D82);
  
	dey;
	bne(B_2D78);
  
	rts;
  
B_2D82:
	sound_play("ship_explodes");
	lda_imm(0xFF);
	sta(0x041E);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xF7);
	tay;
	jsr(P_sound_routine_5456);
	rts;
  

}
end_subroutine

subroutine(P_2D93)
{

	dec(0x3329);
	bpl(B_2D9D);
  
	lda_imm(0x03);
	sta(0x3329);
B_2D9D:
	ldx_imm(0x10);
	ldy_imm(0x30);
	lda(0x3329);
	sta(0x08);
	beq(B_2DB6);
  
B_2DA8:
	txa;
	clc;
	adc_imm(0x06);
	tax;
	tya;
	clc;
	adc_imm(0x05);
	tay;
	dec(0x08);
	bne(B_2DA8);
  
B_2DB6:
	lda_Y(0x9E00);
	bne(B_2DF1);
  
	lda_X(0x9E00);
	beq(B_2DF1);
  
	lda_imm(0x01);
	sta_Y(0x9E00);
	inx;
	iny;
	lda_X(0x9E00);
	cmp_imm(0x09);
	bcc(B_2DF1);
  
	sec;
	sbc_imm(0x09);
	sta_Y(0x9E00);
	inx;
	inx;
	inx;
	iny;
	lda_imm(0x00);
	sta_Y(0x9E00);
	iny;
	lda_X(0x9E00);
	clc;
	adc_imm(0xEE);
	sta_Y(0x9E00);
	inx;
	iny;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta_Y(0x9E00);
B_2DF1:
	rts;
  

}
end_subroutine

subroutine(P_2DF2)
{

	ldx_imm(0x30);
J_2DF4:
	lda_X(0x9E00);
	bne(B_2E03);
  
	txa;
	clc;
	adc_imm(0x05);
	tax;
	cpx_imm(0x44);
	bne(J_2DF4);
  
	rts;
  
B_2E03:
	inx;
	dec_X(0x9E00);
	bmi(B_2E5E);
  
	inx;
	dec_X(0x9E00);
	lda_X(0x9E00);
	sta(0x00);
	bpl(B_2E4F);
  
	lda_imm(0x02);
	sta_X(0x9E00);
	sta(0x00);
	inx;
	lda_X(0x9E00);
	sec;
	sbc_imm(0x01);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sbc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
J_2E32:
	lda_imm(0xCF);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	txa;
	pha;
	jsr(P_blit_to_screen_5285);
	pla;
	tax;
	inx;
	cpx_imm(0x44);
	bne(J_2DF4);
  
	rts;
  
B_2E4F:
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	jmp(J_2E32);
  
B_2E5E:
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	txa;
	pha;
	jsr(P_draw_rectangle_52CE);
	pla;
	tax;
	inx;
	cpx_imm(0x44);
	bne(B_2E8B);
  
	rts;
  
B_2E8B:
	jmp(J_2DF4);
  

}
end_subroutine

subroutine(P_2E8E)
{

	lda_imm(0x50);
	sta(0x01);
	lda_imm(0xBE);
	sta(0x02);
	lda_imm(0x11);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	sta(0x332A);
	lda_imm(0x01);
	sta(0xBDD8);
	sta(0xBE00);
	sta(0xBE28);
	jsr(J_2EB7);
}
end_subroutine

subroutine(J_2EB7)
{
	lda_imm(0x0D);
	sta(0x09);
	lda_imm(0xD9);
	sta(0x07);
	lda_imm(0xBD);
	sta(0x08);
B_2EC3:
	lda(0x332A);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x4C);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x09);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0x03);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_2EC3);
  
	rts;
  

}
end_subroutine

subroutine(P_2EF5)
{

	dec(0x332A);
	bpl(B_2EFF);
  
	lda_imm(0x02);
	sta(0x332A);
B_2EFF:
	sta(0x00);
	jmps(J_2EB7);
}
end_subroutine

subroutine(J_2F04)
{
	txa;
	pha;
	lda_X(0x9E00);
	cmp_imm(0xFF);
	bne(B_2F12);
  
	lda_imm(0x00);
	sta_X(0x9E00);
B_2F12:
	sta(0x08);
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x00);
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x38);
	sta(0x06);
	lda_imm(0x07);
	sta(0x07);
	jsr(P_screen_routine_53D5);
	pla;
	tax;
	inc_X(0x9E00);
	inc_X(0x9E00);
	lda_X(0x9E00);
	cmp_imm(0x0C);
	bcs(B_2F8A);
  
	sta(0x08);
	sta(0x13F6);
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x00);
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x34);
	sta(0x03);
	lda_imm(0x93);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x38);
	sta(0x06);
	lda_imm(0x07);
	sta(0x07);
	txa;
	pha;
P_5305(0);
	jsr(P_32FF);
	pla;
	tax;
	inx;
	cpx_imm(0x28);
	beq(B_GEN_3);
	jsr(J_2F90);
B_GEN_3:
	rts;
  
B_2F87:
	jmps(J_3053);
  
B_2F8A:
	inx;
	jmps(J_3053);
  

}
end_subroutine

subroutine(P_2F8E)
{

	ldx_imm(0x10);
	jsr(J_2F90);
}
end_subroutine

subroutine(J_2F90)
{
	lda_X(0x9E00);
	cmp_imm(0x01);
	beq(B_2FA6);
  
	bcs(B_2FA3);
  
	txa;
	clc;
	adc_imm(0x06);
	tax;
	cpx_imm(0x28);
	beq(B_GEN_1);
	jsr(J_2F90);
B_GEN_1:
	rts;
  
B_2FA3:
	jmps(J_2F04);
  
B_2FA6:
	inx;
	dec_X(0x9E00);
	bne(B_GEN_3);
	jmpl(J_3053);
B_GEN_3:
	inx;
	dec_X(0x9E00);
	lda_X(0x9E00);
	sta(0x00);
	bpl(B_2FF3);
  
	lda_imm(0x02);
	sta_X(0x9E00);
	sta(0x00);
	inx;
	inx;
	lda_X(0x9E00);
	sec;
	sbc_imm(0x01);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sbc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
J_2FD6:
	lda_imm(0x34);
	sta(0x03);
	lda_imm(0x93);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x38);
	sta(0x06);
	txa;
	pha;
	jsr(P_blit_to_screen_5285);
	pla;
	tax;
	inx;
	cpx_imm(0x28);
	beq(B_GEN_2);
	jsr(J_2F90);
B_GEN_2:
	rts;
  
B_2FF3:
	inx;
	lda_X(0x9E00);
	bmi(B_3029);
  
	inx;
	lda_X(0x9E00);
	sec;
	sbc_imm(0x28);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sbc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
	cmp_imm(0xA0);
	beq(B_3017);
  
	jmp(J_2FD6);
  
B_3017:
	lda(0x01);
	cmp_imm(0x28);
	bcs(J_2FD6);
  
	lda_imm(0xFF);
	dex;
	dex;
	sta_X(0x9E00);
	inx;
	inx;
	jmp(J_2FD6);
  
B_3029:
	inx;
	lda_X(0x9E00);
	clc;
	adc_imm(0x28);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
	cmp_imm(0xBB);
	bcs(B_3047);
  
	jmp(J_2FD6);
  
B_3047:
	lda_imm(0x01);
	dex;
	dex;
	sta_X(0x9E00);
	inx;
	inx;
	jmp(J_2FD6);
}
end_subroutine

subroutine(J_3053)
{
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	inx;
	inx;
	inx;
	lda_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x38);
	sta(0x06);
	txa;
	pha;
	jsr(P_draw_rectangle_52CE);
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x02);
	tay;
	jsr(P_sound_routine_5456);
	pla;
	tax;
	inx;
	cpx_imm(0x28);
	bne(B_308C);
  
	rts;
  
B_308C:
	jmps(J_2F90);
  

}
end_subroutine

subroutine(P_308F)
{

	ldx_imm(0x10);
B_3091:
	lda_X(0x9E00);
	beq(B_30A0);
  
	txa;
	clc;
	adc_imm(0x06);
	tax;
	cpx_imm(0x28);
	bne(B_3091);
  
	rts;
  
B_30A0:
	lda_imm(0x01);
	sta_X(0x9E00);
	inx;
	lda_imm(0x6A);
	sta_X(0x9E00);
	inx;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	lda(0x6189);
	bpl(B_30BE);
  
	lda_imm(0x01);
	sta_X(0x9E00);
	bne(B_30C3);
  
B_30BE:
	lda_imm(0xFF);
	sta_X(0x9E00);
B_30C3:
	inx;
	txa;
	pha;
	lda(0x618A);
	and_imm(0x1F);
	sta(0x00);
	cmp_imm(0x02);
	bcc(B_30D5);
  
	dec(0x00);
	dec(0x00);
B_30D5:
	lda_imm(0xF0);
	sta(0x01);
	jsr(P_541C);
	pla;
	tax;
	lda(0x02);
	clc;
	adc_imm(0x4B);
	sta_X(0x9E00);
	inx;
	lda(0x03);
	adc_imm(0xA0);
	sta_X(0x9E00);
	rts;
  

}
end_subroutine

subroutine(P_30EF)
{

	lda_imm(0x01);
	sta(0x332E);
	lda_imm(0x6C);
	sta(0x3332);
	lda_imm(0x00);
	sta(0x00);
	sta(0x332F);
	lda_imm(0xC4);
	sta(0x01);
	sta(0x3330);
	lda_imm(0xAF);
	sta(0x02);
	sta(0x3331);
	lda(0x6189);
	bpl(B_311B);
  
	lda_imm(0xFF);
	sta(0x3333);
	jmps(J_3120);
  
B_311B:
	lda_imm(0x01);
	sta(0x3333);
	jsr(J_3120);
}
end_subroutine

subroutine(J_3120)
{
	lda_imm(0xDC);
	sta(0x03);
	lda_imm(0x93);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x41);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_3134)
{

	lda(0x3334);
	cmp_imm(0x01);
	bne(B_313E);
  
J_31DF:
	jsr(P_31FB);
	inc(0x3335);
	lda(0x3335);
	cmp_imm(0x14);
	beq(B_31F0);
  
	jsr(P_321E);
	rts;
  
B_31F0:
	lda_imm(0x02);
	sta(0x3334);
	lda_imm(0x28);
	sta(0x3335);
	rts;
  
B_313E:
	cmp_imm(0x02);
	bne(B_3145);
  
J_3249:
	jsr(P_325E);
	dec(0x3335);
	jsr(P_3281);
	lda(0x3335);
	beq(B_3258);
  
	rts;
  
B_3258:
	lda_imm(0x40);
	sta(0x041F);
	rts;
  
B_3145:
	dec(0x3332);
	beq(B_3173);
  
	dec(0x332F);
	lda(0x332F);
	sta(0x00);
	bpl(B_3192);
  
	lda_imm(0x02);
	sta(0x00);
	sta(0x332F);
	lda(0x3330);
	sec;
	sbc_imm(0x01);
	sta(0x3330);
	sta(0x01);
	lda(0x3331);
	sbc_imm(0x00);
	sta(0x3331);
	sta(0x02);
	jmps(J_3120);
  
B_3173:
	lda_imm(0x00);
	sta(0x332E);
	lda(0x3330);
	sta(0x01);
	lda(0x3331);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x41);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  
B_3192:
	lda(0x3333);
	bmi(B_31B3);
  
	lda(0x3330);
	sec;
	sbc_imm(0x28);
	sta(0x3330);
	sta(0x01);
	lda(0x3331);
	sbc_imm(0x00);
	sta(0x3331);
	sta(0x02);
	cmp_imm(0xA1);
	bcc(B_31CF);
  
	jmps(J_3120);
  
B_31B3:
	lda(0x3330);
	clc;
	adc_imm(0x28);
	sta(0x3330);
	sta(0x01);
	lda(0x3331);
	adc_imm(0x00);
	sta(0x3331);
	sta(0x02);
	cmp_imm(0xBE);
	bcs(B_31CF);
  
	jmps(J_3120);
  
B_31CF:
	lda_imm(0x01);
	sta(0x3333);
	jmps(J_3120);
  

}
end_subroutine

subroutine(E_31D7)
{

	lda_imm(0xFF);
	sta(0x3333);
	jmps(J_3120);


}
end_subroutine

subroutine(P_31FB)
{

	lda(0x3330);
	sta(0x01);
	lda(0x3331);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x41);
	sta(0x06);
	lda_imm(0x06);
	sta(0x07);
	lda(0x3335);
	sta(0x08);
	jsr(P_screen_routine_53D5);
	rts;
  

}
end_subroutine

subroutine(P_321E)
{

	lda_imm(0x00);
	sta(0x00);
	lda(0x3330);
	sta(0x01);
	lda(0x3331);
	sta(0x02);
	lda_imm(0xDC);
	sta(0x03);
	lda_imm(0x93);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x41);
	sta(0x06);
	lda_imm(0x06);
	sta(0x07);
	lda(0x3335);
	sta(0x08);
P_5305(0);
	rts;
  

  

}
end_subroutine

subroutine(P_325E)
{

	lda(0x3330);
	sta(0x01);
	lda(0x3331);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	lda_imm(0x03);
	sta(0x07);
	lda(0x3335);
	sta(0x08);
	jsr(P_screen_routine_53D5);
	rts;
  

}
end_subroutine

subroutine(P_3281)
{

	lda_imm(0x00);
	sta(0x00);
	lda(0x3330);
	sta(0x01);
	lda(0x3331);
	sta(0x02);
	lda_imm(0x83);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	lda_imm(0x03);
	sta(0x07);
	lda(0x3335);
	sta(0x08);
P_5305(0);
	rts;
  

}
end_subroutine

subroutine(P_32AC)
{

	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x01);
	sta(0x3336);
	sta(0x04);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xF7);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x00);
	sta(0x3337);
	rts;
  

}
end_subroutine

subroutine(P_32CB)
{

	lda(0x3336);
	beq(B_32E4);
  
	inc(0x3336);
	lda(0x3336);
	sta(0x04);
	cmp_imm(0x1F);
	beq(B_32E4);
  
	lda_imm(0x04);
	sta(0x03);
	jsr(P_sound_routine_5482);
	rts;
  
B_32E4:
	lda(0x3337);
	bne(B_32FE);
  
	lda_imm(0x00);
	sta(0x3336);
	lda_imm(0x01);
	sta(0x3337);
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x08);
	tay;
	jsr(P_sound_routine_5456);
B_32FE:
	rts;
  

}
end_subroutine

subroutine(P_32FF)
{

	lda(0x13F6);
	sta(0x01);
	lda_imm(0x32);
	sta(0x00);
	jsr(P_541C);
	lda(0x02);
	sta(0x04);
	lda(0x03);
	sta(0x05);
	lda_imm(0x03);
	sta(0x04);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFD);
	tay;
	jsr(P_sound_routine_5456);
	rts;
  

}
end_subroutine


subroutine(P_reptile_level_1400)
{

	jsr(P_11BF);
	ldx_imm(0x0E);
	lda_imm(0x03);
B_3407:
	sta_X(0x9F00);
	dex;
	bpl(B_3407);
  
	lda_imm(0x16);
	sta(0x618B);
	lda_imm(0x00);
	sta(0x618C);
	jsr(P_55D3);
	lda_imm(0x08);
	sta(0x617E);
	lda_imm(0x09);
	sta(0x617F);
	sta(0x6180);
	lda_imm(0x01);
	sta(0x617D);
	jsr(P_56B9);
	lda_imm(0xE9);
	sta(0x617B);
	lda_imm(0xA8);
	sta(0x617C);
	lda_imm(0x00);
	sta(materialize_bridge_part_flag_617A);
	lda_imm(0x00);
	ldx_imm(0x00);
B_3442:
	sta_X(0x9E00);
	dex;
	bne(B_3442);
  
	sta(0x041F);
	sta(0x041E);
	sta(0x13EF);
	sta(0x3EA4);
	sta(0x3E99);
	sta(0x3E9A);
	sta(0x3E9C);
	sta(laser_beam_index_3E9F);
	lda_imm(0x01);
	sta(0x3EA6);
	sta(0x3EA1);
	sta(0x9F02);
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x14);
	sta(0x13E5);
	lda_imm(0x15);
	sta(0x13E6);
	lda_imm(0xD7);
	sta(0x13E3);
	lda(0x94B1);
	sta(0x3E9B);
	lda(0x94A5);
	sta(0x13FC);
	jsr(P_09F2);
	jsr(P_3D5F);
	jsr(P_set_hero_initial_pos_09B2);
	lda_imm(0x00);
	sta(0x3EAE);
	jsr(P_3A98);
	lda_imm(0xA4);
	sta(0x07);
	lda_imm(0xB8);
	sta(0x08);
	lda_imm(0x00);
	sta(0x09);
	jsr(P_5824);
	lda_imm(0xDE);
	sta(0x07);
	lda_imm(0xA4);
	sta(0x08);
	lda_imm(0x04);
	sta(0x09);
	jsr(P_5824);

	OricTimer tmr = timer_init(5,15);
	set_game_context(GAME_REPTILE);
	LOOP_PLAY(elevator);

	int just_died = 1;
	
	while(1)
	{
	    
	    ldx(0x13FC);

	    update_controls();
	    timer_tick(&tmr);
  
	    lda(0x041E);
	    bne(B_34D3);
  
	    dec(0x9F00);
	    bne(B_34D3);
  
	    lda(0x94A8);
	    sta(0x9F00);
	    jsr(P_move_elevator_0A36);
	    B_34D3:
	    dec(0x9F01);
	    bne(B_3511);
  
	    lda_imm(0x1E);
	    sta(0x9F01);
	    lda(0x3E99);
	    bne(B_34F7);
  
	    lda_imm(0xA4);
	    sta(0x07);
	    lda_imm(0xB8);
	    sta(0x08);
	    lda_imm(0x00);
	    sta(0x09);

	    jsr(P_animate_quadnog_1_5954);
#ifndef VERY_EASY_GAME
	    jsr(P_quadnog_green_shots_39D5);
#endif
	    P_quadnog_red_and_white_shots_37EA(0,0);
	    B_34F7:
	    lda(0x3E9A);
	    bne(B_3511);
  
	    lda_imm(0xDE);
	    sta(0x07);
	    lda_imm(0xA4);
	    sta(0x08);
	    lda_imm(0x04);
	    sta(0x09);
	    jsr(P_animate_quadnog_1_5954);
	    jsr(P_destroy_quadnog_3900);
	    P_36E4(0);
	    
	    B_3511:
	    lda(0x3E99);
	    beq(B_3525);
  
	    bmi(B_3525);
  
	    dec(0x9F09);
	    bne(B_3525);
  
	    lda_imm(0x4B);
	    sta(0x9F09);
	    jsr(P_3C7E);
	    B_3525:
	    lda(0x3E9A);
	    beq(B_3539);
  
	    bmi(B_3539);
  
	    dec(0x9F0A);
	    bne(B_3539);
  
	    lda_imm(0x4B);
	    sta(0x9F0A);
	    jsr(P_3CC4);
	    B_3539:
	    lda(0x041E);
	    bne(B_3558);
  
	    dec(0x9F03);
	    bne(B_3558);
  
	    lda(0x13D6);
	    sta(0x9F03);

	    // animate hero
	    
	    P_handle_hero();

	    jsr(P_3D19);
	    jsr(P_3658);
	    
	    B_3558:
	    lda(0x3EAD);
	    bmi(B_3572);
  
	    dec(0x9F02);
	    bne(B_3572);
  
	    lda_imm(0x1E);
	    sta(0x9F02);
	    lda(0x3EAD);
	    bne(B_356F);
  
	    jsr(P_3BCF);
	    B_356F:
	    jsr(P_3B6D);
	    B_3572:
	    lda(0x3EAD);
	    bne(B_3584);
  
	    dec(0x9F04);
	    bne(B_3584);
  
	    lda_imm(0x14);
	    sta(0x9F04);
	    jsr(P_3C44);
	    B_3584:
	    
	    lda(0x041E);
	    bne(B_359A);

	    dec(0x9F05);
	    bne(B_359A);
  
	    lda(0x94B6);
	    sta(0x9F05);
	    jsr(P_3A07);
	    jsr(P_3935);
	    
	    B_359A:
	    lda(0x041E);
	    beq(B_35B3);
  
	    dec(0x9F06);
	    bne(B_35B3);
  
	    lda(0x13D6);
	    sta(0x9F06);
	    
	    if (just_died)
	    {
		just_died = 0;
		play_kill_sound();
	    }

	    jsr(P_deadly_fall_12A7);
	    jsr(P_fall_sound_139B);
	    
	    jsr(P_3D19);
	    B_35B3:
	    
	    dec(0x9F07);
	    bne(B_35C4);
  
	    lda(0x94B6);
	    sta(0x9F07);
	    P_quadnog_red_and_white_shots_37EA(1,0);
	    P_36E4(1);
	    B_35C4:
	    lda(materialize_bridge_part_flag_617A);
	    beq(B_35D6);
  
	    dec(0x9F08);
	    bne(B_35D6);
  
	    lda_imm(0x0A);
	    sta(0x9F08);
	    jsr(P_materialize_bridge_part_5521);
	B_35D6:
	    lda(0x617D);
	    beq(B_35E3);
  
	    dec(0x9F0B);
	    bne(B_35E3);
  
	    jsr(P_5686);
	    B_35E3:
	    lda(0x041E);
	    beq(B_35F0);
  
	    jsr(P_3E69);
	    lda_imm(0x01);
	    sta(0x3EAE);
	    B_35F0:
	    lda(0x041E);
	    cmp_imm(0x02);
	    beq(B_360E);
  
	    lda(0x041F);
	    cmp_imm(0xFF);
	    bne(B_360E);
  
	    jsr(P_3E69);
	    jsr(P_1360);
	    lda_imm(0x02);
	    sta(0x041E);
	    lda_imm(0x01);
	    sta(0x041F);

	    B_360E:
	
	    lda(0x041F);
	    if ((r.a == 0x80) || (r.a == 0x40))
	    {
		break;
	    }
	
 	    if (handle_escape()) break;

  

	}

	LOOP_STOP(elevator);

}
end_subroutine

subroutine(P_3631)
{

	lda(0x3E9A);
	bpl(B_3657);
  
	lda(0x3EAD);
	bpl(B_3657);
  
	lda(0x617B);
	sec;
	sbc_imm(0xE0);
	sta(0x01);
	lda(0x617C);
	sbc_imm(0x01);
	sta(0x02);
	ldy_imm(0x03);
	lda_ind_Y(0x01);
	cmp_imm(0x40);
	bcs(B_3657);
  
	lda_imm(0x40);
	sta(0x041F);
B_3657:
	rts;
  

}
end_subroutine

subroutine(P_3658)
{

	lda(0x041F);
	bne(B_3675);
  
	jsr(P_36B4);
	jsr(P_1203);
	jsr(P_3683);
	jsr(P_3676);
	jsr(P_36A7);
	jsr(P_3631);
	jsr(P_3B20);
	jsr(P_3B51);
B_3675:
	rts;
  

}
end_subroutine

subroutine(P_3676)
{

	lda(hero_pos_13D4_plus_1);
	cmp_imm(0xA1);
	bcs(B_3682);

	move_ff_41f();
B_3682:
	rts;
  

}
end_subroutine

subroutine(P_3683)
{

	lda(0x3E99);
	bmi(B_3695);
  
	lda(0xBA31);
	cmp_imm(0x41);
	bcc(B_3695);

	move_ff_41f();
	rts;
  
B_3695:
	lda(0x3E9A);
	bmi(B_36A6);
  
	lda(0xA69F);
	cmp_imm(0x41);
	bcc(B_36A6);

	move_ff_41f();
B_36A6:
	rts;
  

}
end_subroutine

subroutine(P_36A7)
{

	lda(0xA700);
	cmp_imm(0x41);
	bcc(B_3682);
  
	lda_imm(0x01);
	sta(0x3E9C);
B_3682:
	rts;
  

}
end_subroutine

subroutine(P_36B4)
{

	lda(0x3EAD);
	bmi(B_36CB);
  
	lda(0xAE13);
	cmp_imm(0x41);
	bcc(B_36CB);
  
	lda_imm(0xFF);
	sta(0x3EAD);
	jsr(P_36CC);
	jsr(P_display_bridge_part_5501);
B_36CB:
	rts;
  

}
end_subroutine

subroutine(P_36CC)
{

	ldx_imm(0x03);
	stx(0x09);
B_36D0:
	ldx(0x09);
	lda_imm(0x40);
	jsr(P_erase_beam_3C10);
	dec(0x09);
	bpl(B_36D0);
  
	lda(0x13FC);
	adc_imm(0x32);
	sta(0x13FC);
B_36E3:
	rts;
  

}
end_subroutine

void P_36E4(int to_375e)
{

    if (!to_375e)
    {
	lda(0x3E9C);
	beq(B_36E3);
  
	dec(0x3EA3);
	bne(B_36E3);
  
	lda_imm(0x1E);
	sta(0x3EA3);
	lda(0x3EA4);
	bne(B_36E3);
  
	lda_imm(0x01);
	sta(0x3EA4);
	dec(0x3EA6);
	bne(B_370F);
  
	lda_imm(0x02);
	sta(0x3EA6);
	lda_imm(0x07);
	sta(0x3EA5);
	jmp(J_3714);
  
B_370F:
	lda_imm(0x01);
	sta(0x3EA5);
J_3714:
	lda_imm(0x00);
	sta(0x3EA7);
	sta(0x3EA8);
B_36E3:
	rts;
  
B_371D:
	dec(0x3EA7);
	lda(0x3EA7);
	sta(0x00);
	bpl(B_375B);
  
	lda_imm(0x02);
	sta(0x3EA7);
	sta(0x00);
	dec(0x3EA8);
	lda(0x3EA8);
	sta(0x01);
	beq(B_373B);
  
	jmps(J_378E);
  
B_373B:
	txa;
	pha;
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xDF);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x01);
	sta(0x3E9A);
	lda(0x13FC);
	clc;
	adc_imm(0x1E);
	sta(0x13FC);
	pla;
	tax;
	jmps(J_37D5);
  
B_375B:
	jmps(J_37CD);
    }
    
// entrypoint of P_375E

	lda(0x3E9C);
	beq(B_376E);
  
	lda(0x3EA4);
	cmp_imm(0x01);
	beq(B_376F);
  
	cmp_imm(0x02);
	beq(B_371D);
  
B_376E:
	rts;
  
B_376F:
	inc(0x3EA7);
	lda(0x3EA7);
	sta(0x00);
	cmp_imm(0x03);
	bne(J_37CD);
  
	lda_imm(0x00);
	sta(0x3EA7);
	sta(0x00);
	inc(0x3EA8);
	lda(0x3EA8);
	sta(0x01);
	cmp_imm(0x08);
	bcs(J_37D5);
	J_378E();
	return;

  
	J_37D5:
	J_37D5();
	return;

J_37CD:
	lda(0x3EA8);
	sta(0x01);
	jmps(J_378E);
}

void J_37CD()
{
	lda(0x3EA8);
	sta(0x01);
	jmps(J_378E);
}

subroutine(J_378E)
{
	ldx(0x01);
	lda_imm(0xEA);
	ldy(0x00);
	beq(B_379C);
  
B_3796:
	clc;
	adc_imm(0x02);
	dey;
	bne(B_3796);
  
B_379C:
	sta(0x01);
	lda_imm(0x90);
	sta(0x02);
	ldy_imm(0x00);
	lda(0x3EA5);
	sta_X(0xA5FC);
	inx;
	lda_ind_Y(0x01);
	sta_X(0xA5FC);
	inx;
	iny;
	lda_ind_Y(0x01);
	sta_X(0xA5FC);
	inx;
	lda_X(0xA5FC);
	cmp_imm(0x41);
	bcc(B_37C2);
  
	jsr(P_38CF);
B_37C2:
	lda_X(0xA623);
	cmp_imm(0x41);
	bcc(B_37CC);
  
	jsr(P_38CF);
B_37CC:
	rts;


}
end_subroutine

subroutine(J_37D5)
{
	ldx(0x01);
	lda_imm(0x40);
	sta_X(0xA5FC);
	inx;
	sta_X(0xA5FC);
	inx;
	sta_X(0xA5FC);
	lda_imm(0x00);
	sta(0x3EA4);
	rts;
  

}
end_subroutine

void P_quadnog_red_and_white_shots_37EA(int to_3865,int to_3891)
{
    if (!to_3891)
    {

	if (!to_3865)
	{
	dec(0x3EA3);
	bne(B_37E9);
  
	lda_imm(0x1E);
	sta(0x3EA3);
	lda(0x3EA4);
	bne(B_37E9);
  
	lda_imm(0x01);
	sta(0x3EA4);
	dec(0x3EA6);
	bne(B_3810);
  
	lda_imm(0x02);
	sta(0x3EA6);
	lda_imm(0x07);
	sta(0x3EA5);
	jmp(J_3815);
  
B_3810:
	lda_imm(0x01);
	sta(0x3EA5);
J_3815:
	lda_imm(0x02);
	sta(0x3EA7);
	lda_imm(0x07);
	sta(0x3EA8);
B_37E9:
	rts;

J_38EA:
	ldx(0x01);
	lda_imm(0x40);
	sta_X(0xB9B1);
	inx;
	sta(0xB9B1);
	inx;
	sta_X(0xB9B1);
	lda_imm(0x00);
	sta(0x3EA4);
	rts;
	
B_3820:
	inc(0x3EA7);
	lda(0x3EA7);
	sta(0x00);
	cmp_imm(0x03);
	bne(B_3862);
  
	lda_imm(0x00);
	sta(0x3EA7);
	sta(0x00);
	inc(0x3EA8);
	lda(0x3EA8);
	sta(0x01);
	cmp_imm(0x0A);
	bcs(B_3842);
  
	P_quadnog_red_and_white_shots_37EA(1,1);
	rts;
	
B_3842:
	txa;
	pha;
	lda_imm(0x01);
	sta(0x3E99);
	lda(0x13FC);
	clc;
	adc_imm(0x1E);
	sta(0x13FC);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xDF);
	tay;

	sound_play("quadnog_dies");

	jsr(P_sound_routine_5456);
	pla;
	tax;
	jmp(J_38EA);
  
B_3862:
	lda(0x3EA8);
	sta(0x01);
	jmp(J_3891);

    }


	lda(0x3E9C);
	bne(B_3875);
  
	lda(0x3EA4);
	cmp_imm(0x01);
	beq(B_3876);
  
	cmp_imm(0x02);
	beq(B_3820);
  
B_3875:
	rts;
  
B_3876:
	dec(0x3EA7);
	lda(0x3EA7);
	sta(0x00);
	
	bmi(B_GEN_4);
	lda(0x3EA8);
	sta(0x01);
	jmp(J_3891);
B_GEN_4:
	lda_imm(0x02);
	sta(0x3EA7);
	sta(0x00);
	dec(0x3EA8);
	lda(0x3EA8);
	sta(0x01);
	beq(J_38EA);
    }
    
J_3891:
	ldx(0x01);
	lda_imm(0xEA);
	ldy(0x00);
	beq(B_389F);
  
B_3899:
	clc;
	adc_imm(0x02);
	dey;
	bne(B_3899);
  
B_389F:
	sta(0x01);
	lda_imm(0x90);
	sta(0x02);
	ldy_imm(0x00);
	lda_X(0xB9B1);
	cmp_imm(0x41);
	bcc(B_38B1);
  
	jsr(P_38CF);
B_38B1:
	lda_X(0xB9DB);
	cmp_imm(0x41);
	bcc(B_38BB);
  
	jsr(P_38CF);
B_38BB:
	lda(0x3EA5);
	sta_X(0xB9B1);
	inx;
	lda_ind_Y(0x01);
	sta_X(0xB9B1);
	inx;
	iny;
	lda_ind_Y(0x01);
	sta_X(0xB9B1);
	rts;
  

}


subroutine(P_38CF)
{

	lda(0x3EA5);
	cmp_imm(0x01);
	beq(B_38DC);
  
	lda_imm(0x02);
	sta(0x3EA4);
	rts;
  
B_38DC:
move_ff_41f();
	rts;
  
J_38E2:
	lda(0x3EA8);
	sta(0x01);
	P_3865(1);
	return;
  
J_38EA:
	ldx(0x01);
	lda_imm(0x40);
	sta_X(0xB9B1);
	inx;
	sta(0xB9B1);
	inx;
	sta_X(0xB9B1);
	lda_imm(0x00);
	sta(0x3EA4);
	rts;
  
B_38FF:
	rts;
  

}
end_subroutine

subroutine(P_destroy_quadnog_3900)
{

	lda(0x3E9C);
	beq(B_38FF);
  
	dec(0x3EA2);
	bne(B_38FF);
  
	lda_imm(0x17);
	sta(0x3EA2);
	lda(0x3EA5);
	cmp_imm(0x07);
	beq(B_38FF);
  
	ldx_imm(0x00);
B_3918:
	lda_X(0x9E00);
	beq(B_3925);
  
	inx;
	inx;
	inx;
	cpx_imm(0x09);
	bne(B_3918);
  
B_38FF:
	rts;
  
B_3925:
	lda_imm(0x01);
	sta_X(0x9E00);
	inx;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	sta_X(0x9E00);
	rts;
  

}
end_subroutine

subroutine(P_3935)
{

	lda(0x3E9C);
	beq(B_3948);
  
	ldx_imm(0x00);
J_393C:
	lda_X(0x9E00);
	bne(B_3949);
  
	inx;
	inx;
	inx;
	cpx_imm(0x09);
	bne(J_393C);
  
B_3948:
	rts;
  
B_3949:
	inx;
	inc_X(0x9E00);
	lda_X(0x9E00);
	sta(0x00);
	cmp_imm(0x03);
	bne(B_39A8);
  
	lda_imm(0x00);
	sta_X(0x9E00);
	sta(0x00);
	inx;
	inc_X(0x9E00);
	lda_X(0x9E00);
	sta(0x01);
	cmp_imm(0x14);
	bcs(B_39B3);
  
	txa;
	pha;
J_396C:
	ldx(0x01);
	lda_imm(0xEA);
	ldy(0x00);
	beq(B_397A);
  
B_3974:
	clc;
	adc_imm(0x02);
	dey;
	bne(B_3974);
  
B_397A:
	sta(0x01);
	lda_imm(0x90);
	sta(0x02);
	ldy_imm(0x00);
	lda_imm(0x02);
	sta_X(0xA8A4);
	inx;
	lda_ind_Y(0x01);
	sta_X(0xA8A4);
	inx;
	iny;
	lda_ind_Y(0x01);
	sta_X(0xA8A4);
	lda_X(0xA8A5);
	cmp_imm(0x41);
	bcc(B_39A0);

	move_ff_41f();
B_39A0:
	pla;
	tax;
	inx;
	cpx_imm(0x09);
	bne(J_393C);
  
	rts;
  
B_39A8:
	inx;
	lda_X(0x9E00);
	sta(0x01);
	txa;
	pha;
	jmp(J_396C);
  
B_39B3:
	dex;
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	lda_imm(0x40);
	ldy(0x01);
	sta_Y(0xA8A4);
	iny;
	sta_Y(0xA8A4);
	iny;
	sta_Y(0xA8A4);
	inx;
	inx;
	inx;
	cpx_imm(0x09);
	bne(B_39D1);
  
	rts;
  
B_39D1:
	jmp(J_393C);
  
B_39D4:
	rts;
  

}
end_subroutine

subroutine(P_quadnog_green_shots_39D5)
{

	dec(0x3EA2);
	bne(B_39D4);
  
	lda_imm(0x17);
	sta(0x3EA2);
	lda(0x3EA5);
	cmp_imm(0x07);
	beq(B_39D4);
  
	ldx_imm(0x00);
B_39E8:
	lda_X(0x9E00);
	beq(B_39F5);
  
	inx;
	inx;
	inx;
	cpx_imm(0x09);
	bne(B_39E8);
  
B_39D4:
	rts;
  
B_39F5:
	lda_imm(0x01);
	sta_X(0x9E00);
	inx;
	lda_imm(0x02);
	sta_X(0x9E00);
	inx;
	lda_imm(0x17);
	sta_X(0x9E00);
	rts;
  

}
end_subroutine

subroutine(P_3A07)
{

	lda(0x3E9C);
	bne(B_3A1A);
  
	ldx_imm(0x00);
	jmps(J_3A0E);
B_3A1A:
	rts;
}
end_subroutine
subroutine(J_3A0E)
{
	lda_X(0x9E00);
	bne(B_3A1B);
  
	inx;
	inx;
	inx;
	cpx_imm(0x09);
	bne(J_3A0E);
  
B_3A1A:
	rts;
  
B_3A1B:
	inx;
	dec_X(0x9E00);
	lda_X(0x9E00);
	sta(0x00);
	bpl(B_3A70);
  
	lda_imm(0x02);
	sta_X(0x9E00);
	sta(0x00);
	inx;
	dec_X(0x9E00);
	beq(B_3A7B);
  
	lda_X(0x9E00);
	sta(0x01);
	txa;
	pha;
	jsr(J_3A3A);
	return;
B_3A70:
	inx;
	lda_X(0x9E00);
	sta(0x01);
	txa;
	pha;
	jmps(J_3A3A);
B_3A7B:
	dex;
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	lda_imm(0x40);
	ldy_imm(0x00);
	sta_Y(0xBC4A);
	iny;
	sta_Y(0xBC4A);
	inx;
	inx;
	inx;
	cpx_imm(0x09);
	bne(B_3A95);
  
	rts;
  
B_3A95:
	jmps(J_3A0E);
  

}

end_subroutine

subroutine(J_3A3A)
{
	ldx(0x01);
	lda_imm(0xEA);
	ldy(0x00);
	beq(B_3A48);
  
B_3A42:
	clc;
	adc_imm(0x02);
	dey;
	bne(B_3A42);
  
B_3A48:
	sta(0x01);
	lda_imm(0x90);
	sta(0x02);
	ldy_imm(0x00);
	lda_X(0xBC49);
	cmp_imm(0x41);
	bcc(B_3A5C);

	move_ff_41f();
B_3A5C:
	lda_ind_Y(0x01);
	sta_X(0xBC4A);
	inx;
	iny;
	lda_ind_Y(0x01);
	sta_X(0xBC4A);
	pla;
	tax;
	inx;
	cpx_imm(0x09);
	beq(B_GEN_5);
	jmps(J_3A0E);
B_GEN_5:
	rts;
  
B_3A70:
	inx;
	lda_X(0x9E00);
	sta(0x01);
	txa;
	pha;
	jmps(J_3A3A);
}
end_subroutine



subroutine(P_3A98)
{

	lda_imm(0xA8);
	sta(0x07);
	lda_imm(0xAA);
	sta(0x08);
	lda_imm(0x04);
	sta(0x09);
B_3AA4:
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0xF0);
	sta(0x03);
	lda_imm(0x8D);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x30);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0x06);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_3AA4);
  
	lda_imm(0x00);
	sta(0x3EA0);
	sta(laser_beam_index_3E9F);
	lda_imm(0x01);
	sta(0x3EA1);
	ldx_imm(0x04);
	lda_imm(0x00);
B_3AE5:
	sta_X(0x3EA9);
	dex;
	bpl(B_3AE5);
  
	sta(0x3EAD);
	rts;
  

}
end_subroutine

subroutine(P_3AEF)
{

	ldx_imm(0x03);
	ldy_imm(0x12);
B_3AF3:
	lda_X(0x3EA9);
	bne(B_3AFF);
  
	lda_Y(0xB070);
	cmp_imm(0x41);
	bcs(B_3B08);
  
B_3AFF:
	tya;
	sec;
	sbc_imm(0x06);
	tay;
	dex;
	bpl(B_3AF3);
  
	rts;
  
B_3B08:
	lda_imm(0x01);
	sta_X(0x3EA9);
	cpx_imm(0x00);
	bne(B_3B16);
  
	lda_imm(0x01);
	sta(0x3EAD);
B_3B16:
	lda(0x3E9B);
	clc;
	adc_imm(0x06);
	sta(0x3E9B);
	rts;
  

}
end_subroutine

subroutine(P_3B20)
{

	ldx_imm(0x03);
	ldy_imm(0x12);
B_3B24:
	lda_X(0x3EA9);
	bne(B_3B32);
  
J_3B29:
	tya;
	sec;
	sbc_imm(0x06);
	tay;
	dex;
	bpl(B_3B24);
  
	rts;
  
B_3B32:
	lda_Y(0xB072);
	cmp_imm(0x11);
	bcc(B_3B40);
  
	cmp_imm(0x18);
	bcs(B_3B40);
  
	jmp(J_3B29);
  
B_3B40:
	lda(0x3EAD);
	bmi(B_3B50);
  
	jsr(P_36CC);
	lda_imm(0xFF);
	sta(0x3EAD);
	move_ff_41f();
B_3B50:
	rts;
  

}
end_subroutine

subroutine(P_3B51)
{

	ldy_imm(0x00);
	ldx(laser_beam_index_3E9F);
	txa;
	beq(B_3B65);
  
	cmp_imm(0x04);
	beq(B_3B6C);
  
B_3B5D:
	tya;
	clc;
	adc_imm(0x06);
	tay;
	dex;
	bne(B_3B5D);
  
B_3B65:
	lda_Y(0xB072);
	cmp_imm(0x7F);
	bne(B_3B40);
  
B_3B6C:
	rts;
B_3B40:
	lda(0x3EAD);
	bmi(B_3B50);
  
	jsr(P_36CC);
	lda_imm(0xFF);
	sta(0x3EAD);
	move_ff_41f();
B_3B50:
	rts;  

}
end_subroutine

subroutine(P_3B6D)
{

	inc(0x3E9E);
	lda(0x3E9E);
	cmp_imm(0x18);
	bcc(B_3B7C);
  
	lda_imm(0x11);
	sta(0x3E9E);
B_3B7C:
	sta(0x06);
	lda_imm(0xB2);
	sta(0x02);
	lda_imm(0xAC);
	sta(0x03);
	ldx_imm(0x00);
B_3B88:
	lda_X(0x3EA9);
	beq(B_3BC2);
  
	ldy_imm(0x00);
	lda_imm(0x2C);
	sta(0x07);
	lda(0x02);
	sta(0x00);
	lda(0x03);
	sta(0x01);
B_3B9B:
	lda(0x06);
	sta_ind_Y(0x00);
	iny;
	lda_imm(0x10);
	sta_ind_Y(0x00);
	dey;
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dec(0x06);
	lda(0x06);
	cmp_imm(0x10);
	bne(B_3BBE);
  
	lda_imm(0x17);
	sta(0x06);
B_3BBE:
	dec(0x07);
	bne(B_3B9B);
  
B_3BC2:
	lda(0x02);
	clc;
	adc_imm(0x06);
	sta(0x02);
	inx;
	cpx_imm(0x04);
	bne(B_3B88);
  
	rts;
  

}
end_subroutine

subroutine(P_3BCF)
{

    jsr(P_3AEF);
	dec(0x3EA1);
	bne(B_3C04);
  
	lda(0x3E9B);
	sta(0x3EA1);
	ldx(laser_beam_index_3E9F);
	inx;
	lda_X(0x3EA9);
	cmp_imm(0x01);
	bne(B_3BED);
  
	lda_imm(0x03);
	sta(laser_beam_index_3E9F);
B_3BED:
	lda_imm(0x40);
	ldx(0x3EA0);
	jsr(P_erase_beam_3C10);
	
	
	inc(laser_beam_index_3E9F);
	lda(laser_beam_index_3E9F);
	cmp_imm(0x05);
	bne(B_3C04);
  
	sound_play("deadly_beam");
	lda_imm(0x00);
	sta(laser_beam_index_3E9F);
B_3C04:
	ldx(laser_beam_index_3E9F);
	stx(0x3EA0);
	lda_imm(0x7F);
	jsr(P_erase_beam_3C10);
	rts;
  

}
end_subroutine

subroutine(P_erase_beam_3C10)
{

	cpx_imm(0x04);
	beq(B_3C3D);
  
	sta(0x03);
	lda_imm(0xB2);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	txa;
	beq(B_3C31);
  
B_3C21:
	lda(0x01);
	clc;
	adc_imm(0x06);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	dex;
	bne(B_3C21);
  
B_3C31:
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  
B_3C3D:
	ldy_imm(0x00);
B_3C3F:
	dex;
	nop;
	bne(B_3C3F);
  
	rts;
  

}
end_subroutine

subroutine(P_3C44)
{

	inc(0x3E9D);
	lda(0x3E9D);
	cmp_imm(0x08);
	bne(B_3C53);
  
	lda_imm(0x01);
	sta(0x3E9D);
B_3C53:
	sta(0x02);
	ldx_imm(0x2C);
	ldy_imm(0x00);
	lda_imm(0xAD);
	sta(0x00);
	lda_imm(0xAC);
	sta(0x01);
B_3C61:
	dec(0x02);
	bne(B_3C69);
  
	lda_imm(0x07);
	sta(0x02);
B_3C69:
	lda(0x02);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_3C61);
  
	rts;
  

}
end_subroutine

subroutine(P_3C7E)
{

	jsr(P_3D0A);
	lda_imm(0xA3);
	sta(0x01);
	lda_imm(0xB8);
	sta(0x02);
	lda_imm(0x82);
	sta(0x03);
	lda_imm(0xBD);
	sta(0x04);
	lda_imm(0x07);
	sta(0x05);
	inc(0x3E99);
	lda(0x3E99);
	cmp_imm(0x07);
	bcc(B_3CC0);
  
	lda_imm(0x10);
	lda_imm(0x00);
B_3CA3:
	sta_X(0x9E00);
	dex;
	bpl(B_3CA3);
  
	lda_imm(0xFF);
	sta(0x3E99);
	jsr(P_5578);
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x20);
	tay;
	jsr(P_sound_routine_5456);
	jsr(P_3E69);
	rts;
  
B_3CC0:
	jsr(P_5578);
	rts;
  

}
end_subroutine

subroutine(P_3CC4)
{

	jsr(P_3D0A);
	lda_imm(0xDD);
	sta(0x01);
	lda_imm(0xA4);
	sta(0x02);
	lda_imm(0xDD);
	sta(0x03);
	lda_imm(0xA9);
	sta(0x04);
	lda_imm(0x07);
	sta(0x05);
	inc(0x3E9A);
	lda(0x3E9A);
	cmp_imm(0x07);
	bcc(B_3D06);
  
	lda_imm(0x10);
	lda_imm(0x00);
B_3CE9:
	sta_X(0x9E00);
	dex;
	bpl(B_3CE9);
  
	lda_imm(0xFF);
	sta(0x3E9A);
	jsr(P_5578);
	ldx_imm(0x07);
	lda(0x6101);
	ora_imm(0x20);
	tay;
	jsr(P_sound_routine_5456);
	jsr(P_3E69);
	rts;
  
B_3D06:
	jsr(P_5578);
	rts;
  

}
end_subroutine

subroutine(P_3D0A)
{

	lda(VIA_T2CL);
	and_imm(0x1F);
	sta(0x04);
	lda_imm(0x06);
	sta(0x03);
	jsr(P_sound_routine_5482);
	rts;
  

}
end_subroutine

subroutine(P_3D19)
{

	lda_imm(0xAB);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xFB);
	sta(0x01);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xD3);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  

}
end_subroutine

subroutine(P_3D5F)
{

	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xB1);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x07);
	sta(0xBF3A);
	lda_imm(0xFF);
	sta(0xBF3B);
	sta(0xBF3C);
	sta(0xBF3D);
	sta(0xBF3E);
	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x23);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xD8);
	sta(0x01);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x23);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xD8);
	sta(0x01);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x16);
	sta(0x03);
	lda_imm(0x28);
	sta(0x05);
	lda_imm(0xF0);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xB0);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x23);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xB0);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xCB);
	sta(0x01);
	lda_imm(0xAA);
	sta(0x02);
	lda_imm(0x48);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x59);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x20);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x08);
	sta(0xAC5C);
	sta(0xAC84);
	sta(0xACAC);
	sta(0xACD4);
	rts;
  

}
end_subroutine

subroutine(P_3E69)
{

	lda(0x3EAE);
	bne(B_3E98);
  
	lda_imm(0xA4);
	sta(0x01);
	lda_imm(0xA8);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x18);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x4A);
	sta(0x01);
	lda_imm(0xBC);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x18);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_3E98:
	rts;
  
 

}
end_subroutine



subroutine(E_3EB0)
{

	lda_imm(0x01);
	sta(0x3336);
	sta(0x04);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xF7);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x00);
	sta(0x3337);
	rts;
  
 

}
end_subroutine

subroutine(P_bird_level_3F00)
{

	jsr(P_11BF);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0xE8);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x02);
	sta(0x03);
	jsr(P_sound_routine_5482);
	lda_imm(0xEA);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x03);
	sta(0x03);
	jsr(P_sound_routine_5482);
	lda_imm(0x00);
	sta(0x618C);
	lda_imm(0x12);
	sta(0x618B);
	jsr(P_55D3);
	lda_imm(0x07);
	sta(0x617E);
	lda_imm(0x00);
	sta(0x617F);
	sta(0x6180);
	jsr(P_56B9);
	lda_imm(0x01);
	sta(0x617D);
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x15);
	sta(0x13E5);
	lda_imm(0xFF);
	sta(0x13E6);

	move16(0xA6DB,0x617B); // position of the bridge part

	lda_imm(0x00);
	ldx_imm(0x00);
B_3F67:
	sta_X(0x9E00);
	dex;
	bne(B_3F67);
  
	sta(0x041F);
	sta(0x041E);
	sta(0x13EF);
	sta(0x4562);
	sta(0x4563);
	sta(0x4565);
	sta(0x4564);
	jsr(P_439C);
	jsr(P_set_hero_initial_pos_09B2);
	jsr(P_4340);

	// display bridge part at the beginning (does not work first time!!)

	poke(0x0401,189); // fixes bridge display first time (0x401???? used in 5336)
	
	jsr(P_display_bridge_part_5501);
	lda_imm(0xD7);
	sta(0x13E3);

	OricTimer tmr = timer_init(8,20);
	set_game_context(GAME_BIRD);

	int noise_count = 0;
	int just_died = 1;
	
	while(1)
	{
    
	    ldx(0x94A6);

	    update_controls();
	    timer_tick(&tmr);
  
	    lda(0x041E);
	    bne(B_3FBA);
  
	    lda(0x4563);
	    bne(B_3FBA);
  
	    dec(0x9F03);
	    bne(B_3FBA);
  
	    lda(0x13D6);
	    sta(0x9F03);

	    P_handle_hero();

	    jsr(P_bird_level_specifics_4099);
	    
	B_3FBA:
	    
	    lda(0x041E);
	    beq(B_3FD0);
  
	    dec(0x9F06);
	    bne(B_3FD0);

	    // falling/dying (bird level)
	    
	    lda(0x13D6);
	    sta(0x9F06);
	    if (just_died)
	    {
		just_died = 0;
		play_kill_sound();
	    }

	    jsr(P_deadly_fall_12A7);
	    jsr(P_fall_sound_139B);
	    
	B_3FD0:
	    lda(0x617D);
	    beq(B_3FDD);
  
	    dec(0x9F07);
	    bne(B_3FDD);
  
	    jsr(P_5686);
	B_3FDD:
	    lda(0x4564);
	    cmp_imm(0x02);
	    beq(B_4004);
  
	    dec(0x9F00);
	    bne(B_4004);
  
	    lda_imm(0x28);
	    sta(0x9F00);
	    
	    P_animate_bird_429A(&noise_count);
	    lda(0x041E);
	    bne(B_4004);
  
	    lda(0x4563);
	    beq(B_4004);

	    // animate/handle player while clinging to bird
	    jsr(P_41EB);
	    jsr(P_40A6);
	    jsr(P_412B);
	    
	B_4004:
	    if (peek(0x4565))
	    {
		dec(0x9F01);
		if (r.z)
		{
		    LOOP_PLAY(laser_bridge);
		    
		    lda_imm(0x14);
		    sta(0x9F01);
		    jsr(P_anim_color_bridge_422F);
		}
	    }
	    else
	    {
		LOOP_STOP(laser_bridge);
	    }

	    // bird level bridge part display is buggy
	    if (peek(materialize_bridge_part_flag_617A))
	    {
  
		dec(0x9F02);
		if (r.z)
		{
		    lda_imm(0x1E);
		    sta(0x9F02);
		    jsr(P_materialize_bridge_part_5521);
		}
	    }
	    lda(0x4563);
	    bne(B_404D);
  
	    lda(0x4562);
	    beq(B_4037);
  
	    lda(0x4564);
	    beq(B_404D);
  
	    B_4037:
	    lda(0x4562);
	    cmp_imm(0x02);
	    bne(B_4045);
  
	    lda(0x9619);
	    cmp_imm(0x09);
	    beq(B_404D);
  
	    B_4045:
	    dec(0x9F05);
	    bne(B_404D);
  
#ifndef VERY_EASY_GAME
	    jsr(P_create_falling_stones_4468);
#endif
	B_404D:
	    dec(0x9F04);
	    bne(B_405B);
  
	    lda(0x94B2);
	    sta(0x9F04);
	    jsr(P_raining_stones_4499);
	B_405B:

	    if (handle_escape()) break;
  
	B_406F:
	    lda(0x041E);
	    cmp_imm(0x02);
	    beq(B_408A);
  
	    lda(0x041F);
	    cmp_imm(0xFF);
	    bne(B_408A);
  
	    jsr(P_1360); // hero death? sound?
	    
	    lda_imm(0x02);
	    sta(0x041E);
	    lda_imm(0x01);
	    sta(0x041F);
	    
	    B_408A:
	    lda(0x041F);
	    if (r.a == 0x40 || r.a == 0x80) break;
	}
  
	LOOP_STOP(laser_bridge);
  

}
end_subroutine

subroutine(P_bird_level_specifics_4099)
{
	jsr(P_catch_bird_test_4178);
	jsr(P_40E0);
	jsr(P_416A);
	jsr(P_1203);
B_40A5:
	rts;
  

}
end_subroutine

subroutine(P_40A6)
{

	lda(0x4565);
	bne(B_40A5);
  
	lda(0xAF56);
	cmp_imm(0x41);
	bcc(B_40A5);
  
	lda_imm(0x01);
	sta(0x4564);
	sta(0x4562);
	sta(0x4565);
	lda_imm(0x9F);
	sta(0x4566);
	sta(0x01);
	lda_imm(0xB2);
	sta(0x4567);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x1A);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	ldx_imm(0x07);
	ldy_imm(0xF9);
	jsr(P_sound_routine_5456);
B_40A5:
	rts;

  

}
end_subroutine

subroutine(P_40E0)
{

	lda(0x4564);
	beq(B_40A5);
  
	lda(0xAF72);
	cmp_imm(0x41);
	bcc(B_40A5);
  
	lda_imm(0x00);
	sta(0x4564);
	sta(0x4565);
	lda_imm(0x9F);
	sta(0x01);
	lda_imm(0xB2);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x1A);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x9F);
	sta(0x01);
	lda_imm(0xB2);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	jsr(P_4340);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
B_40A5:
	rts;
  

}
end_subroutine

subroutine(P_412B)
{

	lda(0x4565);
	bne(B_4169);
  
	lda(0xA466);
	cmp_imm(0x41);
	bcc(B_4169);
  
	lda_imm(0x01);
	sta(0x4564);
	sta(0x4565);
	lda_imm(0x02);
	sta(0x4562);
	lda_imm(0xAF);
	sta(0x4566);
	sta(0x01);
	lda_imm(0xA7);
	sta(0x4567);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x1A);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	asl(0x94B2);
	ldx_imm(0x07);
	ldy_imm(0xF9);
	jsr(P_sound_routine_5456);
B_4169:
	rts;
  

}
end_subroutine

subroutine(P_416A)
{

	lda(0xA4F9);
	cmp_imm(0x41);
	bcs(B_4172);
  
	rts;
  
B_4172:
	lda_imm(0x40);
	sta(0x041F);
	rts;
  

}
end_subroutine

subroutine(P_catch_bird_test_4178)
{

	lda(0x455C);
	clc;
	adc_imm(0x80);
	sta(0x00);
	lda(0x455D);
	adc_imm(0x02);
	sta(0x01);
	ldy_imm(0x03);
B_4189:
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_4195);
  
	iny;
	cpy_imm(0x06);
	bne(B_4189);
  
	rts;
  
B_4195:
	lda(0x00);
	clc;
	adc_imm(0xE0);
	sta(0x00);
	lda(0x01);
	adc_imm(0x01);
	sta(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x41);
	bcs(B_41A9);
  
	rts;
  
B_41A9:
	lda(0x4569);
	bne(B_41EA);
  
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0x00);
	sta(0x13DF);
	lda_imm(0xFF);
	sta(0x13D8);
	lda_imm(0xC9);
	sta(0x13E1);
	lda_imm(0x89);
	sta(0x13E2);
	lda_imm(0x01);
	sta(0x4563);
	sta(0x4569);
B_41EA:
	rts;
  

}
end_subroutine

subroutine(P_41EB)
{

	lda_imm(0x04);
	ldx_imm(0xFE);
	jsr(P_read_keyboard_matrix_5FDE);
	beq(B_4207);
  
	lda(0x13DE);
	bne(B_420C);
  
	lda_imm(0x00);
	sta(0x13DE);
	lda_imm(0x00);

	// release bird: fall down
	
	falling_sound_id = sound_play("falling");

	sta(jump_counter_13E0);
	sta(0x4563);
	rts;
  
B_4207:
	lda_imm(0x00);
	sta(0x13DE);
B_420C:
	lda_imm(0x05);
	sec;
	sbc(0x455B);
	sta(0x13D3);
	sta(0x00);
	lda(0x455C);
	clc;
	adc_imm(0x0A);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(0x455D);
	adc_imm(0x02);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	jmpl(P_0D8A);
  

}
end_subroutine

subroutine(P_anim_color_bridge_422F)
{

    /*ldx_imm(0x07);
      lda(0x6101);
      and_imm(0xF9);
      tay;
      jsr(P_sound_routine_5456);*/

    // flashing a lot of colors
    
	dec(0x4568);
	lda(0x4568);
	cmp_imm(0x11);
	bcs(B_4249);
  
	lda_imm(0x17);
	sta(0x4568);
B_4249:
	lda(0x4566);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x4567);
	adc_imm(0x00);
	sta(0x02);
	lda(0x4568);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  

}
end_subroutine

subroutine(P_4269)
{

	lda(0x455A);
	cmp_imm(0xAF);
	bcs(B_4281);
  
	cmp_imm(0x96);
	bcs(B_428C);
  
	cmp_imm(0x5A);
	bcc(B_4281);
  
	lda_imm(0x01);
	sta(0x455F);
	sta(0x4560);
	rts;
  
B_4281:
	lda_imm(0x00);
	sta(0x4560);
	lda_imm(0x01);
	sta(0x455F);
	rts;
  
B_428C:
	lda_imm(0x04);
	sta(0x455F);
	lda_imm(0xFF);
	sta(0x4560);
	rts;
  
B_4297:
	jmps(J_4377);
  

}
end_subroutine

void P_animate_bird_429A(int *noise_count)
{
    (*noise_count)++;
    if (*noise_count == 20)
    {
	if (cpu_random_8_bit() == 0)
	{
	    sound_play("bird");
	}
	*noise_count = 0;
    }
	    
	dec(0x455A);
	beq(B_4317);
  
	jsr(P_4269);
	dec(0x455B);
	lda(0x455B);
	sta(0x00);
	bpl(B_430A);
  
	lda_imm(0x05);
	sta(0x455B);
	sta(0x00);
	lda(0x455C);
	sec;
	sbc_imm(0x01);
	sta(0x455C);
	sta(0x01);
	lda(0x455D);
	sbc_imm(0x00);
	sta(0x455D);
	sta(0x02);
J_42C8:
	dec(0x455E);
	bne(B_4297);
  
	lda(0x455F);
	sta(0x455E);
	lda(0x4560);
	beq(B_4297);
  
	bpl(B_42F2);
  
	lda(0x455C);
	clc;
	adc_imm(0x28);
	sta(0x455C);
	sta(0x01);
	lda(0x455D);
	adc_imm(0x00);
	sta(0x455D);
	sta(0x02);
	jmps(J_4377);
  
B_42F2:
	lda(0x455C);
	sec;
	sbc_imm(0x28);
	sta(0x455C);
	sta(0x01);
	lda(0x455D);
	sbc_imm(0x00);
	sta(0x455D);
	sta(0x02);
	jmps(J_4377);
  
B_430A:
	lda(0x455C);
	sta(0x01);
	lda(0x455D);
	sta(0x02);
	jmp(J_42C8);
  
B_4317:
	lda(0x4563);
	beq(B_4324);
	
  	sound_play("jump_down");

	lda_imm(0x00);
	sta(0x4563);
	sta(jump_counter_13E0);
B_4324:
	lda(0x455C);
	sta(0x01);
	lda(0x455D);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	jmpl(P_4340);
B_4297:
	jmps(J_4377);
  

}


subroutine(P_4340)
{

	lda(0x4564);
	beq(B_434B);
  
	lda_imm(0x02);
	sta(0x4564);
	rts;
  
B_434B:
	lda_imm(0x00);
	sta(0x4569);
	lda_imm(0x01);
	sta(0x455E);
	sta(0x455F);
	lda_imm(0xD2);
	sta(0x455A);
	lda_imm(0x00);
	sta(0x00);
	sta(0x455B);
	lda(0x4562);
	bne(B_438B);
  
	lda_imm(0xD3);
	sta(0x455C);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x455D);
	sta(0x02);
	jmps(J_4377);

B_438B:
	lda_imm(0x0B);
	sta(0x455C);
	sta(0x01);
	lda_imm(0xA9);
	sta(0x455D);
	sta(0x02);
	jmps(J_4377);	
}
end_subroutine

subroutine(J_4377)
{
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x70);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

  

}
end_subroutine

subroutine(P_439C)
{

	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0B);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x28);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x98);
	sta(0x07);
	lda_imm(0xB2);
	sta(0x08);
	lda_imm(0x03);
	sta(0x09);
J_43D4:
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x07);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	clc;
	adc_imm(0x07);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	clc;
	adc_imm(0x21);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	clc;
	adc_imm(0x21);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0x15);
	sta(0x03);
	lda_imm(0x07);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	sec;
	sbc_imm(0xF0);
	sta(0x07);
	lda(0x08);
	sbc_imm(0x0A);
	sta(0x08);
	dec(0x09);
	bne(B_4464);
  
	rts;
  
B_4464:
	jmp(J_43D4);
  
 

}
end_subroutine

subroutine(P_create_falling_stones_4468)
{

	ldx_imm(0x00);
B_446A:
	lda_X(0x9E00);
	beq(B_4477);
  
	inx;
	inx;
	inx;
	cpx_imm(0x18);
	bne(B_446A);
  
	rts;
  
B_4477:
	lda_imm(0x01);
	sta_X(0x9E00);
	inx;
B_447D:
	lda(VIA_T2CL);
	adc(0x6189);
	sta(0x6189);
	and_imm(0x1F);
	clc;
	adc_imm(0x08);
	cmp_imm(0x20);
	bcs(B_447D);
  
	sta_X(0x9E00);
	inx;
	lda_imm(0xA0);
	sta_X(0x9E00);
	rts;
  

}
end_subroutine

subroutine(P_raining_stones_4499)
{
	ldx_imm(0x00);
J_449B:
	lda_X(0x9E00);
	bne(B_44A8);
  
	inx;
	inx;
	inx;
	cpx_imm(0x18);
	bne(J_449B);
  
	rts;
  
B_44A8:
	inx;
	lda_X(0x9E00);
	clc;
	adc_imm(0xF0);
	sta(0x00);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x00);
	cmp_imm(0x40);
	beq(B_44C5);
  
	cmp_imm(0x11);
	bcs(B_44E9);
  
B_44C5:
	dex;
	lda_X(0x9E00);
	clc;
	adc_imm(0x28);
	sta_X(0x9E00);
	sta(0x01);
	inx;
	lda_X(0x9E00);
	adc_imm(0x00);
	sta_X(0x9E00);
	sta(0x02);
	txa;
	pha;
	jsr(P_13B8);
	pla;
	tax;
	inx;
	cpx_imm(0x18);
	bne(J_449B);
  
	rts;
  
B_44E9:
	dex;
	dex;
	lda_imm(0x00);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	sta(0x01);
	sta(0x07);
	inx;
	lda_X(0x9E00);
	sta(0x02);
	sta(0x08);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	txa;
	pha;
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	clc;
	adc_imm(0xF0);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	cmp(hero_pos_13D4_plus_1);
	bcc(J_4547);
  
	bne(B_452C);
  
	lda(0x07);
	cmp(hero_pos_13D4);
	bcc(J_4547);
  
B_452C:
	lda(0x07);
	sec;
	sbc_imm(0x70);
	sta(0x07);
	lda(0x08);
	sbc_imm(0x03);
	sta(0x08);
	cmp(hero_pos_13D4_plus_1);
	bcc(B_454F);
  
	bne(J_4547);
  
	lda(0x07);
	cmp(hero_pos_13D4);
	bcc(B_454F);
  
J_4547:
	pla;
	tax;
	inx;
	cpx_imm(0x18);
	bne(B_4557);
  
	rts;
  
B_454F:
move_ff_41f();
	jmp(J_4547);
  
B_4557:
	jmp(J_449B);
  
 

}
end_subroutine

subroutine(E_4570)
{

	inx;
	lda_X(0x9E00);
	sta(0x01);
	txa;
	pha;
	jmps(J_3A3A);
  
 

}
end_subroutine

subroutine(P_zorgon_king_level_4600)
{

	jsr(P_11BF);
	lda_imm(0x06);
	sta(0x03);
	lda_imm(0x1F);
	sta(0x04);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	lda_imm(0x04);
	sta(0x618C);
	lda_imm(0x13);
	sta(0x618B);
	jsr(P_55D3);
	lda_imm(0x09);
	sta(0x617E);
	lda_imm(0x00);
	sta(0x617F);
	sta(0x6180);
	lda_imm(0x01);
	sta(0x617D);
	lda_imm(0x88);
	sta(0x0400);
	lda_imm(0xBD);
	sta(0x0401);
	lda_imm(0xD7);
	sta(0x13E3);
	lda_imm(0xFF);
	sta(0x13E6);
	lda_imm(0x13);
	sta(0x13E5);
	lda_imm(0x00);
	sta(0x041E);
	sta(0x13EF);
	sta(0x041F);
	sta(0x50FE);
	sta(0x50FF);
	sta(0x5100);
	lda_imm(0x03);
	sta(0x5101);
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x08);
	sta(0xB5E5);
	sta(0xB60D);
	sta(0xB635);
	sta(0xB65D);
	jsr(P_4909);
	jsr(P_set_hero_initial_pos_09B2);
	jsr(P_4BF9);
	jsr(P_4D5A);
	lda_imm(0x02);
	sta(0x50FD);
	jsr(P_4DD8);

	OricTimer tmr = timer_init(5,15);

	int just_died = 1;
	int platform_just_crashed = 1;
	int noise_count = 0;
	
	while(1)
	{
	    /*ldx(0x94A7);
	      B_4690:
	      dex;
	      bne(B_4690);*/

	    timer_tick(&tmr);

	    update_controls();
	    
	    dec(0x9F00);
	    bne(B_46A4);
  
	    lda(0x94B3);
	    sta(0x9F00);
#ifndef VERY_EASY_GAME
	    jsr(P_handle_ant_moves_4CF6);
	    jsr(P_handle_ant_stings_4C57);
#endif
	B_46A4:
	    lda(0x5100);
	    bne(B_46B6);
  
	    dec(0x9F01);
	    bne(B_46B6);
  
	    lda_imm(0xC8);
	    sta(0x9F01);
	    jsr(P_animate_zorgon_king_4D83);
	B_46B6:
	    lda(0x5100);
	    bne(B_46E2);

	    // jsr(P_sound_routine_50AD);

	    lda(0x50FE);
	    beq(B_46D0);
  
	    dec(0x9F02);
	    bne(B_46D0);
  
	    lda_imm(0x05);
	    sta(0x9F02);

	    jsr(P_anim_right_diag_shoot_4EAF);

	B_46D0:
	    lda(0x50FF);
	    beq(B_46E2);
  
	    dec(0x9F04);
	    bne(B_46E2);
  
	    lda_imm(0x05);
	    sta(0x9F04);

	    jsr(P_anim_left_diag_shoot_4F4E);

	B_46E2:
	    lda(0x041E);
	    bne(B_46FE);
  
	    dec(0x9F03);
	    bne(B_46FE);
  
	    lda(0x13D6);
	    sta(0x9F03);
	    
	    P_handle_hero();
	    jsr(P_king_button_test_48A0);
	    
	    B_46FE:
	    lda(0x041E);
	    beq(B_4714);
  
	    dec(0x9F07);
	    bne(B_4714);
  
	    lda(0x13D6);
	    sta(0x9F07);
	    if (just_died)
	    {
		just_died = 0;
		play_kill_sound();
	    }
	    jsr(P_deadly_fall_12A7);	    
	    jsr(P_fall_sound_139B);
	    
	B_4714:
	    lda(0x617D);
	    beq(B_4721);
  
	    dec(0x9F08);
	    bne(B_4721);
  
	    jsr(P_5686);
	B_4721:
	    lda(0x5100);
	    beq(B_4735);
  
	    bmi(B_4735);

	    dec(0x9F05);
	    bne(B_4735);
  
	    if (platform_just_crashed)
	    {
		platform_just_crashed = 0;
		sound_play("crash");
	    }
	    
	    lda_imm(0x1E);
	    sta(0x9F05);
	    jsr(P_king_falls_4FF7);

	B_4735:
	    lda(0x5100);
	    bpl(B_4747);
	    
	    dec(0x9F06);
	    bne(B_4747);
	    
	    LOOP_PLAY(falling);

	    lda_imm(0x04);
	    sta(0x9F06);
	    jsr(P_5066);
	B_4747:

	    if (handle_escape()) return;
  
	B_475B:
	    lda(0x5100);
	    bmi(B_477B);
  
	    lda(0x041E);
	    cmp_imm(0x02);
	    beq(B_477B);
  
	    lda(0x041F);
	    cmp_imm(0xFF);
	    bne(B_477B);
  
	    jsr(P_1360);
	    lda_imm(0x02);
	    sta(0x041E);
	    lda_imm(0x01);
	    sta(0x041F);
	B_477B:
	    if (peek(0x041F) == 0x80) return;
	    if (peek(0x041F) == 0x40) break;

	    noise_count++;
	    if (noise_count == 50)
	    {
		if (cpu_random_8_bit() == 0)
		{
		    sound_play("female_scream");
		}
	
		noise_count = 0;
	    } 
	}
	
	LOOP_STOP(run);
	
	oric_wait(200);
	
	LOOP_STOP(falling);
  
	set_game_context(COMPLETED);
	
	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0xC7);
	sta(0x06);

	// clear screen
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x6E);
	sta(0x01);
	lda_imm(0xA9);
	sta(0x02);
	lda_imm(0x95);
	sta(0x03);
	lda_imm(0x75);
	sta(0x04);
	lda_imm(0x0C);
	sta(0x05);
	lda_imm(0xD8);
	sta(0x06);
	// draw upper part of heart
	jsr(P_blit_to_screen_5285);
	lda_imm(0x3E);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x0C);
	sta(0x05);
	lda_imm(0xD8);
	sta(0x06);
	// draw central rectangular red part
	
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x0E);
	sta(0x01);
	lda_imm(0xAF);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x0C);
	sta(0x05);
	lda_imm(0x90);
	sta(0x06);
	// red part again
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x31);
	sta(0x01);
	lda_imm(0xAD);
	sta(0x02);
	lda_imm(0x01);
	sta(0x03);
	lda_imm(0x06);
	sta(0x05);
	lda_imm(0x90);
	sta(0x06);
	// draw black hole in the middle
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xEE);
	sta(0x01);
	lda_imm(0xB0);
	sta(0x02);
	lda_imm(0x6D);
	sta(0x03);
	lda_imm(0x76);
	sta(0x04);
	lda_imm(0x0C);
	sta(0x05);
	lda_imm(0xD8);
	sta(0x06);
	// draw lower part of the heart
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xBE);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x45);
	sta(0x03);
	lda_imm(0x77);
	sta(0x04);
	lda_imm(0x0C);
	sta(0x05);
	lda_imm(0x90);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x59);
	sta(0x01);
	lda_imm(0xAD);
	sta(0x02);
	lda_imm(0x8E);
	sta(0x03);
	lda_imm(0x8F);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x42);
	sta(0x06);
	// draw princess roz
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x5C);
	sta(0x01);
	lda_imm(0xAD);
	sta(0x02);
	lda_imm(0xF1);
	sta(0x03);
	lda_imm(0x8B);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x2E);
	sta(0x06);
	// draw our hero
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_king_button_test_48A0)
{

	lda(0x5100);
	bne(B_48AB);
  
	jsr(P_48AC);
	jsr(P_48E4);
B_48AB:
	rts;
  

}
end_subroutine

subroutine(P_48AC)
{

	ldx_imm(0x00);
	ldy_imm(0x00);
B_48B0:
	lda_X(0x9E00);
	bne(B_48BC);
  
	inx;
	inx;
	cpx_imm(0x06);
	bne(B_48B0);
  
B_48BB:
	rts;
  
B_48BC:
	lda_X(0x5109);
	beq(B_48BB);
  
	inx;
	lda_X(0x9E00);
	sec;
	sbc_imm(0x8D);
	sta(0x01);
	lda_imm(0xBD);
	sbc_imm(0x01);
	sta(0x02);
	ldy_imm(0x00);
	lda_ind_Y(0x01);
	cmp_imm(0x41);
	bcs(B_48DE);
  
	inx;
	cpx_imm(0x06);
	bne(B_48B0);
  
	rts;
  
B_48DE:
	move_ff_41f();
	rts;
  

}
end_subroutine

subroutine(P_48E4)
{

	lda(0x50FD);
	cmp_imm(0x02);
	beq(B_48F3);
  
	lda(0xB795);
	cmp_imm(0x41);
	bcs(B_48FB);
  
	rts;
  
B_48F3:
	lda(0xB772);
	cmp_imm(0x41);
	bcs(B_4902);
  
	rts;
  
B_48FB:
	jsr(P_4E6E);
	jsr(P_4DD8);
	rts;
  
B_4902:
	jsr(P_init_shoot_left_4E2D);
	jsr(P_4DD8);
	rts;
  

}
end_subroutine

subroutine(P_4909)
{

	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x09);
	sta(0x01);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x1F);
	sta(0x01);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x14);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x15);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x12);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x18);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x25);
	sta(0x01);
	lda_imm(0xB2);
	sta(0x02);
	lda_imm(0x99);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x30);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x12);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x33);
	sta(0x01);
	lda_imm(0xB3);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x01);
	sta(0x00);
	lda_imm(0x3F);
	sta(0x01);
	lda_imm(0xB2);
	sta(0x02);
	lda_imm(0x99);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x43);
	sta(0x01);
	lda_imm(0xB2);
	sta(0x02);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x90);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x91);
	sta(0x07);
	lda_imm(0xA6);
	sta(0x08);
	lda_imm(0x26);
	sta(0x09);
B_4A32:
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x68);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0x01);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_4A32);
  
	lda_imm(0x9E);
	sta(0x07);
	lda_imm(0xA6);
	sta(0x08);
	lda_imm(0x0C);
	sta(0x09);
	lda_imm(0x48);
	sta(0x0A);
B_4A72:
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda(0x09);
	sta(0x05);
	lda(0x0A);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	clc;
	adc_imm(0xF1);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	lda(0x0A);
	sec;
	sbc_imm(0x0C);
	sta(0x0A);
	dec(0x09);
	dec(0x09);
	bne(B_4A72);
  
	lda_imm(0x8E);
	sta(0x07);
	lda_imm(0xA7);
	sta(0x08);
	lda_imm(0x01);
	sta(0x09);
B_4AAF:
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda(0x09);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	clc;
	adc_imm(0xEF);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	inc(0x09);
	lda(0x09);
	cmp_imm(0x03);
	bne(B_4ADF);
  
	lda_imm(0x05);
	sta(0x09);
B_4ADF:
	cmp_imm(0x08);
	bne(B_4AAF);
  
	lda_imm(0x13);
	sta(0xBF18);
	lda_imm(0x88);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x13);
	sta(0x03);
	lda_imm(0x28);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x8B);
	sta(0x07);
	lda_imm(0xA7);
	sta(0x08);
	lda_imm(0x0C);
	sta(0x09);
B_4B09:
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0xC1);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0xEF);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_4B09);
  
	lda_imm(0x99);
	sta(0x07);
	lda_imm(0xA7);
	sta(0x08);
	lda_imm(0x0C);
	sta(0x09);
B_4B45:
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0xD3);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda(0x07);
	clc;
	adc_imm(0xF1);
	sta(0x07);
	lda(0x08);
	adc_imm(0x00);
	sta(0x08);
	dec(0x09);
	bne(B_4B45);
  
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x0F);
	sta(0x01);
	lda_imm(0xA3);
	sta(0x02);
	lda_imm(0x8E);
	sta(0x03);
	lda_imm(0x8F);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x42);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x08);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	lda_imm(0xD0);
	sta(0x03);
	lda_imm(0x8F);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x3C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x90);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x56);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x91);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x56);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xB6);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x7F);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x56);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  

}
end_subroutine

subroutine(P_4BF9)
{

	lda_imm(0xB0);
	sta(0x01);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x06);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x08);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	ldx_imm(0x00);
	lda_imm(0xB5);
	sta(0x07);
	lda_imm(0x00);
	sta(0x08);
B_4C1A:
	lda(0x08);
	inc(0x08);
	sta_X(0x9E00);
	sta(0x00);
	inx;
	lda(0x07);
	sta_X(0x9E00);
	sta(0x01);
	txa;
	pha;
	jsr(P_4C3F);
	pla;
	tax;
	lda(0x07);
	clc;
	adc_imm(0x0B);
	sta(0x07);
	inx;
	cpx_imm(0x06);
	bne(B_4C1A);
  
	rts;
  

}
end_subroutine

subroutine(P_4C3F)
{

	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0xE5);
	sta(0x03);
	lda_imm(0x73);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x28);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_handle_ant_stings_4C57)
{

	ldx_imm(0x00);
J_4C59:
	lda_X(0x9E00);
	beq(B_4C69);
  
	cmp_imm(0x02);
	beq(B_4CBA);
  
B_4C62:
	inx;
	inx;
	cpx_imm(0x06);
	bne(J_4C59);
  
	rts;
  
B_4C69:
	dec(0x50FC);
	bne(B_4C62);
  
	lda_imm(0x02);
	sta(0x50FC);
	sta_X(0x5109);
	inx;
	lda_X(0x9E00);
	cmp_imm(0xB2);
	beq(B_4CE6);
  
	sbc_imm(0x65);
	sta(0x01);
	lda_imm(0xBD);
	sbc_imm(0x01);
	sta(0x02);
	ldy_imm(0x00);
	lda_imm(0x08);
	sta(0x06);
B_4C8E:
	lda_ind_Y(0x01);
	cmp_imm(0x41);
	bcs(B_4CAF);
  
	lda_imm(0x44);
	sta_ind_Y(0x01);
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	dec(0x06);
	bne(B_4C8E);
  
	inx;
	cpx_imm(0x06);
	bne(J_4C59);
  
	rts;
  
B_4CAF:
	lda(0x5100);
	bne(B_4CB9);

	move_ff_41f();
B_4CB9:
	rts;
  
B_4CBA:
	lda_X(0x5109);
	beq(B_4CEF);
  
	lda_imm(0x00);
	sta_X(0x5109);
	inx;
	lda_X(0x9E00);
	sec;
	sbc_imm(0x64);
	sta(0x01);
	lda_imm(0xBD);
	sbc_imm(0x01);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x08);
	sta(0x06);
	txa;
	pha;
	jsr(P_draw_rectangle_52CE);
	pla;
	tax;
B_4CE6:
	inx;
	cpx_imm(0x06);
	bne(B_4CEC);
  
	rts;
  
B_4CEC:
	jmp(J_4C59);
  
B_4CEF:
	inx;
	inx;
	cpx_imm(0x06);
	bne(B_4CEC);
  
	rts;
  

}
end_subroutine

subroutine(P_handle_ant_moves_4CF6)
{

	ldx_imm(0x00);
B_4CF8:
	dec_X(0x9E00);
	lda_X(0x9E00);
	sta(0x00);
	bpl(B_4D23);
  
	lda_imm(0x02);
	sta_X(0x9E00);
	sta(0x00);
	inx;
	dec_X(0x9E00);
	lda_X(0x9E00);
	sta(0x01);
	cmp_imm(0xB1);
	beq(B_4D2C);
  
J_4D16:
	txa;
	pha;
	jsr(P_4C3F);
	pla;
	tax;
	inx;
	cpx_imm(0x06);
	bne(B_4CF8);
  
	rts;
  
B_4D23:
	inx;
	lda_X(0x9E00);
	sta(0x01);
	jmp(J_4D16);
  
B_4D2C:
	dex;
	lda_imm(0x02);
	sta_X(0x9E00);
	inx;
	lda_X(0x9E00);
	sta(0x01);
	lda_imm(0xD3);
	sta_X(0x9E00);
	lda_imm(0xBD);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x28);
	sta(0x06);
	txa;
	pha;
	jsr(P_draw_rectangle_52CE);
	pla;
	tax;
	inx;
	cpx_imm(0x06);
	bne(B_4CF8);
  
	rts;
  

}
end_subroutine

subroutine(P_4D5A)
{

	lda_imm(0x00);
	sta(0x50FB);
	sta(0x00);
	jsr(J_4D61);
}
end_subroutine

subroutine(J_4D61)
{
	lda_imm(0x31);
	sta(0x01);
	sta(0x5102);
	lda_imm(0xA3);
	sta(0x02);
	sta(0x5103);
	lda_imm(0x5D);
	sta(0x03);
	lda_imm(0x74);
	sta(0x04);
	lda_imm(0x06);
	sta(0x05);
	lda_imm(0x7E);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_animate_zorgon_king_4D83)
{

	lda(0x50FB);
	beq(B_4D92);
  
	lda_imm(0x00);
	sta(0x50FB);
	sta(0x00);
	jmps(J_4D61);
  
B_4D92:
	lda_imm(0x01);
	sta(0x50FB);
	sta(0x00);
	jmps(J_4D61);
}
end_subroutine

subroutine(J_4D9C)
{
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	lda_imm(0x48);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	clc;
	adc_imm(0xEE);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0x20);
	sta(0x03);
	lda_imm(0x8E);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_4DD8)
{

	lda(0x50FD);
	cmp_imm(0x01);
	beq(B_4E06);
  
	
	lda_imm(0x01);
	sta(0x50FD);
	lda_imm(0xF2);
	sta(0x01);
	lda_imm(0xB4);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x15);
	sta(0x07);
	lda_imm(0xB5);
	sta(0x08);
	jmps(J_4D9C);
  
B_4E06:
	lda_imm(0x02);
	sta(0x50FD);
	lda_imm(0x15);
	sta(0x01);
	lda_imm(0xB5);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0xF2);
	sta(0x07);
	lda_imm(0xB4);
	sta(0x08);
	jmps(J_4D9C);
  

}
end_subroutine

subroutine(P_init_shoot_left_4E2D)
{

	sound_play("cannon");
	
	lda_imm(0x01);
	sta(0x50FF);
	sta(0x13F6);
	lda_imm(0x00);
	sta(0x00);
	sta(0x5106);
	lda_imm(0x0F);
	sta(0x01);
	sta(0x5107);
	lda_imm(0xB1);
	sta(0x02);
	sta(0x5108);
	jsr(J_4E4A);
}
end_subroutine

subroutine(J_4E4A)
{
	lda_imm(0x59);
	sta(0x03);
	lda_imm(0x75);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xDF);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x05);
	sta(0x510F);
	rts;
  

}
end_subroutine

subroutine(P_4E6E)
{
	sound_play("cannon");

	lda_imm(0x01);
	sta(0x50FE);
	sta(0x13F6);
	lda_imm(0x00);
	sta(0x00);
	sta(0x5106);
	lda_imm(0x27);
	sta(0x01);
	sta(0x5107);
	lda_imm(0xB1);
	sta(0x02);
	sta(0x5108);
	jsr(J_4E8B);
}
end_subroutine

subroutine(J_4E8B)
{
	lda_imm(0x77);
	sta(0x03);
	lda_imm(0x75);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x0A);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xDF);
	tay;
	jsr(P_sound_routine_5456);
	lda_imm(0x05);
	sta(0x510F);
B_4EAE:
	rts;
  

}
end_subroutine

subroutine(P_anim_right_diag_shoot_4EAF)
{

	jsr(P_4F14);
	lda(0x50FE);
	beq(B_4EAE);
  
	dec(0x13F6);
	bne(B_4EF7);
  
	lda_imm(0x03);
	sta(0x13F6);

	dec(0x5106);
	lda(0x5106);
	sta(0x00);
	bpl(B_4EEA);
  
	lda_imm(0x02);
	sta(0x00);
	sta(0x5106);
	lda(0x5107);
	sec;
	sbc_imm(0x29);
	sta(0x5107);
	sta(0x01);
	lda(0x5108);
	sbc_imm(0x00);
	sta(0x5108);
	sta(0x02);
	jmps(J_4E8B);
  
B_4EEA:
	lda(0x5107);
	sta(0x01);
	lda(0x5108);
	sta(0x02);
	jmps(J_4E8B);
  
B_4EF7:
	lda(0x5106);
	sta(0x00);
	lda(0x5107);
	sec;
	sbc_imm(0x28);
	sta(0x5107);
	sta(0x01);
	lda(0x5108);
	sbc_imm(0x00);
	sta(0x5108);
	sta(0x02);
	jmps(J_4E8B);
  
B_4EAE:
	rts;

}
end_subroutine

subroutine(P_4F14)
{

	lda(0x5107);
	sec;
	sbc_imm(0x28);
	sta(0x01);
	lda(0x5108);
	sbc_imm(0x00);
	sta(0x02);
	ldy_imm(0x00);
	lda_ind_Y(0x01);
	cmp_imm(0x7F);
	beq(B_4F2C);
  
	rts;
  
B_4F2C:
	// zorgon king right shoot reaches ceiling
	sound_play("hit_ceiling");

	lda(0x01);
	sec;
	sbc_imm(0xC9);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x00);
	sta(0x50FE);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_4F4D:
	rts;
  

}
end_subroutine

subroutine(P_anim_left_diag_shoot_4F4E)
{

	jsr(P_4FB3);
	lda(0x50FF);
	beq(B_4F4D);
  
	dec(0x13F6);
	bne(B_4F96);
  
	lda_imm(0x03);
	sta(0x13F6);
	dec(0x5106);
	lda(0x5106);
	sta(0x00);
	bpl(B_4F89);
  
	lda_imm(0x02);
	sta(0x00);
	sta(0x5106);
	lda(0x5107);
	sec;
	sbc_imm(0x27);
	sta(0x5107);
	sta(0x01);
	lda(0x5108);
	sbc_imm(0x00);
	sta(0x5108);
	sta(0x02);
	jmps(J_4E4A);
  
B_4F4D:
	rts;

B_4F89:
	lda(0x5107);
	sta(0x01);
	lda(0x5108);
	sta(0x02);
	jmps(J_4E4A);
  
B_4F96:
	lda(0x5106);
	sta(0x00);
	lda(0x5107);
	sec;
	sbc_imm(0x28);
	sta(0x5107);
	sta(0x01);
	lda(0x5108);
	sbc_imm(0x00);
	sta(0x5108);
	sta(0x02);
	jmps(J_4E4A);
  

}
end_subroutine

subroutine(P_4FB3)
{

	lda(0x5107);
	sec;
	sbc_imm(0x27);
	sta(0x01);
	lda(0x5108);
	sbc_imm(0x00);
	sta(0x02);
	ldy_imm(0x00);
	lda_ind_Y(0x01);
	cmp_imm(0x7F);
	beq(B_4FCB);
  
	rts;
  
B_4FCB:
	// zorgon king left shoot reaches ceiling
	sound_play("hit_ceiling");
	
	lda(0x01);
	sec;
	sbc_imm(0xC9);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x00);
	sta(0x50FF);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x1E);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	dec(0x5101);
	bne(B_4FF6);
  
	lda_imm(0x01);
	sta(0x5100);
B_4FF6:
	rts;
  

}
end_subroutine

subroutine(P_king_falls_4FF7)
{

	lda_imm(0x78);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x08);
	sta(0x03);
	lda_imm(0xAD);
	sta(0x04);
	lda_imm(0x07);
	sta(0x05);
	inc(0x5100);
	lda(0x5100);
	cmp_imm(0x14);
	bne(B_501A);
  
	lda_imm(0xFF);
	sta(0x5100);
B_501A:
	jsr(P_5578);
	lda(0x5100);
	bpl(B_503D);
  
	lda_imm(0x7F);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x06);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	jsr(P_50E0);
	rts;
  
B_503D:
	ldx_imm(0x07);
	ldy_imm(0xC7);
	jsr(P_sound_routine_5456);
	lda_imm(0x04);
	sta(0x03);
B_5048:
	lda(0x09);
	sta(0x03);
	lda(0x618A);
	eor(VIA_T2CL);
	adc(VIA_T1CL);
	sta(0x618A);
	sta(0x04);
	jsr(P_sound_routine_5482);
	inc(0x09);
	lda(0x09);
	cmp_imm(0x07);
	bne(B_5048);
  
	rts;
  

}
end_subroutine

subroutine(P_5066)
{

	lda_imm(0x00);
	sta(0x00);
	lda(0x5102);
	clc;
	adc_imm(0x28);
	sta(0x5102);
	sta(0x01);
	lda(0x5103);
	adc_imm(0x00);
	sta(0x5103);
	sta(0x02);
	cmp_imm(0xC0);
	bcs(B_50A0);
  
	lda_imm(0x5D);
	sta(0x03);
	lda_imm(0x74);
	sta(0x04);
	lda_imm(0x06);
	sta(0x05);
	lda_imm(0x7E);
	sta(0x06);
	lda_imm(0x00);
	sta(0x07);
	sta(0x08);
P_5305(0);
	jsr(P_50C3);
	rts;
  
B_50A0:
	lda_imm(0x40);
	sta(0x041F);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	rts;
  

}
end_subroutine

subroutine(P_sound_routine_50AD)
{

	lda(0x510F);
	beq(B_50B8);
  
	bmi(B_50C2);
  
	dec(0x510F);
	rts;
  
B_50B8:
	dec(0x510F);
	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
B_50C2:
	rts;
  

}
end_subroutine

subroutine(P_50C3)
{

	lda_imm(0x01);
	sta(0x03);
	lda(0x5104);
	clc;
	adc_imm(0x04);
	sta(0x5104);
	sta(0x04);
	lda(0x5105);
	adc_imm(0x00);
	sta(0x5105);
	sta(0x05);
	jsr(P_sound_routine_5482);
	rts;
  

}
end_subroutine

subroutine(P_50E0)
{

	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x00);
	sta(0x5104);
	sta(0x04);
	sta(0x5105);
	sta(0x05);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	ldy_imm(0xFE);
	jsr(P_sound_routine_5456);
	rts;
  
 

}
end_subroutine

/*subroutine(0x5108)
{

	lda_imm(0x00);
	jsr(P_2000);
	0x02;

}
end_subroutine
*/
/*
subroutine(E_510E)
{

	jsr(P_16FF);
	sta(0x05);
	lda_imm(0x03);
	sta(0x04);
	jsr(P_sound_routine_5482);
	ldx_imm(0x07);
	lda(0x6101);
	and_imm(0xFD);
	tay;
	jsr(P_sound_routine_5456);
	rts;
  
 

}
end_subroutine
*/
subroutine(E_513B)
{

	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x5C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x13D8);
	bmi(B_5171);
  
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x02);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0xA1);
	sta(0x13ED);
	lda_imm(0x90);
	sta(0x13EE);
	jmpl(P_1285);
  
B_5171:
	lda(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	sta(0x02);
	lda_imm(0xF1);
	sta(0x13ED);
	lda_imm(0x8B);
	sta(0x13EE);
	lda_imm(0x00);
	sta(0x00);
	lda(0x13ED);
	sta(0x03);
	lda(0x13EE);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x2E);
	sta(0x06);
	lda_imm(0x17);
	sta(0x07);
	lda_imm(0x00);
	sta(0x08);
P_5305(0);
	rts;
  

}
end_subroutine

/*subroutine(E_51A7)
{

	lda(0x13EF);
	bne(B_51E0);
  
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x98);
	sta(0x00);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x03);
	sta(0x01);
	ldy_imm(0x01);
	lda_ind_Y(0x00);
	cmp_imm(0x11);
	bcc(B_51E0);
  
	cmp_imm(0x40);
	beq(B_51E0);
  
	lda(0x00);
	sta(0x0400);
	lda(0x01);
	sta(0x0401);
	lda_imm(0x01);
	sta(0x13EF);
	lda_imm(0x13);
	sta(0x13F0);
	lda_imm(0x1E);
	sta(0x13D6);
B_51E0:
	lda(hero_pos_13D4);
	clc;
	adc_imm(0x28);
	sta(hero_pos_13D4);
	sta(0x01);
	lda(hero_pos_13D4_plus_1);
	adc_imm(0x00);
	sta(hero_pos_13D4_plus_1);
	sta(0x02);
	lda(0x041E);
	cmp_imm(0x01);
	beq(B_5202);
  
	jsr(P_1316);
	// jsr(P_5200);
}
end_subroutine*/

subroutine(P_5200)
{
    ldx_imm(0x03);
B_5202:
	lda_imm(0x01);
B_5204:
	sta_X(0x9E10);
	dex;
	bne(B_5204);
  
	lda(0x06);
	asl_A();
	sta(0x08);
	lda(0x04);
	sta(0x9E00);
	lda(0x05);
	sta(0x9E01);
	lda(0x9E00);
	clc;
	adc(0x08);
	sta(0x9E02);
	lda(0x9E01);
	adc_imm(0x00);
	sta(0x9E03);
	lda(0x9E02);
	clc;
	adc(0x08);
	sta(0x9E04);
	lda(0x9E03);
	adc_imm(0x00);
	sta(0x9E05);
	rts;
}
end_subroutine

subroutine(P_527C)
{
	ldx(0x02);
	dex;
	beq(B_GEN_1);
	jmps(B_523E);
B_GEN_1:
	jmps(P_523C);
  
}
end_subroutine

subroutine(P_523C)
{
	ldx_imm(0x03);
	B_523E();
}
end_subroutine

subroutine(B_523E)
{
	dec_X(0x9E10);
	beq(B_5249);
  
	dex;
	bne(B_523E);
  
	ldx_imm(0x03);
	jmp(B_523E);
  
B_5249:
	txa;
	asl_A();
	sec;
	sbc_imm(0x02);
	tay;
	lda_Y(0x9E00);
	sta(0x08);
	iny;
	lda_Y(0x9E00);
	sta(0x09);
	dey;
	lda_Y(0x9E00);
	clc;
	adc_imm(0x02);
	sta_Y(0x9E00);
	iny;
	lda_Y(0x9E00);
	adc_imm(0x00);
	sta_Y(0x9E00);
	ldy_imm(0x00);
	lda_ind_Y(0x08);
	sta(0x03);
	iny;
	lda_ind_Y(0x08);
	sta_X(0x9E10);
	stx(0x02);
	rts;
}
end_subroutine



/* copy to screen (same routine as Xenon1)
 *
 * params:
 *
 * 0.b : frame number (if animated sprite)
 * 1.w : destination
 * 3.w : source
 * 5   : number of rows
 * 6.b : number of bytes
 */
void P_blit_to_screen_5285()
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
    {
	
	for (i = 0; i < nb_lines; i++)
	{
	    int nbr = 0xC000 - dest_address > nb_rows ? nb_rows : 0xC000 - dest_address;
	    cpu_memcopy(source_address,dest_address,nbr);
	    
	    dest_address += 40;
	    source_address += nb_rows;
	    if (dest_address >= 0xC000) break;
	}
    

	doke(1,dest_address);
    }
  
}
/*
subroutine()
{
	lda(0x02);
	cmp_imm(0xA0);
	bcc(B_52CD);
  
	cmp_imm(0xC0);
	bcs(B_52CD);
  
	ldx(0x00);
	beq(B_52A3);
  
B_5293:
	lda(0x03);
	clc;
	adc(0x06);
	sta(0x03);
	lda(0x04);
	adc_imm(0x00);
	sta(0x04);
	dex;
	bne(B_5293);
  
B_52A3:
	ldy_imm(0x00);
B_52A5:
	ldx(0x05);
B_52A7:
	lda_ind_Y(0x03);
	sta_ind_Y(0x01);
	iny;
	dex;
	bne(B_52A7);
  
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
	bne(B_52A5);
  
B_52CD:
	rts;
  

}
end_subroutine*/

subroutine(P_draw_rectangle_52CE)
{
	lda(0x02);
	cmp_imm(0xA0);
	bcc(B_52CD);
  
	cmp_imm(0xC0);
	bcs(B_52CD);
  
	ldy_imm(0x00);
B_52DA:
	ldx_imm(0x00);
B_52DC:
	lda(0x03);
	sta_ind_Y(0x01);
	iny;
	inx;
	cpx(0x05);
	bne(B_52DC);
  
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
	bne(B_52DA);
  
B_52CD:
	rts;

  

}
end_subroutine

void P_5305(int to_5356)
{
    if (!to_5356)
    {
	lda(0x02);
	cmp_imm(0xA0);
	bcc(B_5355);
  
	cmp_imm(0xC0);
	bcs(B_5355);
  
	jsr(P_5385);
	ldx(0x00);
	beq(B_5326);
  
B_5316:
	lda(0x03);
	clc;
	adc(0x06);
	sta(0x03);
	lda(0x04);
	adc_imm(0x00);
	sta(0x04);
	dex;
	bne(B_5316);
  
B_5326:
	ldy_imm(0x00);
B_5328:
	ldx(0x05);
	P_5305(1);// jsr(P_5356);
	bne(B_5371);
  
B_532F:
	lda_ind_Y(0x03);
	sta_ind_Y(0x01);  // write to screen
	iny;
	dex;
	bne(B_532F);
  
J_5337:
	lda(0x01);
	clc;
	adc(0x09);
	sta(0x01);
	lda(0x02);
	adc(0x0A);
	sta(0x02);
	lda(0x01);
	sec;
	sbc(0x05);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	cpy(0x06);
	bne(B_5328);
  
B_5355:
	rts;
  

    }

// 5356
	tya;
	clc;
	adc(0x01);
	sta(0x0B);
	lda(0x02);
	adc_imm(0x00);

	cmp_imm(0xA0);
	bcc(B_536E);
  
	cmp(0x0401);
	beq(B_537B);  // why 401 is used?
  
	bcs(B_536E);
  
	lda_imm(0x00);
	rts;
  
B_536E:
	lda_imm(0x01);
	rts;
  
B_5371:
	txa;
	sty(0x00);
	clc;
	adc(0x00);
	tay;
	jmp(J_5337);
  
B_537B:
	lda(0x0B);
	cmp(0x0400);
	bcs(B_536E);
  
	lda_imm(0x00);
	rts;
  

}


subroutine(P_5385)
{

	lda(0x08);
	pha;
	jsr(P_53BF);
	lda_imm(0x28);
	sta(0x07);
	lda(0x09);
	sta(0x08);
	jsr(P_53BF);
	lda(0x01);
	sec;
	sbc(0x09);
	sta(0x01);
	lda(0x02);
	sbc(0x0A);
	sta(0x02);
	pla;
	pha;
	sta(0x08);
	lda_imm(0x28);
	sta(0x07);
	jsr(P_53BF);
	pla;
	sta(0x08);
	lda(0x09);
	clc;
	adc_imm(0x28);
	sta(0x09);
	lda(0x0A);
	adc_imm(0x00);
	sta(0x0A);
	rts;
  

}
end_subroutine

subroutine(P_53BF)
{

	lda_imm(0x00);
	sta(0x09);
	ldx_imm(0x08);
B_53C5:
	lsr(0x07);
	bcc(B_53CC);
  
	clc;
	adc(0x08);
B_53CC:
	ror_A();
	ror(0x09);
	dex;
	bne(B_53C5);
  
	sta(0x0A);
	rts;
  

}
end_subroutine

subroutine(P_screen_routine_53D5)
{    
	lda(0x02);
	cmp_imm(0xA0);
	bcc(B_5411);
  
	cmp_imm(0xC0);
	bcs(B_5411);
  
	jsr(P_5385);
	ldy_imm(0x00);
B_53E4:
	ldx(0x05);
	P_5305(1);// jsr(P_5356);
	bne(B_5412);
  
B_53EB:
	lda(0x03);
	sta_ind_Y(0x01);
	iny;
	dex;
	bne(B_53EB);
  
J_53F3:
	lda(0x01);
	clc;
	adc(0x09);
	sta(0x01);
	lda(0x02);
	adc(0x0A);
	sta(0x02);
	lda(0x01);
	sec;
	sbc(0x05);
	sta(0x01);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x02);
	cpy(0x06);
	bne(B_53E4);
  
B_5411:
	rts;
  
B_5412:
	txa;
	sty(0x00);
	clc;
	adc(0x00);
	tay;
	jmp(J_53F3);
  

}
end_subroutine

subroutine(P_541C)
{

	lda_imm(0x00);
	sta(0x02);
	ldx_imm(0x08);
B_5422:
	lsr(0x00);
	bcc(B_5429);
  
	clc;
	adc(0x01);
B_5429:
	ror_A();
	ror(0x02);
	dex;
	bne(B_5422);
  
	sta(0x03);
	rts;
  

}
end_subroutine

subroutine(P_5432)
{

	ldy_imm(0x08);
	sec;
	lda(0x01);
	sbc(0x02);
B_5439:
	php;
	rol(0x04);
	asl(0x00);
	rol_A();
	plp;
	bcc(B_5447);
  
	sbc(0x02);
	jmp(J_5449);
  
B_5447:
	adc(0x02);
J_5449:
	dey;
	bne(B_5439);
  
	bcs(B_5451);
  
	adc(0x02);
	clc;
B_5451:
	rol(0x04);
	sta(0x03);
	rts;
  

}
end_subroutine

subroutine(P_sound_routine_5456)
{

	lda(VIA_PA);
	pha;
	sei;
	lda_imm(0xFF);
	sta(VIA_PCR);
	stx(VIA_PA);
	lda_imm(0xFD);
	sta(VIA_PCR);
	sty(VIA_PA);
	lda_imm(0xDD);
	sta(VIA_PCR);
	lda_imm(0x00);
	sta(VIA_PA);
	cli;
	cpx_imm(0x07);
	bne(B_547D);
  
	sty(0x6101);
B_547D:
	pla;
	sta(VIA_PA);
	rts;
  

}
end_subroutine

subroutine(P_sound_routine_5482)
{

	lda(VIA_PA);
	pha;
	dec(0x03);
	lda(0x03);
	cmp_imm(0x03);
	bcs(B_54A0);
  
	asl_A();
	tax;
	ldy(0x04);
	jsr(P_sound_routine_5456);
	inx;
	ldy(0x05);
	jsr(P_sound_routine_5456);
	pla;
	sta(VIA_PA);
	rts;
  
B_54A0:
	ldx_imm(0x06);
	ldy(0x04);
	jsr(P_sound_routine_5456);
	pla;
	sta(VIA_PA);
	rts;
  

}
end_subroutine

subroutine(P_54AC)
{

	ldx_imm(0x07);
	ldy_imm(0xFF);
	jsr(P_sound_routine_5456);
	ldx_imm(0x08);
B_54B5:
	ldy(0x04FD);
	jsr(P_sound_routine_5456);
	inx;
	cpx_imm(0x0B);
	bne(B_54B5);
  
	rts;
  

}
end_subroutine

subroutine(P_54C1)
{

	jsr(P_init_54EB);
	jsr(P_init_hiscores_54C8);
	rts;
  

}
end_subroutine

subroutine(P_init_hiscores_54C8)
{

	ldx_imm(0x00);
B_54CA:
	lda_imm(0x00);
	sta_X(0x9C00);
	dex;
	bne(B_54CA);
  
	ldx_imm(0x39);
B_54D4:
	lda_imm(0x01);
	sta_X(0x9C00);
	txa;
	sec;
	sbc_imm(0x06);
	tax;
	bpl(B_54D4);
  
	lda_imm(0x2E);
	ldx_imm(0xC8);
B_54E4:
	sta_X(0x9C7F);
	dex;
	bne(B_54E4);

	load_hiscore();
  

}
end_subroutine

void P_init_54EB()
{

	lda_imm(0x00);
	ldx_imm(0x28);
B_54EF:
	sta_X(0x9610);
	dex;
	bpl(B_54EF);
  
	lda_imm(get_max_nb_lives());
	sta(number_of_lives_9616);
	lda(0x0410);
	sta(0x9619);
	rts;
  

}


subroutine(P_display_bridge_part_5501)
{

    if (!peek(materialize_bridge_part_flag_617A))
    {
	sound_play("bridge_appears");
    }
	lda_imm(0x01);
	sta(materialize_bridge_part_flag_617A);
	lda_imm(0x11);
	sta(0x6179);
	lda(0x617B);
	sec;
	sbc_imm(0xE0);
	sta(0x00);
	lda(0x617C);
	sbc_imm(0x01);
	sta(0x01);
	ldy_imm(0x03);
	lda_imm(0x40);
	sta_ind_Y(0x00);
	rts;
  

}
end_subroutine

subroutine(P_materialize_bridge_part_5521)
{
    // 6179: index of the bridge materialization: 17 -> 1
	lda_imm(0x00);
	sta(0x00);
	lda(0x617B);
	sta(0x01);
	lda(0x617C);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x12);
	sta(0x06);
	lda_imm(0x06);
	sta(0x07);
	lda(0x6179);
	sta(0x08);
	jsr(P_screen_routine_53D5);
	lda(0x617B);
	sta(0x01);
	lda(0x617C);
	sta(0x02);
	lda_imm(0x83);
	sta(0x03);
J_5555:
	lda_imm(0x90);
	sta(0x04);
	dec(0x6179);
	lda_imm(0x06);
	sta(0x07);
	lda(0x6179);
	sta(0x08);
	lda_imm(0x00);
	sta(0x00);
	
	P_5305(0);  // draw the lines
	
	lda(0x6179);
	beq(B_5572);
  
	rts;
  
B_5572:
	lda_imm(0x00);
	sta(materialize_bridge_part_flag_617A);
	rts;
  

}
end_subroutine

subroutine(P_5578)
{

	cmp_imm(0xFF);
	beq(B_55B2);
  
	ldy_imm(0x00);
	lda(0x6189);
	sta(0x00);
B_5583:
	ldy(0x05);
B_5585:
	lda_ind_Y(0x01);
	cmp_imm(0x41);
	bcc(B_5591);
  
	and(0x00);
	ora_imm(0x40);
	sta_ind_Y(0x01);
B_5591:
	clc;
	adc(VIA_T2CL);
	sta(0x00);
	dey;
	bne(B_5585);
  
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	cmp(0x04);
	bne(B_5583);
  
	lda(0x01);
	cmp(0x03);
	bcc(B_5583);
  
	rts;
  
B_55B2:
	ldy(0x05);
B_55B4:
	lda_imm(0x40);
	sta_ind_Y(0x01);
	dey;
	bne(B_55B4);
  
	lda(0x01);
	clc;
	adc_imm(0x28);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	cmp(0x04);
	bcc(B_55B2);
  
	lda(0x01);
	cmp(0x03);
	bcc(B_55B2);
  
	rts;
  

}
end_subroutine

subroutine(P_55D3)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xBF);
	sta(0x02);
	lda_imm(0x68);
	sta(0x01);
	lda_imm(0x02);
	sta(0x03);
	lda_imm(0x61);
	sta(0x04);
	lda_imm(0x78);
	sta(0x05);
	lda_imm(0x78);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	jsr(P_update_player_status_5614);
	lda(0x618B);
	sta(0xBF68);
	sta(0xBF90);
	sta(0xBFB8);
	sta(0xBFE0);
	lda(0x618C);
	sta(0xBF69);
	sta(0xBF91);
	sta(0xBFB9);
	sta(0xBFE1);
	rts;
  

}
end_subroutine

subroutine(P_update_player_status_5614)
{

	ldx_imm(0x05);
B_5616:
	lda_X(0x9610);
	ora_imm(0x30);
	sta_X(0xBF92);
	dex;
	bpl(B_5616);
  
	ldx_imm(0x05);
B_5623:
	lda_X(0x9610);
	ora_imm(0x30);
	sta_X(0xBF92);
	dex;
	bpl(B_5623);
  
	lda(number_of_lives_9616);
	ora_imm(0x30);
	sta(0xBFA2);
	lda(0x9619);
	ora_imm(0x30);
	sta(0xBFC7);
	ldx_imm(0x06);
	ldy_imm(0x00);
B_5642:
	lda_X(0x9C35);
	ora_imm(0x30);
	sta_Y(0xBFB0);
	iny;
	dex;
	bne(B_5642);  

}
end_subroutine

subroutine(P_564F)
{

	pha;
	txa;
	pha;
	tya;
	pha;
	php;
	ldx_imm(0x04);
B_5657:
	lda_X(0x9610);
	clc;
	adc_X(0x961A);
	cmp_imm(0x0A);
	bcs(B_567C);
  
J_5662:
	sta_X(0x9610);
	dex;
	bne(B_5657);
  
	ldx_imm(0x04);
	lda_imm(0x00);
B_566C:
	sta_X(0x961A);
	dex;
	bne(B_566C);
  
	jsr(P_update_player_status_5614);
	plp;
	pla;
	tay;
	pla;
	tax;
	pla;
	rts;
  
B_567C:
	sbc_imm(0x0A);
	dex;
	inc_X(0x9610);
	inx;
	jmp(J_5662);
  

}
end_subroutine

subroutine(P_5686)
{
	lda(0x6180);
	sec;
	sbc_imm(0x05);
	sta(0x6180);
	bpl(B_56B5);
  
	lda_imm(0x05);
	sta(0x6180);
	dec(0x617F);
	bpl(B_56B5);
  
	lda_imm(0x09);
	sta(0x617F);
	dec(0x617E);
	bne(B_56B5);
  
	lda_imm(0x01);
	sta(0x617E);
	lda_imm(0x00);
	sta(0x617D);
	sta(0x617F);
	sta(0x6180);
B_56B5:
	jsr(P_56B9);
	rts;
  

}
end_subroutine

subroutine(P_56B9)
{

	ldx_imm(0x02);
B_56BB:
	lda_X(0x617E);
	ora_imm(0x30);
	sta_X(0xBFD3);
	dex;
	bpl(B_56BB);
  
	rts;
  

}
end_subroutine

subroutine(P_56C7)
{

	lda_imm(0x01);
	sta(0x6195);
	lda_imm(0x10);
	sta(0x6194);
	lda_imm(0x02);
	sta(0x00);
	sta(0x618E);
	lda(0x6189);
	and_imm(0x1F);
	clc;
	adc_imm(0x07);
	sta(0x05);
	cmp_imm(0x23);
	bcs(B_571F);
  
	lda(0x618A);
	and_imm(0x1F);
	sta(0x00);
	cmp_imm(0x02);
	bcc(B_56F5);
  
	dec(0x00);
	dec(0x00);
B_56F5:
	lda_imm(0xF0);
	sta(0x01);
	jsr(P_541C);
	lda(0x02);
	clc;
	adc(0x05);
	sta(0x618F);
	lda(0x03);
	adc_imm(0xA0);
	sta(0x6190);
	lda(0x05);
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	jsr(P_541C);
	lda(0x02);
	clc;
	adc_imm(0x02);
	sta(0x6193);
	rts;
  
B_571F:
	lda_imm(0x00);
	sta(0x6195);
	rts;
  

}
end_subroutine

subroutine(E_5725)
{

	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x92);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(E_5739)
{

	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x92);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	lda_imm(0x06);
	sta(0x07);
	lda(0x6194);
	sta(0x08);
	P_5305(0);
	rts;
}
end_subroutine

subroutine(J_5756)
{
	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x92);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_576A)
{

	lda_imm(0x2C);
	sta(0x03);
	lda_imm(0x92);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	lda_imm(0x06);
	sta(0x07);
	lda(0x6194);
	sta(0x08);
P_5305(0);
	rts;
  

}
end_subroutine

subroutine(P_5787)
{

	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	lda_imm(0x06);
	sta(0x07);
	lda(0x6194);
	sta(0x08);
	jsr(P_screen_routine_53D5);
	rts;
  

}
end_subroutine

subroutine(P_57A0)
{

	lda(0x6194);
	bne(B_57F2);
  
	dec(0x6193);
	beq(B_57D3);
  
	dec(0x618E);
	lda(0x618E);
	sta(0x00);
	bpl(B_5817);
  
	lda_imm(0x05);
	sta(0x00);
	sta(0x618E);
	lda(0x618F);
	sec;
	sbc_imm(0x01);
	sta(0x01);
	sta(0x618F);
	lda(0x6190);
	sbc_imm(0x00);
	sta(0x6190);
	sta(0x02);
	jmps(J_5756);
  
B_57D3:
	lda_imm(0x00);
	sta(0x6195);
	lda(0x618F);
	sta(0x01);
	lda(0x6190);
	sta(0x02);
	lda_imm(0x40);
	sta(0x03);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x2C);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  
B_57F2:
	lda(0x618F);
	sta(0x01);
	lda(0x6190);
	sta(0x02);
	jsr(P_5787);
	dec(0x6194);
	dec(0x6194);
	lda_imm(0x02);
	sta(0x00);
	lda(0x618F);
	sta(0x01);
	lda(0x6190);
	sta(0x02);
	jsr(P_576A);
	rts;
  
B_5817:
	lda(0x618F);
	sta(0x01);
	lda(0x6190);
	sta(0x02);
	jmps(J_5756);
  

}
end_subroutine

subroutine(P_5824)
{

	ldx_imm(0x07);
B_5826:
	lda_imm(0x01);
	sta_X(0x619E);
	dex;
	bpl(B_5826);
  
	lda(0x07);
	sec;
	sbc_imm(0x01);
	sta(0x01);
	lda(0x08);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x02);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x20);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda(0x07);
	sec;
	sbc_imm(0x01);
	sta(0x01);
	lda(0x08);
	sbc_imm(0x00);
	sta(0x02);
	lda_imm(0x03);
	sta(0x03);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x0D);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	ldx(0x09);
	ldy_imm(0x03);
B_586A:
	tya;
	sta_X(0x6196);
	inx;
	dey;
	bpl(B_586A);
  
	lda_imm(0x04);
	sta(0x61A6);
	lda_imm(0x05);
	sta(0x61A7);
	lda_imm(0x06);
	sta(0x61A8);
	lda_imm(0x07);
	sta(0x61A9);
	lda_imm(0x07);
	sta(0x61AA);
	lda_imm(0x06);
	sta(0x61AB);
	lda_imm(0x05);
	sta(0x61AC);
	lda_imm(0x04);
	sta(0x61AD);
	lda_imm(0x00);
	sta(0x00);
	lda(0x07);
	clc;
	adc_imm(0xF2);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0xE5);
	sta(0x03);
	lda_imm(0x71);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x34);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	J_58BE();
}
end_subroutine

subroutine(J_58BE)
{
	ldx(0x09);
	lda_X(0x6196);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	lda(0x08);
	sta(0x02);
	jsr(P_591C);
	inc(0x09);
	ldx(0x09);
	lda_X(0x6196);
	sta(0x00);
	lda(0x07);
	clc;
	adc_imm(0x04);
	sta(0x01);
	lda(0x08);
	adc_imm(0x00);
	sta(0x02);
	jsr(P_593C);
	inc(0x09);
	ldx(0x09);
	lda_X(0x6196);
	sta(0x00);
	clc;
	lda(0x07);
	adc_imm(0x08);
	sta(0x01);
	lda(0x08);
	adc_imm(0x02);
	sta(0x02);
	jsr(P_5930);
	inc(0x09);
	ldx(0x09);
	lda_X(0x6196);
	sta(0x00);
	lda(0x07);
	clc;
	adc_imm(0x0C);
	sta(0x01);
	lda(0x08);
	adc_imm(0x02);
	sta(0x02);
	jsr(P_5948);
	rts;
  

}
end_subroutine

subroutine(P_591C)
{

	lda_imm(0x19);
	sta(0x03);
	lda_imm(0x72);
	sta(0x04);
	lda_imm(0x02);
	sta(0x05);
	lda_imm(0x18);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_5930)
{

	lda_imm(0x79);
	sta(0x03);
	lda_imm(0x72);
	sta(0x04);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_593C)
{

    lda_imm(0xD9);
    sta(0x03);
    lda_imm(0x72);
    sta(0x04);
    jsr(P_blit_to_screen_5285);
    rts;
  

}
end_subroutine

subroutine(P_5948)
{

    lda_imm(0x39);
    sta(0x03);
    lda_imm(0x73);
    sta(0x04);
    jsr(P_blit_to_screen_5285);
    rts;
  

}
end_subroutine

subroutine(P_animate_quadnog_1_5954)
{

    ldx(0x09);
	ldy_imm(0x03);
	B_5958:
	dec_X(0x619E);
	bne(B_5970);
	
	lda_X(0x61A6);
	sta_X(0x619E);
	dec_X(0x6196);
	lda_X(0x6196);
	bpl(B_5970);
  
	lda_imm(0x03);
	sta_X(0x6196);
B_5970:
	inx;
	dey;
	bpl(B_5958);
  
	jmps(J_58BE);
  

}
end_subroutine

subroutine(P_5977)
{

	lda_imm(0x0C);
	jsr(P_clear_text_screen_5A57);
	lda_imm(0x00);
	sta(0x07);
	lda(0x08);
	sta(0x06);
J_5984:
	lda(0x06);
	sta(0x00);
	lda(0x07);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x09);
	cmp_imm(0x16);
	beq(B_59B2);
  
	cmp_imm(0x0D);
	bne(B_599B);
  
	jsr(P_59B3);
B_599B:
	jsr(P_clear_text_screen_5A57);
	inc(0x06);
	lda(0x06);
	cmp_imm(0x28);
	bne(B_59A9);
  
	jsr(P_59B3);
B_59A9:
	inc(0x09);
	bne(B_59AF);
  
	inc(0x0A);
B_59AF:
	jmp(J_5984);
  
B_59B2:
	rts;
  

}
end_subroutine

subroutine(P_59B3)
{

	lda(0x08);
	sta(0x06);
	inc(0x07);
	rts;
  
}
end_subroutine

subroutine(P_main_menu_59D4)
{
    while(1)
    {
	lda_imm(0x35);
	sta(0x09);
	lda_imm(0x79);
	sta(0x0A);
	lda_imm(0x05);
	sta(0x08);
	jsr(P_5977);
	lda_imm(0x01);
	sta(0x04);
	sta(0x03);
	sta(0x05);

	OricTimer tmr = timer_init(4,80);
	
	while(1)
	{
	
	    timer_tick(&tmr);
	
	    ldx_imm(0x08);
	    ldy_imm(0x06);
	    jsr(P_animate_menu_5A0C);

	    SDL_Event event;
	
	    while (SDL_PollEvent(&event))
	    {
		flush_events();
	    
		switch(event.type)
		{
		  case SDL_QUIT:
		    quit();
		    break;
		  case SDL_JOYBUTTONDOWN:
		    rts;
		    break;
		  case SDL_KEYDOWN:	      
		    switch (event.key.keysym.sym)
		    {
		      case SDLK_ESCAPE:
			quit();
			break;
		      case SDLK_RCTRL:
		      case SDLK_LCTRL:
			rts;
			break;
		      case SDLK_s:
			jmps(J_5F25);
			  break;
			default:
			  break;
		    }
		}		    
	    }

	
	}

    }
  
}
end_subroutine

subroutine(P_animate_menu_5A0C)
{
  
	lda_imm(0x08);
	sta(0x04);
	dec(0x05);
	bne(B_5A20);
  
	lda_imm(0x07);
	sta(0x05);
B_5A20:
	lda(0x05);
	sta(0x02);
	lda_imm(0x20);
	sta(0x00);
	lda_imm(0xBC);
	sta(0x01);
B_5A2C:
	dec(0x02);
	bne(B_5A34);
  
	lda_imm(0x07);
	sta(0x02);
B_5A34:
	lda(0x02);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x50);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dex;
	bne(B_5A2C);
  
  B_5A48:
  cpu_random_8_bit();  // so mission selection is really random
	rts;
  

  

}
end_subroutine

subroutine(P_clear_text_screen_5A57)
{

	cmp_imm(0x0C);
	bne(B_GEN_1);

	B_5A54:

	cpu_memset(0xBB80,0xC000-0xBB80,0x20);
	/*
	  overlapped to ROM: not good :)
	lda_imm(0xBB);
	sta(0x01);
	lda_imm(0x80);
	sta(0x00);
	ldy_imm(0x00);
B_59C4:
	lda_imm(0x20);
	sta_ind_Y(0x00);
	dey;
	bne(B_59C4);
  
	inc(0x01);
	lda(0x01);
	cmp_imm(0xC0);
	bne(B_59C4);
	*/
	
	rts;
B_GEN_1:
	sta(0x03);
	tya;
	pha;
	txa;
	pha;
	lda(0x03);
	pha;
	lda(0x00);
	pha;
	lda(0x01);
	sta(0x00);
	lda_imm(0x28);
	sta(0x01);
	jsr(P_541C);
	pla;
	clc;
	adc(0x02);
	sta(0x01);
	lda(0x03);
	adc_imm(0x00);
	sta(0x02);
	lda(0x01);
	clc;
	adc_imm(0x80);
	sta(0x01);
	lda(0x02);
	adc_imm(0xBB);
	sta(0x02);
	cmp_imm(0xA0);
	bcc(B_5A99);
  
	pla;
	ldy_imm(0x00);
	sta_ind_Y(0x01);
J_5A94:
	pla;
	tax;
	pla;
	tay;
	rts;
  
B_5A99:
	pla;
	jmp(J_5A94);
  

}
end_subroutine

subroutine(P_5A9D)
{

	lda_imm(0x00);
	sta(0x6196);
	sta(0x6197);
	lda_imm(0x05);
	sta(0x6198);
	jsr(P_5B19);
	lda_imm(0x01);
	sta(0x04);
	sta(0x03);
	sta(0x05);
	ldx_imm(0x0A);
	ldy_imm(0x05);
	jsr(P_animate_menu_5A0C);
	jsr(P_enter_hiscore_5B90);
	lda(0x0400);
	beq(B_5ACA);
  
	jsr(P_5D07);
	jsr(P_5B19);
B_5ACA:
	jsr(P_5DA0);
	lda_imm(0x01);
	sta(0x04);
	sta(0x03);
	sta(0x05);

	OricTimer tmr = timer_init(1,50);
	B_5AD5:
timer_tick(&tmr);

	dec(0x6196);
	bne(B_5AEA);
  
	dec(0x6197);
	bne(B_5AEA);
  
	dec(0x6198);
	bne(B_5AEA);
  
	lda_imm(0x20);
	sta(0x041F);
	rts;
B_5AEA:
	tmr = timer_init(1,100);
	SDL_Event event;
	set_game_context(MENU);
	
	while(1)
	{
	    // randomize
	    
	    cpu_random_8_bit();
	    
	    ldx_imm(0x0A);
	    ldy_imm(0x05);
	    jsr(P_animate_menu_5A0C);

	    timer_tick(&tmr);
	    
	    // hiscore display: ESC or SPACE
	    	    	 
	    if (SDL_PollEvent(&event))
	    {
		flush_events();
		switch(event.type)
		{
		  case SDL_JOYBUTTONUP:
		    goto J_5B13;		
		  case SDL_QUIT:
		    quit();
		    break;
		  case SDL_KEYDOWN:	      
		    switch (event.key.keysym.sym)
		    {
		      case SDLK_ESCAPE:
			lda_imm(0x0);
			sta(0x041F);
			rts;
			break;
		      case SDLK_RCTRL:
		      case SDLK_LCTRL:
		      goto J_5B13;
		    default:
		      break;
		    }	    
		}
	    }
	}

  
J_5B13:
	lda_imm(0x80);
	sta(0x041F);
	rts;
  

}
end_subroutine

subroutine(P_5B19)
{

	lda_imm(0x7A);
	sta(0x0A);
	lda_imm(0xD3);
	sta(0x09);
	lda_imm(0x00);
	sta(0x08);
	jsr(P_5977);
	lda_imm(0x04);
	sta(0x0A);
	lda_imm(0x0A);
	sta(0x08);
	ldx_imm(0x3B);
B_5B32:
	ldy_imm(0x06);
	lda_imm(0x05);
	sta(0x09);
	sta(0x00);
	lda(0x0A);
	sta(0x01);
	lda_imm(0x03);
	jsr(P_clear_text_screen_5A57);
	inc(0x09);
B_5B45:
	lda(0x09);
	sta(0x00);
	lda(0x0A);
	sta(0x01);
	lda_X(0x9C00);
	ora_imm(0x30);
	jsr(P_clear_text_screen_5A57);
	dex;
	inc(0x09);
	dey;
	bne(B_5B45);
  
	inc(0x0A);
	inc(0x0A);
	dec(0x08);
	bne(B_5B32);
  
	lda_imm(0x0A);
	sta(0x08);
	lda_imm(0x04);
	sta(0x0A);
	ldx_imm(0xC7);
B_5B6D:
	ldy_imm(0x14);
	lda_imm(0x12);
	sta(0x09);
B_5B73:
	lda(0x09);
	sta(0x00);
	lda(0x0A);
	sta(0x01);
	lda_X(0x9C80);
	jsr(P_clear_text_screen_5A57);
	dex;
	inc(0x09);
	dey;
	bne(B_5B73);
  
	inc(0x0A);
	inc(0x0A);
	dec(0x08);
	bne(B_5B6D);
  
	rts;
  

}
end_subroutine

subroutine(P_enter_hiscore_5B90)
{
	set_game_context(MENU);

	ldx_imm(0x00);
	ldy_imm(0x05);
B_5B94:
	lda_X(0x9610);
	cmp_Y(0x9C00);
	bcc(B_5BA4);
  
	bne(B_5BAA);
  
	inx;
	dey;
	cpx_imm(0x06);
	bne(B_5B94);
  
B_5BA4:
	lda_imm(0x00);
	sta(0x0400);
	rts;
  
B_5BAA:
	lda_imm(0x06);
	sta(0x09);
	ldx_imm(0x00);
B_5BB0:
	lda_imm(0x1A);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x9610);
	ora_imm(0x30);
	jsr(P_clear_text_screen_5A57);
	inc(0x09);
	inx;
	cpx_imm(0x06);
	bne(B_5BB0);
  
	ldx_imm(0x13);
	lda_imm(0x20);
B_5BCB:
	sta_X(0x9C80);
	dex;
	bpl(B_5BCB);
  
	ldx_imm(0x27);
	stx(0x09);
B_5BD5:
	lda_imm(0x18);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x7B37);
	jsr(P_clear_text_screen_5A57);
	dec(0x09);
	dex;
	bpl(B_5BD5);
  
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

J_5C01:
	
	lda_imm(0x13);
	sec;
	sbc(0x0400);
	sta(0x0404);
	jsr(P_enter_hiscore_letter_5C74);
	cmp_imm(0x7F);
	beq(B_5C52);
  
	cmp_imm(0x0D);  // end
	beq(B_5C3C);
  
	ldx(0x09);
	cpx_imm(0x26);
	bcs(J_5C01);
  
	ldy(0x0404);
	sta_Y(0x9C80);
	sta(0x0401);
	lda(0x09);
	sta(0x00);
	lda_imm(0x1A);
	sta(0x01);
	lda(0x0401);
	and_imm(0x7F);
	jsr(P_clear_text_screen_5A57);
	inc(0x09);
	inc(0x0400);
	jmp(J_5C01);
  
B_5C3C:
	ldx_imm(0x00);
	ldy_imm(0x05);
B_5C40:
	lda_Y(0x9610);
	sta_X(0x9C00);
	dey;
	inx;
	cpx_imm(0x06);
	bne(B_5C40);
  
	lda_imm(0x01);
	sta(0x0400);
	rts;
  
B_5C52:
	lda(0x0400);
	beq(J_5C01);
  
	dec(0x0400);
	dec(0x09);
	ldy(0x0404);
	lda_imm(0x20);
	sta_Y(0x9C81);
	lda(0x09);
	sta(0x00);
	lda_imm(0x1A);
	sta(0x01);
	lda_imm(0x20);
	jsr(P_clear_text_screen_5A57);
	jmp(J_5C01);
  

}
end_subroutine

subroutine(P_enter_hiscore_letter_5C74)
{

    while(1)
    {
	
	// hiscore entry

	oric_wait(6);
	
	update_controls();

	if ((controls.fire_pressed) && (!controls.fire_was_pressed))
	{

	    
	    ldx(0x0402);
	    lda_imm(0x01);
	    sta(0x0403);
	    lda_X(0xBF40);
	    jsr(P_decode_hiscore_entry_5CEF);

	    while (1)
	    {
		oric_wait(10);
		update_controls();
		if (!controls.fire_pressed) break;
	    }
	    break;
	}
	if (controls.left_pressed)
	{
	    ldx(0x0402);
	   
	    if (!r.z)
	    {		

		lda_X(0xBF40);
		and_imm(0x7F);
		sta_X(0xBF40);
		dex;
		lda_X(0xBF40);
		ora_imm(0x80);
		sta_X(0xBF40);
		stx(0x0402);
	    }
	    
	}
  
	if (controls.right_pressed)

	{
	    ldx(0x0402);
	    
	    cpx_imm(0x27);
	    if (!r.z)
	    {
	
		lda_X(0xBF40);
		and_imm(0x7F);
		sta_X(0xBF40);
		inx;
		lda_X(0xBF40);
		ora_imm(0x80);
		sta_X(0xBF40);
		stx(0x0402);
	    }
	}
    }
  

}
end_subroutine

subroutine(P_decode_hiscore_entry_5CEF)
{

	and_imm(0x7F);
	cmp_imm(0x61);
	bcc(B_5D06);
  
	cmp_imm(0x72);
	beq(B_5D04);
  
	cmp_imm(0x75);
	beq(B_5D04);
  
	cmp_imm(0x62);
	beq(B_5D04);
  
	lda_imm(0x0D);
	rts;
  
B_5D04:
	lda_imm(0x7F);
B_5D06:
	rts;
  

}
end_subroutine

subroutine(P_5D07)
{

	lda_imm(0x01);
	sta(0x0400);
J_5D0C:
	inc(0x0400);
	lda(0x0400);
	cmp_imm(0x0B);
	beq(B_5D34);
  
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	jsr(P_541C);
	ldy(0x02);
	ldx_imm(0x06);
B_5D23:
	lda_X(0x9BFF);
	cmp_Y(0x9BFF);
	bcc(B_5D34);
  
	bne(J_5D0C);
  
	dex;
	dey;
	bne(B_5D23);
  
	jmp(J_5D0C);
  
B_5D34:
	ldy_imm(0x06);
B_5D36:
	lda_Y(0x9BFF);
	sta_Y(0x0480);
	dey;
	bne(B_5D36);
  
	dec(0x0400);
	lda(0x0400);
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	jsr(P_541C);
	ldy_imm(0x06);
B_5D50:
	lda_Y(0x9BFF);
	sta_Y(0x9BF9);
	iny;
	cpy(0x02);
	beq(B_5D50);
  
	bcc(B_5D50);
  
	ldy(0x02);
	ldx_imm(0x06);
B_5D61:
	lda_X(0x0480);
	sta_Y(0x9BFF);
	dey;
	dex;
	bne(B_5D61);
  
	lda(0x0400);
	sta(0x00);
	lda_imm(0x14);
	sta(0x01);
	jsr(P_541C);
	ldy_imm(0x14);
B_5D79:
	lda_Y(0x9C7F);
	sta_Y(0x0480);
	dey;
	bne(B_5D79);
  
	ldy_imm(0x14);
B_5D84:
	lda_Y(0x9C7F);
	sta_Y(0x9C6B);
	iny;
	cpy(0x02);
	beq(B_5D84);
  
	bcc(B_5D84);
  
	ldy(0x02);
	ldx_imm(0x14);
B_5D95:
	lda_X(0x0480);
	sta_Y(0x9C7F);
	dey;
	dex;
	bne(B_5D95);
  
	rts;
  

}
end_subroutine

subroutine(P_5DA0)
{

	ldx_imm(0x26);
	stx(0x09);
B_5DA4:
	lda_imm(0x1A);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x61B6);
	jsr(P_clear_text_screen_5A57);
	dec(0x09);
	dex;
	bpl(B_5DA4);
  
	rts;
  
}
end_subroutine

subroutine(P_5E4E)
{

	ldy(0x04FD);
	ldx_imm(0x08);
	jsr(P_sound_routine_5456);
	rts;
  

}
end_subroutine

subroutine(P_adjust_skill_5E57)
{
    update_controls();
    
    lda(0x61DD);

    if (controls.left_pressed)
    {
	if (r.a > 2)
	{
	    dec(0x61DD);
	    jmpl(P_5E9D);
	}
    }
    if (controls.right_pressed)
    {
	if (r.a < 0x27)
	{
	    inc(0x61DD);
	    jmpl(P_5E9D);
	}
    }
    if (controls.fire_pressed)
    {
	move_ff_41f();
	rts;
    }
}
end_subroutine

subroutine(P_5E9D)
{

	sta(0x61DE);
	ldx_imm(0x06);
B_5EA2:
	lda(0x61DD);
	sta(0x00);
	stx(0x01);
	lda_imm(0x10);
	jsr(P_clear_text_screen_5A57);
	inx;
	cpx_imm(0x0B);
	bne(B_5EA2);
  
	ldx_imm(0x06);
B_5EB5:
	lda(0x61DE);
	sta(0x00);
	stx(0x01);
	lda_imm(0x20);
	jsr(P_clear_text_screen_5A57);
	inx;
	cpx_imm(0x0B);
	bne(B_5EB5);
  
	lda(0x61E1);
	beq(B_5EE0);
  
	lda(0x61DD);
	lsr_A();
	lsr_A();
	sta(0x0410);
	lda_imm(0x09);
	sec;
	sbc(0x0410);
	sta(0x0410);
	jsr(P_5FCD);
	rts;
  
B_5EE0:
	lda(0x61DD);
	lsr_A();
	cmp_imm(0x10);
	bcc(B_5EF1);
  
	lda_imm(0x0F);
	sta(0x04FD);
	jsr(P_5E4E);
	rts;
  
B_5EF1:
	sta(0x04FD);
	jsr(P_5E4E);
	rts;
  

}
end_subroutine

subroutine(P_5EF8)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x05);
	sta(0x01);
	lda_imm(0x12);
	jsr(P_clear_text_screen_5A57);
	ldx_imm(0x06);
B_5F07:
	lda_imm(0x00);
	sta(0x00);
	stx(0x01);
	lda_imm(0x11);
	jsr(P_clear_text_screen_5A57);
	inx;
	cpx_imm(0x0B);
	bne(B_5F07);
  
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x0B);
	sta(0x01);
	lda_imm(0x12);
	jsr(P_clear_text_screen_5A57);
	rts;
}
end_subroutine

subroutine(J_5F25)
{
	lda_imm(0x62);
	sta(0x09);
	lda_imm(0x60);
	sta(0x0A);
	lda_imm(0x07);
	sta(0x08);
	jsr(P_5977);
	lda_imm(0x01);
	sta(0x61E1);
	ldx_imm(0x05);
	lda_imm(0x1B);
	sta(0x09);
B_5F3F:
	lda_imm(0x11);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x60D3);
	jsr(P_clear_text_screen_5A57);
	dec(0x09);
	dex;
	bpl(B_5F3F);
  
	ldx_imm(0x05);
	lda_imm(0x16);
	sta(0x09);
B_5F58:
	lda_imm(0x00);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x60D3);
	jsr(P_clear_text_screen_5A57);
	dec(0x09);
	dex;
	bpl(B_5F58);
  
	ldx_imm(0x27);
	stx(0x09);
B_5F6F:
	lda_imm(0x0C);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x60D9);
	jsr(P_clear_text_screen_5A57);
	dec(0x09);
	dex;
	bpl(B_5F6F);
  
	ldx_imm(0x05);
	lda_imm(0x14);
	sta(0x09);
B_5F88:
	lda_imm(0x04);
	sta(0x01);
	lda(0x09);
	sta(0x00);
	lda_X(0x60D3);
	jsr(P_clear_text_screen_5A57);
	dec(0x09);
	dex;
	bpl(B_5F88);
  
	jsr(P_5FCD);
	lda(0x61E0);
	sta(0x61DD);
	clc;
	adc_imm(0x01);
	jsr(P_5E9D);
	jsr(P_5EF8);
	lda_imm(0x00);
	sta(0x041F);
	
	OricTimer tmr_skill = timer_init(1,50);
	do
	{
	    timer_tick(&tmr_skill);
	    jsr(P_adjust_skill_5E57);
	}
	while (!peek(0x041F));
  
	lda(0x61DD);
	sta(0x61E0);
	jmpl(P_main_menu_59D4);
  

}
end_subroutine

subroutine(P_5FCD)
{

	lda_imm(0x04);
	sta(0x01);
	lda_imm(0x16);
	sta(0x00);
	lda(0x0410);
	ora_imm(0x30);
	jsr(P_clear_text_screen_5A57);
	rts;
  

}
end_subroutine

// < A: line
// < X: column (or the other way round, who cares?)

subroutine(P_read_keyboard_matrix_5FDE)
{
    int keycode = (((int)r.a) << 8) + r.x;
    int rval = 0;
    
    switch (keycode)
    {
      case 0x04DF:
	rval = controls.left_pressed;
	break;
      case 0x04BF:
	rval = controls.right_pressed;
	break;
      case 0x04FE:
	rval = controls.fire_pressed;
	break;
     case 0x00BF:
       rval = controls.up_pressed;
	break;
      case 0x4FF:  // hack to make ship work
       rval = controls.down_pressed;
	break;
       break;
      default:
	printf("unsup kc %x\n",keycode);fflush(stdout);
	break;
    }

    r.z = !rval;
    
    /*
      
	sta(VIA_PB);
	lda_imm(0x0E);
	tay;
	jsr(P_5FEE);
	lda(VIA_PB);
	and_imm(0x08);
	cli;
	rts;*/
  

}
end_subroutine
// sound or keyboard routine?

subroutine(P_5FEE)
{

	sei;
	sta(VIA_PANH);
	cmp_imm(0x07);
	bne(B_5FFB);
  
	txa;
	ora_imm(0x40);
	bne(B_5FFC);
  
B_5FFB:
	txa;
B_5FFC:
	pha;
	ldx_imm(0x0C);
	ldy_imm(0xEE);
	lda_imm(0x11);
	jsr(P_6027);
	ldy_imm(0xCC);
	lda_imm(0x11);
	jsr(P_6027);
	ldy_imm(0xCC);
	lda_imm(0x11);
	jsr(P_6027);
	pla;
	sta(VIA_PANH);
	ldy_imm(0xEC);
	lda_imm(0x11);
	jsr(P_6027);
	ldy_imm(0xCC);
	lda_imm(0x11);
	jsr(P_6027);
	rts;
  

}
end_subroutine

subroutine(P_6027)
{

	and_X(VIA_PB);
	sta(0x00);
	tya;
	ora(0x00);
	sta_X(VIA_PB);
	rts;
  
 

}
end_subroutine

subroutine(P_6200)
{

    set_game_context(MISSION_SELECT);
	jsr(P_6250);
	jsr(P_62BA);
	jsr(P_6357);
#if !defined (START_AT_BRIDGE_LEVEL) && !defined (START_AT_LAST_STAGE)
	jsr(P_select_mission_screen_6429);
#endif
	
	lda(0x041F);
	bpl(B_6212);

	// return to menu
	
	rts;
  
B_6212:
	jsr(P_6250);
	jsr(P_62BA);
	jsr(P_6357);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x98);
	sta(0x01);
	lda_imm(0xBF);
	sta(0x02);
	lda_imm(0xBC);
	sta(0x03);
	lda_imm(0x64);
	sta(0x04);
	lda_imm(0x18);
	sta(0x05);
	sta(0x06);
	jsr(P_blit_to_screen_5285);

	lda_imm(0x00);
	sta(0x041F);
	rts;
  

}
end_subroutine

subroutine(P_6250)
{

	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x68);
	sta(0x01);
	lda_imm(0xBF);
	sta(0x02);
	lda_imm(0x20);
	sta(0x03);
	lda_imm(0x78);
	sta(0x05);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	lda_imm(0x09);
	sta(0x00);
	lda_imm(0x0E);
	sta(0x01);
	lda_imm(0x31);
	jsr(P_104C);
	lda_imm(0x1D);
	sta(0x00);
	lda_imm(0x0E);
	sta(0x01);
	lda_imm(0x32);
	jsr(P_104C);
	lda_imm(0x09);
	sta(0x00);
	lda_imm(0x1F);
	sta(0x01);
	lda_imm(0x33);
	jsr(P_104C);
	lda_imm(0x1D);
	sta(0x00);
	lda_imm(0x1F);
	sta(0x01);
	lda_imm(0x34);
	jsr(P_104C);
	lda_imm(0x14);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x10);
	sta(0x03);
	lda_imm(0x14);
	lda(0x01);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0xC8);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
	rts;
  

}
end_subroutine

subroutine(P_62BA)
{

	lda(0x61AE);
	bmi(B_62CA);
  
	cmp_imm(0x01);
	beq(B_62E1);
  
	lda_imm(0x15);
	sta(0x03);
	jmp(J_62CE);
  
B_62CA:
	lda_imm(0x14);
	sta(0x03);
J_62CE:
	lda_imm(0x00);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x64);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_62E1:
	lda(0x61AF);
	bmi(B_62F1);
  
	cmp_imm(0x01);
	beq(B_6308);
  
	lda_imm(0x15);
	sta(0x03);
	jmp(J_62F5);
  
B_62F1:
	lda_imm(0x14);
	sta(0x03);
J_62F5:
	lda_imm(0x14);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x64);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_6308:
	lda(0x61B0);
	bmi(B_6318);
  
	cmp_imm(0x01);
	beq(B_632F);
  
	lda_imm(0x15);
	sta(0x03);
	jmp(J_631C);
  
B_6318:
	lda_imm(0x14);
	sta(0x03);
J_631C:
	lda_imm(0xA0);
	sta(0x01);
	lda_imm(0xAF);
	sta(0x02);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x64);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_632F:
	lda(0x61B1);
	bmi(B_633F);
  
	cmp_imm(0x01);
	beq(B_6356);
  
	lda_imm(0x15);
	sta(0x03);
	jmp(J_6343);
  
B_633F:
	lda_imm(0x14);
	sta(0x03);
J_6343:
	lda_imm(0xB4);
	sta(0x01);
	lda_imm(0xAF);
	sta(0x02);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x64);
	sta(0x06);
	jsr(P_draw_rectangle_52CE);
B_6356:
	rts;
  

}
end_subroutine

subroutine(P_6357)
{

	ldx(0x61B2);
	dex;
	lda_X(0x61AE);
	beq(B_637C);
  
	ldx_imm(0x00);
	jsr(P_63F6);
	lda_imm(0x01);
	sta(0x00);
	lda_imm(0x5F);
	sta(0x03);
	lda_imm(0x84);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
B_637C:
	ldx(0x61B3);
	dex;
	lda_X(0x61AE);
	beq(B_63AE);
  
	ldx_imm(0x01);
	jsr(P_63F6);
	lda(0x01);
	clc;
	adc_imm(0x01);
	sta(0x01);
	lda(0x02);
	adc_imm(0x00);
	sta(0x02);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x0C);
	sta(0x03);
	lda_imm(0x90);
	sta(0x04);
	lda_imm(0x04);
	sta(0x05);
	lda_imm(0x40);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
B_63AE:
	ldx(0x61B4);
	dex;
	lda_X(0x61AE);
	beq(B_63D0);
  
	ldx_imm(0x02);
	jsr(P_63F6);
	lda_imm(0x00);
	sta(0x09);
	lda(0x01);
	sec;
	sbc_imm(0xEF);
	sta(0x07);
	lda(0x02);
	sbc_imm(0x00);
	sta(0x08);
	jsr(P_5824);
B_63D0:
	ldx(0x61B5);
	dex;
	lda_X(0x61AE);
	beq(B_63F5);
  
	ldx_imm(0x03);
	jsr(P_63F6);
	lda_imm(0x05);
	sta(0x00);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x70);
	sta(0x04);
	lda_imm(0x05);
	sta(0x05);
	lda_imm(0x50);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
B_63F5:
	rts;
  

}
end_subroutine

subroutine(P_63F6)
{

	lda_X(0x61B2);
	cmp_imm(0x02);
	beq(B_640E);
  
	cmp_imm(0x03);
	beq(B_6417);
  
	cmp_imm(0x04);
	beq(B_6420);
  
	lda_imm(0xE6);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	rts;
  
B_640E:
	lda_imm(0xFA);
	sta(0x01);
	lda_imm(0xA6);
	sta(0x02);
	rts;
  
B_6417:
	lda_imm(0x86);
	sta(0x01);
	lda_imm(0xB6);
	sta(0x02);
	rts;
  
B_6420:
	lda_imm(0x9A);
	sta(0x01);
	lda_imm(0xB6);
	sta(0x02);
	rts;
  

}
end_subroutine

subroutine(P_select_mission_screen_6429)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x9A);
	sta(0x01);
	lda_imm(0xBF);
	sta(0x02);
	lda_imm(0xA8);
	sta(0x03);
	lda_imm(0x64);
	sta(0x04);
	lda_imm(0x14);
	sta(0x05);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
B_6446:
	lda_imm(0x00);
	sta(0x041F);
	jsr(P_select_mission_6477);
	dex;
	lda_X(0x61AE);
	bmi(B_6446);
  
	lda_imm(0x01);
	sta_X(0x61AE);
	ldy_imm(0x03);
	inx;
	txa;
B_645D:
	cmp_Y(0x61B2);
	beq(B_6465);
  
	dey;
	bpl(B_645D);
  
B_6465:
	iny;
	sty(0x0411);
	rts;
  

  

}
end_subroutine

// < X: mission number

subroutine(P_select_mission_6477)
{
    SDL_Event event;
    
    int mission = 0;

    while (!mission)
    {
	if (SDL_WaitEvent(&event))
	{
	    flush_events();
	    
	    switch(event.type)
	    {
	      case SDL_QUIT:
		quit();
		break;
	      case SDL_KEYDOWN:	      
		switch (event.key.keysym.sym)
		{
		  case SDLK_ESCAPE:
		    move_ff_41f();
		    mission = -1;
		    break;
		  case SDLK_1:
		  case SDLK_2:
		  case SDLK_3:
		  case SDLK_4:
		    mission = event.key.keysym.sym - SDLK_0;
		      break;
		    default:
		      break;
		}
	    }
	}
    }
    ldx_imm(mission);
    rts;
  
 

}
end_subroutine

/*
subroutine(E_64E0)
{

	lda_imm(0x2E);
	ldx_imm(0xC8);
B_64E4:
	sta_X(0x9C7F);
	dex;
	bne(B_64E4);
  
	rts;
  

}
end_subroutine
*/
/*
subroutine(E_64EB)
{

	lda_imm(0x00);
	ldx_imm(0x28);
B_64EF:
	sta_X(0x9610);
	dex;
	bpl(B_64EF);
  
	lda_imm(0x03);
	sta(number_of_lives_9616);
	lda(0x0410);
	sta(0x8019);

	}
end_subroutine
*/

subroutine(P_6500)
{

	lda_imm(0x0C);
	jsr(P_104C);
	lda_imm(0x0A);
	sta(0x026A);
	jsr(P_6778);
	lda_imm(0x2D);
	sta(0x00);
	lda_imm(0x65);
	sta(0x01);
	lda_imm(0x04);
	sta(0x03);
	lda_imm(0x1B);
	sta(0x04);
	lda_imm(0x78);
	sta(0x05);
	lda_imm(0x10);
	sta(0x06);
	ldx_imm(0x07);
	ldy_imm(0xF8);
	jsr(P_sound_routine_5456);
	rts;
  

}
end_subroutine

subroutine(E_intro_652D)
{

	jsr(P_66FD);
	lda_imm(0x00);
	sta(0x041F);

	// start position : left
	move16(0xB5B4,hero_pos_13D4);
	
	
	lda_imm(0x00);
	sta(0x13D3);
	lda_imm(0xFF);
	sta(0x13D8);
	lda_imm(0xD2);
	sta(0x13DD);

	LOOP_PLAY(run);
	OricTimer tmr = timer_init(1,40);
	
	B_654E:;

	timer_tick(&tmr);


       // hero crosses the screen loop
	
	jsr(P_6699);
	jsr(P_hero_crosses_screen_66C8);
	jsr(P_test_fire_65C8);
	beq(B_6575);
  
	bmi(B_656D);
  
	lda_imm(0x00);
	sta(0x041F);
	jmp(J_65C5);
  
B_656D:
	move_ff_41f();
	jmp(J_65C5);
  
B_6575:
	lda(0x041F);
	beq(B_654E);
	
  	LOOP_STOP(run);

	jsr(P_66DD);
	lda_imm(0x00);
	sta(0x041F);
	sta(0x041E);
	lda_imm(0x06);
	sta(0x9F00);
	lda_imm(0x69);
	sta(0x9F01);
	lda_imm(0x01);
	sta(0x68F2);

	// scrolling credits
tmr = timer_init(1,40);
	
B_6594:
	timer_tick(&tmr);	

	
	jsr(P_6628);
	jsr(P_6699);
	jsr(P_test_fire_65C8);
	beq(B_65BB);
  
	bmi(B_65B3);
  
	lda_imm(0x00);
	sta(0x041F);
	jmp(J_65C5);
  
B_65B3:
move_ff_41f();

	jmp(J_65C5);
  
B_65BB:
	lda(0x041F);
	beq(B_6594);
  
	lda_imm(0x01);
	sta(0x041F);

	
J_65C5:

	LOOP_STOP(run);

	
	// jump indirect cannot be simulated
	
	int addr = deek(0x405);
	switch (addr)
	{
	  case 0x732:
	    E_0732();
	    break;
	  default:
	  {
	      char buf[100];
	      sprintf(buf,"unsupported indirect jump to %x\n",addr);
	      fatal_error(buf);
	  }
	}
//	jmp_ind(J_0405);
}
end_subroutine


subroutine(P_test_fire_65C8)
{
	SDL_Event event;
	// ESC pressed ?

	while (SDL_PollEvent(&event))
	{
	    flush_events();
	    
	    switch(event.type)
	    {
	      case SDL_QUIT:
		quit();
		break;
	      case SDL_KEYDOWN:	      
		switch (event.key.keysym.sym)
		{
		  case SDLK_RCTRL:
		  case SDLK_LCTRL:
		    	lda_imm(0x01);
			rts;
		  break;
		default:
		  break;
		}
	    }
	}
	lda_imm(0x00);
	rts;
	
	/*
	lda(VIA_PB);
	cmp_imm(0xB9);
	bne(B_65D9);
  
	lda(VIA_PA);
	cmp_imm(0xDF);
	bne(B_65D9);
  
	lda_imm(0xFF);
	rts;
  
B_65D9:
	lda(VIA_PB);
	cmp_imm(0xBC);
	bne(B_65EA);
  
	lda(VIA_PA);
	cmp_imm(0xFE);
	bne(B_65EA);
  
	lda_imm(0x01);
	rts;
  
B_65EA:
	lda_imm(0x00);
	rts;
	*/

}
end_subroutine

subroutine(P_65ED)
{

	lda_imm(0xEB);
	sta(0x00);
	lda_imm(0xB0);
	sta(0x01);
	lda_imm(0x13);
	sta(0x02);
	lda_imm(0xB1);
	sta(0x03);
B_65FD:
	ldy_imm(0x00);
	ldx_imm(0x12);
B_6601:
	lda_ind_Y(0x02);
	sta_ind_Y(0x00);
	iny;
	dex;
	bne(B_6601);
  
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	lda(0x02);
	clc;
	adc_imm(0x28);
	sta(0x02);
	lda(0x03);
	adc_imm(0x00);
	sta(0x03);
	cmp_imm(0xBF);
	bne(B_65FD);
  
	rts;
  

}
end_subroutine

subroutine(P_6628)
{

	jsr(P_65ED);
	dec(0x68F2);
	bne(B_664C);
  
	lda(0x041E);
	bne(B_664D);
  
	lda_imm(0x14);
	sta(0x68F2);
	jsr(P_6653);
	ldx_imm(0x0B);
	ldy_imm(0x1F);
	lda_imm(0xF3);
	sta(0x08);
	lda_imm(0x68);
	sta(0x09);
	jsr(P_6749);
B_664C:
	rts;
  
B_664D:
move_ff_41f();
	rts;
  

}
end_subroutine

subroutine(P_6653)
{

	ldx_imm(0x11);
	lda_imm(0x20);
B_6657:
	sta_X(0x68F3);
	dex;
	bpl(B_6657);
  
	ldy_imm(0x00);
	ldx_imm(0x00);
	lda(0x9F00);
	sta(0x00);
	lda(0x9F01);
	sta(0x01);
J_666B:
	lda_ind_Y(0x00);
	iny;
	cmp_imm(0x0D);
	beq(B_667D);
  
	cmp_imm(0x16);
	beq(B_668E);
  
	sta_X(0x68F3);
	inx;
	jmp(J_666B);
  
B_667D:
	tya;
	clc;
	adc(0x9F00);
	sta(0x9F00);
	lda(0x9F01);
	adc_imm(0x00);
	sta(0x9F01);
	rts;
  
B_668E:
	lda_imm(0x01);
	sta(0x041E);
	lda_imm(0x64);
	sta(0x68F2);
	rts;
  

}
end_subroutine

subroutine(P_6699)
{

	lda_imm(0x47);
	sta(0x00);
	lda_imm(0xAB);
	sta(0x01);
	ldy_imm(0x00);
	lda_imm(0x28);
	sta(0x09);
B_66A7:
	dec(0x68F1);
	bne(B_66B1);
  
	lda_imm(0x07);
	sta(0x68F1);
B_66B1:
	lda(0x68F1);
	sta_ind_Y(0x00);
	lda(0x00);
	clc;
	adc_imm(0x28);
	sta(0x00);
	lda(0x01);
	adc_imm(0x00);
	sta(0x01);
	dec(0x09);
	bne(B_66A7);
  
	rts;
  

}
end_subroutine

subroutine(P_hero_crosses_screen_66C8)
{

    
    jsr(P_move_hero_left_0F36);
    ldx_imm(0x07);
    ldy_imm(0xFF);
    jsr(P_sound_routine_5456);
    dec(0x13DD);
    bne(B_66DC);

    move_ff_41f();

    B_66DC:
    rts;
  

}
end_subroutine

subroutine(P_66DD)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0xB2);
	sta(0x01);
	lda_imm(0xB5);
	sta(0x02);
	lda_imm(0x8E);
	sta(0x03);
	lda_imm(0x8F);
	sta(0x04);
	lda_imm(0x03);
	sta(0x05);
	lda_imm(0x42);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  

}
end_subroutine

subroutine(P_66FD)
{

	ldx_imm(0x00);
	ldy_imm(0x14);
	lda_imm(0xCE);
	sta(0x08);
	lda_imm(0x68);
	sta(0x09);
	jsr(P_6749);
	ldx_imm(0x1F);
	ldy_imm(0x14);
	lda_imm(0xCE);
	sta(0x08);
	lda_imm(0x68);
	sta(0x09);
	jsr(P_6749);
	ldx_imm(0x00);
	ldy_imm(0x1D);
	lda_imm(0xD8);
	sta(0x08);
	lda_imm(0x68);
	sta(0x09);
	jsr(P_6749);
	ldx_imm(0x1F);
	ldy_imm(0x1D);
	lda_imm(0xE2);
	sta(0x08);
	lda_imm(0x68);
	sta(0x09);
	jsr(P_6749);
	ldx_imm(0x21);
	ldy_imm(0x1F);
	lda_imm(0xEC);
	sta(0x08);
	lda_imm(0x68);
	sta(0x09);
	jsr(P_6749);
	rts;
  

}
end_subroutine

subroutine(P_6749)
{

	stx(0x13E7);
	sty(0x13E8);
J_674F:
	lda(0x13E7);
	sta(0x00);
	lda(0x13E8);
	sta(0x01);
	ldy_imm(0x00);
	lda_ind_Y(0x08);
	cmp_imm(0x0D);
	beq(B_6777);
  
	jsr(P_104C);
	inc(0x13E7);
	lda(0x08);
	clc;
	adc_imm(0x01);
	sta(0x08);
	lda(0x09);
	adc_imm(0x00);
	sta(0x09);
	jmp(J_674F);
  
B_6777:
	rts;
  

}
end_subroutine

subroutine(P_6778)
{

	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x06);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0xF7);
	sta(0x03);
	lda_imm(0x78);
	sta(0x04);
	lda_imm(0x01);
	sta(0x05);
	lda_imm(0x3A);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x07);
	sta(0x01);
	lda_imm(0xA0);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x6A);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0xEA);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x01);
	sta(0x00);
	lda_imm(0x6F);
	sta(0x01);
	lda_imm(0xA1);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x6A);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0xEA);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x02);
	sta(0x00);
	lda_imm(0xD7);
	sta(0x01);
	lda_imm(0xA2);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x6A);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0xEA);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x03);
	sta(0x00);
	lda_imm(0x3F);
	sta(0x01);
	lda_imm(0xA4);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x6A);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0xEA);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x04);
	sta(0x00);
	lda_imm(0xA7);
	sta(0x01);
	lda_imm(0xA5);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x6A);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0xEA);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x05);
	sta(0x00);
	lda_imm(0x0F);
	sta(0x01);
	lda_imm(0xA7);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x6A);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0xEA);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x77);
	sta(0x01);
	lda_imm(0xA8);
	sta(0x02);
	lda_imm(0x7C);
	sta(0x03);
	lda_imm(0x6F);
	sta(0x04);
	lda_imm(0x1A);
	sta(0x05);
	lda_imm(0x68);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x47);
	sta(0x01);
	lda_imm(0xAB);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x7C);
	sta(0x04);
	lda_imm(0x18);
	sta(0x05);
	lda_imm(0xF0);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x01);
	sta(0x00);
	lda_imm(0xD7);
	sta(0x01);
	lda_imm(0xAC);
	sta(0x02);
	lda_imm(0x00);
	sta(0x03);
	lda_imm(0x7C);
	sta(0x04);
	lda_imm(0x18);
	sta(0x05);
	lda_imm(0xF0);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	lda_imm(0x00);
	sta(0x00);
	lda_imm(0x67);
	sta(0x01);
	lda_imm(0xAE);
	sta(0x02);
	lda_imm(0xE0);
	sta(0x03);
	lda_imm(0x7D);
	sta(0x04);
	lda_imm(0x18);
	sta(0x05);
	lda_imm(0xC0);
	sta(0x06);
	jsr(P_blit_to_screen_5285);
	rts;
  
 

}
end_subroutine

subroutine(P_94B7)
{

	lda_imm(0x14);
	sta(0x94B6);
	lda_imm(0x0A);
	sta(0x94A0);
	lda_imm(0x64);
	sta(0x94A1);
	sta(0x94A2);
	lda_imm(0xAF);
	sta(0x94A3);
	sta(0x94A7);
	lda_imm(0x14);
	sta(0x94A8);
	sta(0x94A9);
	lda_imm(0x46);
	sta(0x94AA);
	sta(0x94AB);
	lda_imm(0x09);
	sta(0x94AC);
	lda_imm(0x05);
	sta(0x94AD);
	lda_imm(0x0A);
	sta(0x94AE);
	sta(0x94AF);
	lda_imm(0x00);
	sta(0x94A4);
	lda_imm(0x4B);
	sta(0x94A5);
	lda_imm(0x1E);
	sta(0x94B0);
	lda_imm(0x0E);
	sta(0x94B1);
	lda_imm(0x08);
	sta(0x94B2);
	lda_imm(0x7D);
	sta(0x94A6);
	lda_imm(0x28);
	sta(0x94B3);
	lda_imm(0x32);
	sta(0x94B5);
	lda_imm(0x14);
	sta(0x94B4);
	lda_imm(0x09);
	sec;
	sbc(0x9619);
	sta(0x00);
	ldx(0x00);
	beq(B_959B);
  
B_952C:
	dec(0x94A8);
	dec(0x94B4);
	dec(0x94B6);
	dec(0x94B3);
	dec(0x94B3);
	dec(0x94B5);
	dec(0x94B5);
	dec(0x94AA);
	dec(0x94AA);
	dec(0x94AA);
	lda(0x94A1);
	sec;
	sbc_imm(0x05);
	sta(0x94A1);
	lda(0x94A2);
	sec;
	sbc_imm(0x05);
	sta(0x94A2);
	lda(0x94A6);
	sec;
	sbc_imm(0x04);
	sta(0x94A6);
	lda(0x94A3);
	sec;
	sbc_imm(0x05);
	sta(0x94A3);
	lda(0x94A4);
	sec;
	sbc_imm(0x0E);
	sta(0x94A4);
	lda(0x94A7);
	sec;
	sbc_imm(0x03);
	sta(0x94A7);
	lda(0x94A5);
	sec;
	sbc_imm(0x03);
	sta(0x94A5);
	lda(0x94AB);
	sec;
	sbc_imm(0x03);
	sta(0x94AB);
	inc(0x94B0);
	inc(0x94B0);
	dex;
	bne(B_952C);
  
B_959B:
	inc(0x00);
	lda(0x00);
	lsr_A();
	tax;
	beq(B_95CA);
  
	cpx_imm(0x05);
	bcc(B_95A9);
  
	ldx_imm(0x04);
B_95A9:
	dec(0x94AC);
	dec(0x94AE);
	dec(0x94AF);
	dec(0x94B1);
	dec(0x94B2);
	lda(0x94A9);
	sec;
	sbc_imm(0x03);
	sta(0x94A9);
	dec(0x94AD);
	dec(0x94A0);
	dex;
	bne(B_95A9);
  
B_95CA:
	ldx(0x9619);
	beq(B_95D8);
  
B_95CF:
	inc(0x94B0);
	inc(0x94B0);
	dex;
	bne(B_95CF);
  
B_95D8:
	rts;
  
 
}
end_subroutine

