#include "oric_basic.h"
#include "oric_sound.h"
#include <stdio.h>
#include <stdlib.h>

extern const T_STRING_PTR data_array[];
extern const int total_nb_data;


static T_STRING str_temp;
static T_INT freeze_state = 0;

typedef struct
   {T_STRING str_KO;
   T_INT HH;
   T_INT E;
   T_INT ZC;  // user input code
   T_INT SALLE;
   T_INT NL;
   T_STRING str_current_condition_group;
   T_NUMERIC PY;
   T_STRING str_X;
   T_INT LI;
   T_NUMERIC A1;
   T_NUMERIC A0;
   T_NUMERIC A3;
   T_NUMERIC A2;
   T_NUMERIC A5;
   T_NUMERIC A4;
   T_NUMERIC A7;
   T_NUMERIC A6;
   T_NUMERIC A9;
   T_NUMERIC A8;
   T_INT PL;
   T_NUMERIC A;
   T_NUMERIC C;
   T_NUMERIC CO;
   T_INT G;
   T_NUMERIC H;
   T_NUMERIC M;
   T_INT nb_objects;
   T_INT N;
   T_NUMERIC ZX;
   T_NUMERIC T;
   T_NUMERIC W;
   T_NUMERIC V;
   T_NUMERIC GN;
   T_STRING str_T;
   T_NUMERIC Z;
   T_NUMERIC GH;
   T_NUMERIC F1;
   T_INT OK;
   T_STRING str_known_keywords;  // all words the user may enter, prefixed by their index
   T_NUMERIC KO;
   T_STRING str_Y;
   T_INT BREAK;
   T_NUMERIC LX;
   
} ScalarGlobals;

ScalarGlobals gs;

#define MARGIN 10
typedef struct
{
   T_STRING str_M[25+MARGIN];
   T_NUMERIC object_counter[12+MARGIN];
   T_STRING str_known_keywords[70+MARGIN];
   T_STRING str_A[128+MARGIN];
   T_STRING str_object_description[25+MARGIN];
   T_INT object_room[25+MARGIN];  // room indexes where the objects are
   T_STRING str_condition_group[14+MARGIN];  
  T_NUMERIC P[12+MARGIN];
  T_STRING str_MO[2+MARGIN];
  T_STRING str_Y[2+MARGIN];
  T_NUMERIC S[1+MARGIN];
   
  T_STRING str_X[4+MARGIN];
} ArrayGlobals;

ArrayGlobals gv;

static void play_sample(const char *sample_name)
{
  if (sound_load("snd", sample_name, "wav", 0)==0)
    {
      sound_play(sample_name);
    }
  else
    {
      fprintf(stderr,"cannot load %s.wav\n",sample_name);
      }
}



void BP_1500();
void BP_1800();
void print_player_state_4000(int s);
void analyse_input();
void load_game_data();
void BP_9000();
void draw_10000_outside_house();
void BP_20100();
T_INT player_holds_object(T_INT object_number);

void draw_10100_hall();
void draw_10200_2nd_floor_stairs();
void draw_10300_dining_room_or_library();
void draw_10500_laundry_room();
void draw_10600_lounge();
void draw_10700_bedroom();
void draw_10800_corridor();
void draw_10900_small_empty_bath_waiting_room();
void draw_11000_lobby_smoke_room();
void draw_11500_laboratory();
void draw_11700_dont_know_where();
void draw_11800_upstairs();
void draw_12200_room();
void draw_12300_storage_room();
void draw_12400_dressing_room();
void draw_grave();
void play_sarabande();
void play_badinerie();
void decode_condition();
void decode_action();
void BP_32000();
void BP_50000();
void disclaimer();
void display_room(T_INT room_number);
   #define clear() memset(&gs,0,sizeof(gs));memset(&gv,0,sizeof(gv))

static const char *gsn = "gamesave";
static T_INT load_game()
{
    FILE *f = fopen(gsn,"rb");
    if (f != 0)
    {
	fread(&gv,sizeof(gv),1,f);
	fread(&gs,sizeof(gs),1,f);
	fclose(f);
    }
    return (f != 0);
}
static T_INT save_game()
{
    FILE *f = fopen(gsn,"wb");
    if (f != 0)
    {
	fwrite(&gv,sizeof(gv),1,f);
	fwrite(&gs,sizeof(gs),1,f);
	fclose(f);
    }
     return (f != 0);
   
}



void basic_entrypoint(int jump)
{
  switch(jump)
    {
      case 45:
	paper(3);
	goto label_game;
    default:
      break;
    }
      clear();
      /* 0 */ cls();print(0);print(chr$(4),chr$(27),"N  LORICIELS EST FIER DE PRESENTER :",chr$(17),PRINT_LINEFEED,0);
      /* 1 */ oric_wait(250);print(chr$(4),chr$(17),PRINT_LINEFEED,0);zap();BP_9000();
      /* 2 */  // COPYRIGHT LORICIELS 83 L.BENES 
      /* 3 */  //  LE MANOIR DU DOCTEUR GENIUS  
      /* 4 */ 
      /* 5 */ for (gs.N=46841;gs.N<=46846;gs.N++)
         {
         poke(gs.N,63);
      }
      
      /* 6 */ poke(gs.N,30);poke(gs.N-7,30);poke(0x26A,10);
      /* 7 */ 
      /* 8 */ text();paper(0);ink(0);cls();
      /* 9 */  // ... COPYRIGHT LORICIELS TOUT      DROIT DE REPRODUCTION INTERDIT ..... 
      /* 10 */ plot(2,0,"_   ___   _   _ ___ _  _ ___ _ ___");
      /* 15 */ plot(2,1,"_   _     __ __ _ _ __ _ _ _ _ _ _");
      /* 16 */ plot(2,2,"_   __    _ _ _ ___ ____ _ _ _ ___");
      /* 17 */ plot(2,3,"_   _     _   _ _ _ _ __ _ _ _ __");
      /* 18 */ plot(2,4,"___ ___   _   _ _ _ _  _ ___ _ _ _");
      /* 19 */ plot(5,7,"__  _ _     __");plot(5,8,"_ _ _ _     _ _ _");plot(5,9,"_ _ _ _");
      /* 20 */ plot(17,9,"_ _ __");plot(5,10,"_ _ _ _     _ _ _ _");plot(5,11,"___ ___");
      /* 22 */ plot(17,11,"___ _");plot(2,15,"____  ____  __  _  _  _  _  ____");
      /* 23 */ plot(2,16,"_  _  _     __  _  _  _  _  _");plot(2,17,"_     _     ___ _  _  _");
      /* 24 */ plot(27,17,"_  _");plot(2,18,"_     ___   _ _ _  _  _  _  ____");
      /* 25 */ plot(2,19,"_ __  _     _ ___  _  _  _     _");plot(2,20,"_  _  _     _  __");
      /* 26 */ plot(21,20,"_  _  _     _");plot(2,21,"____  ____  _  __  _  ____  ____ _ _");
      /* 27 */ for (gs.N=0;gs.N<=4;gs.N++)
         {
         plot_char(1,gs.N,5);
      }
      
      /* 28 */ for (gs.N=7;gs.N<=11;gs.N++)
         {
         plot_char(1,gs.N,1);
      }
      
      /* 29 */ for (gs.N=15;gs.N<=21;gs.N++)
         {
         plot_char(1,gs.N,12);
      }
      
  /* 30 */ ink(3);oric_wait(90);ink(4);oric_wait(90);ink(3);oric_wait(90);ink(4);oric_wait(99);ink(3);
  play_badinerie();
  disclaimer();
      /* 31 */ plot(5,25,"Appuyez sur une touche");
      /* 32 */ do{
    gs.ZX = rnd(1);
    oric_wait(10); // cpu!!
  } while(!(strneq(key$(),"")));
  
  label_40000:
      cls();print(0);print(0);print(0);print(0);
      /* 40010 */ print("   La liste des instructions  (O/N) ?",PRINT_LINEFEED,0);
      /* 40015 */ 

  
  while(1)
    {
      get(gs.str_known_keywords);
      /* 40020 */ if (streq(gs.str_known_keywords,"O"))
         {
         cls();print(0);print("Vous voici arrive dans le manoir du             Dr Genius...",PRINT_LINEFEED,0);
      /* 34 */ oric_wait(350);print(0);print("Pour converser avec l'ordinateur,il   faut rentrer les ordres",PRINT_LINEFEED,0);
      /* 35 */ oric_wait(300);print(0);print("en 1 ou 2 mots tels que:",PRINT_LINEFEED,0);print(0);oric_wait(200);print("     NORD",PRINT_LINEFEED,0);
      /* 36 */ oric_wait(200);print(0);print("     PRENDS PILULE",PRINT_LINEFEED,0);
      /* 37 */ oric_wait(200);print(0);print("ou pour commencer:",0);oric_wait(150);print("ENTRE",PRINT_LINEFEED,0);
      /* 38 */ oric_wait(300);print(0);print(0);print("Si vous voulez faire durer la phrase  decrivant",0);
      /* 39 */ print(" la salle taper une touche",PRINT_LINEFEED,0);oric_wait(400);
	  /* 40 */ print(0);print("  SAUVE:  sauve la partie",PRINT_LINEFEED,0);print("  CHARGE: charge une partie sauvee",PRINT_LINEFEED,0);
	  print(0);print("        pressez une touche",PRINT_LINEFEED,0);get(gs.str_X);
      /* 41 */ cls();print(0);print("  Un dernier conseil:",0);oric_wait(200);print("Il peut parfois",PRINT_LINEFEED,0);print(0);
      /* 42 */ print("y avoir une porte derriere vous.",PRINT_LINEFEED,0);oric_wait(400);
	  /* 45 */ 
	  break;
      }
      /* 40030 */ if (streq(gs.str_known_keywords,"N"))
         {
         break;
	}
    }
  
  
 
      
  label_game:
  unload_sounds();
  
  cls();hires();print(0);
  print("       VEUILLEZ PATIENTER",PRINT_LINEFEED,0); // useless: too fast now :)
  /* 50 */ 
  load_game_data(); //CHARGEMENT VARIABLES 
  /* 55 */ 
  gv.P[10] = 0;
  gv.P[11] = 0;
  /* 60 */ gs.SALLE = 1; // outside
  /* 70 */ for (gs.N=1;gs.N<=10;gs.N++)
    {
         
      /* 80 */ gv.P[gs.N-1] = 0;gv.object_counter[gs.N-1] = 0;
      /* 85 */ 
    }
      
  /* 90 */ gv.object_counter[2] = 14;
  gv.object_counter[6] = 12;
  gv.object_counter[0] = 80;
  gv.object_counter[8] = 12;
  /* 100 */ 
  label_mainloop:
  if (!freeze_state)
    {

      if (gs.SALLE!=14 && gs.SALLE!=16 && gs.SALLE!=17 && gs.SALLE!=19) // originally dark rooms
         {
         gv.P[1] = 0;
      }
      /* 105 */ if (gv.P[1] == 0)
         {
         goto label_200;
      }
      /* 106 */ if (gv.object_room[21] == gs.SALLE && gv.P[6] == 1)
         {
         goto label_200;
      }
      /* 107 */ if (gv.object_room[4] == gs.SALLE && gv.P[2] == 1)
         {
         goto label_200;
      }
      /* 110 */ if (gv.object_counter[8]>1)
         {
         gv.object_counter[8]--;  // decrease lamp timeout: after a while you trip and die
	}
    }
  
      /* 130 */ hires();
      /* 140 */ print("Il fait noir comme dans un four,il    faudrait peut etre allumer",PRINT_LINEFEED,0);
      /* 145 */ if (strneq(key$(),""))
         {
         oric_wait(500);
      }
      else
         {
         oric_wait(300);
      }
      /* 150 */ goto label_500_player_input;
      /* 200 */ 
label_200:
       //   DESCRIPTION SALLE 
      /* 210 */ print(0);
  /* 220 */ display_room((7000+gs.SALLE*10)-10);
  label_300:
      /* 300 */ gs.H = 0;gs.N = 1;
      /* 310 */ 
  label_310:
  
      if (gv.object_room[gs.N-1]==gs.SALLE)
         {
         
      /* 320 */ if (gs.H != 1)
	{
         
	  /* 330 */ print("Il y a dans la salle:",PRINT_LINEFEED,0);oric_wait(100);
	  /* 340 */ gs.H = 1;
	  /* 350 */ 
	}

      print(" ",gv.str_object_description[gs.N-1],0);
      /* 360 */ oric_wait(150);
      /* 400 */ 
    }
  
label_400:
      gs.N++;
      /* 410 */ if (gs.N<= gs.nb_objects)
         {
         goto label_310;
      }
      /* 420 */ print(0);
      /* 500 */ 
label_500_player_input:
       // ACCEPTATION COMMANDE 
      /* 510 */ gs.T = 1;strcpy(gv.str_Y[0],"");strcpy(gv.str_Y[1],"");gs.N = 0;
  /* 520 */ goto label_1000;
  label_530:
  if (!freeze_state)
    {
      /* 530 */ if (gv.object_counter[6]>1 && gv.P[6] == 1)
         {
         gv.object_counter[6]--;
      }
      /* 540 */ if (gv.object_counter[2]>1 && gv.P[2] == 1)
         {
         gv.object_counter[2]--;
      }
      /* 545 */ if (gv.object_counter[3]>1 && gv.P[3] == 1)
         {
         gv.object_counter[3]--;
      }
      /* 547 */ if (gv.object_counter[4]>1)
         {
      gv.object_counter[4]--;
	}
    }
  
  label_550:
  freeze_state = 0;
  /* 550 */ input("Que faites vous",gs.str_X,0);
  /* 560 */ cls();
  if (strncmp("SAUV",gs.str_X,4)==0)
    {
      if (save_game())
	{
	  print("Sauvegarde OK",PRINT_LINEFEED,0);
	}
      else
	{
	  print("Echec sauvegarde !",PRINT_LINEFEED,0);
	}
      oric_wait(200);goto label_550;
    }
  else if (strncmp("CHAR",gs.str_X,4)==0)
    {
      if (load_game())
	{
	  print("Chargement OK",PRINT_LINEFEED,0);
	  oric_wait(200);goto label_mainloop;
	}
      else
	{
	  print("Echec chargement",PRINT_LINEFEED,0);
	  oric_wait(200);goto label_550;
	}
      
    }
  
  /* 570 */ analyse_input();
  /* 580 */ if (streq(gv.str_MO[0],"00"))
    {
         print("Je ne comprends pas...",PRINT_LINEFEED,0);oric_wait(200);goto label_mainloop;
      }
      /* 900 */  //  CONTROLE Mvt 
      /* 910 */ gs.Z = 1;
      /* 920 */ 
label_920:
      strcpy(gs.str_T,mid$(gv.str_M[(T_INT)gs.SALLE-1],gs.Z,2));
      /* 930 */ if (streq(gs.str_T,"00"))
         {
         goto label_980;
      }
      /* 940 */ if (strneq(gs.str_T,gv.str_MO[0]))
         {
         goto label_970;
      }
      /* 950 */ gs.SALLE = val(mid$(gv.str_M[(T_INT)gs.SALLE-1],gs.Z+2,2));
      /* 960 */ goto label_mainloop;
      /* 970 */ 
label_970:
      gs.Z = gs.Z+4;goto label_920;
      /* 980 */ 
label_980:
      gs.T = 0;
      /* 990 */ gs.A1 = 0;
      /* 1000 */ 
label_1000:
       //  CONTROLE  
      /* 1010 */ gs.NL = 0;
      /* 1100 */ 
label_1100:
      gs.NL++;
      /* 1110 */ if (gs.T == 0)
         {
         goto label_1150;
      }
      /* 1120 */ strcpy(gs.str_current_condition_group,gv.str_condition_group[gs.NL-1]);  // load current condition
      /* 1130 */ goto label_1400;
      /* 1150 */ 
label_1150:
      if (gs.NL<= gs.A)
         {
         goto label_1200;
      }
      /* 1159 */ gs.PY = 23;gs.CO = 12;
      /* 1160 */ if (gs.A1 == 1)
         {
         goto label_500_player_input;
      }
      /* 1170 */ print("Impossible ",0);
      /* 1180 */ if (val(gv.str_MO[0])<10)
         {
         print("de prendre ce chemin",0);
      }
      /* 1190 */ print("!",PRINT_LINEFEED,0);oric_wait(100);goto label_mainloop;
      /* 1200 */ 
label_1200:
      if (strneq(mid$(gv.str_A[gs.NL-1],1,2),gv.str_MO[0]))
         {
         goto label_1100;
      }
      /* 1210 */ strcpy(gs.str_Y,mid$(gv.str_A[gs.NL-1],3,2));
      /* 1220 */ if (strneq(gs.str_Y,"00")&strneq(gs.str_Y,gv.str_MO[1]))
         {
         goto label_1100;
      }
      /* 1230 */ strcpy(gs.str_current_condition_group,mid2$(gv.str_A[gs.NL-1],5));
      /* 1400 */ 
label_1400:
  //  CONDITIONS (all conditions of the group must be true)
  /* 1410 */ gs.E = 1;
  /* 1420 */ 
  while (strneq(mid$(gs.str_current_condition_group,gs.E,1),".")) // . : end of conditions
    {      
      /* 1430 */ gs.LI = asc(mid$(gs.str_current_condition_group,gs.E,1))-65; // condition number
      /* 1440 */ gs.N = val(mid$(gs.str_current_condition_group,gs.E+1,2)); // on object index
    
      decode_condition();
  
      /* 1460 */ if (gs.OK == 0)
	{
	  goto label_1100; // condition failed, next condition group
	}
      /* 1470 */ gs.E += 3; // next condition
      
      /* 1500 */ 
    }
  
      /* 1700 */ 
       //    decode ACTIONS 
      /* 1705 */ gs.E = gs.E+1;gs.A1 = 1;
      /* 1710 */ 

  while (strneq(mid$(gs.str_current_condition_group,gs.E,1),"."))
    {
      
      /* 1720 */ gs.LI = asc(mid$(gs.str_current_condition_group,gs.E,1))-65;
      /* 1730 */ if (strneq(mid$(gs.str_current_condition_group,gs.E+1,1),"."))
	{
	  gs.N = val(mid$(gs.str_current_condition_group,gs.E+1,2));
	}
      //printf("decode action %s %d %d %d\n",gs.str_current_condition_group,gs.E,gs.LI,gs.N);
      decode_action();
  
      /* 1760 */ switch (gs.BREAK)
	{
	  case 0:
	    break;
	  case 100:
	    goto label_mainloop;
	  case 300:
	    goto label_300;
	  case 500:
	    goto label_500_player_input;
	  case 530:
	    goto label_530;
	  case 1000:
	    goto label_death;
	
	  default:
		  
	    {
	      error(str$(__LINE__),": unsupported BREAK value ",str$(gs.BREAK),0);
	    }
	}
  
      /* 1780 */ gs.E += 3; // next action
      /* 1790 */
    }

        // no associated actions: next condition group / action group
         goto label_1100;

  label_death:
      draw_grave(); //TOMBE 
      /* 20010 */ play_sarabande(); //SARABANDE  
  /* 20100 */ 
  BP_20100();
}

void BP_20100()
{
  while(1)
    {
  text();print(0);print(0);print(0);print(0);print("Voulez-vous rejouer ?",PRINT_LINEFEED,0);
      get(gs.str_X);

      /* 20105 */ if (streq(gs.str_X,"O"))
         {
         clear();basic_entrypoint(45); // RUN 45
      }
      /* 20110 */ if (streq(gs.str_X,"N"))
         {
         break;
      }
      
    }
  

  cls();print(0);print(0);print(0);print("     AU REVOIR",PRINT_LINEFEED,0);
  oric_wait(500);
      /* 20210 */ end();
      /* 30000 */ 
     
}
   
void print_player_state_4000(int s)
{
      
  switch(s)
    {
      case 0: // bug: impossible to reach
	hires();for (gs.N=1;gs.N<=20;gs.N++)
         {
         explode();oric_wait(4);
      }
      /* 4001 */ print("Vous avez garde la lampe trop         longtemps allumee,elle a explose",PRINT_LINEFEED,0);
	/* 4005 */ oric_wait(400);return;
	case 1:
	/* 4010 */ hires();
	play_sample("running_water");
      /* 4011 */ print("Vous avez oublie de fermer le robinet vous mourez sous des tonnes d'eau",PRINT_LINEFEED,0);
	/* 4015 */ oric_wait(500);return;
	case 2:
	/* 4020 */ hires();
	play_sample("door_slam");
      /* 4022 */ print("La porte vient de se refermer derrierevous,vous voila prisonnier...",PRINT_LINEFEED,0);
	/* 4025 */ oric_wait(500);return;
	case 3:
	/* 4030 */ hires();
	play_sample("stab");
      /* 4031 */ print("Vous avez trebuche dans l'escalier,vous vous empalez sur le couteau!",PRINT_LINEFEED,0);
	/* 4035 */ oric_wait(500);return;
	case 4:
      /* 4040 */ hires();
      /* 4041 */ print("Vous renversez l'eau dans l'escalier, ce qui provoque une decharge",0);
      /* 4042 */ print(" de la",PRINT_LINEFEED,0);oric_wait(300);print("prise electrique",PRINT_LINEFEED,0);
	/* 4045 */ oric_wait(300);return;
	case 5:
      /* 4050 */ hires();
      /* 4051 */ print("Vous etes sauf grace a la combinaison que vous avez enfile..!",PRINT_LINEFEED,0);
	/* 4055 */ oric_wait(500);return;
      case 6:
	play_sample("fizzle");
      /* 4060 */ print("Vous mourez electrocute...",PRINT_LINEFEED,0);
	/* 4065 */ oric_wait(300);return;
	case 7:
      /* 4070 */ hires();play_sample("rpg");
      /* 4071 */ print("La piece etait pleine de gaz explosif,vous auriez du eteindre...",PRINT_LINEFEED,0);
      /* 4072 */ oric_wait(500);cls();print("On ramassera vos morceaux un          autre jour..!",PRINT_LINEFEED,0);
	/* 4075 */ oric_wait(300);return;
	case 8:
	/* 4080 */ hires();

	play_sample("spike");
      /* 4081 */ print("Vous mourez empale sur des lances    sorties du mur...!",PRINT_LINEFEED,0);
	/* 4085 */ oric_wait(400);return;
	case 9:
      /* 4090 */ 
	/* 4091 */ print("La porte de cette piece ne s'ouvre pas",PRINT_LINEFEED,0);oric_wait(300);return;
	case 10:
      /* 4100 */ hires();
      /* 4101 */ print("La lampe et le briquet refusent de    marcher dans cette piece",PRINT_LINEFEED,0);
	/* 4105 */ oric_wait(400);return;
	case 11:
	/* 4110 */ hires();
	play_sample("fall_and_splat");
      /* 4111 */ print("Vous tombez dans une trappe,vous vous disloquez en arrivant au sol..",PRINT_LINEFEED,0);
	/* 4115 */ oric_wait(500);return;
	case 12:
      /* 4120 */  //   
      /* 4121 */ print("Vous avez raison de passer,car ce     monstre n'etait qu'une projection",PRINT_LINEFEED,0);
      /* 4124 */ oric_wait(400);print("en 3 dimensions sur un ecran de fumee",PRINT_LINEFEED,0);
	/* 4125 */ oric_wait(250);return;
	case 13:
      /* 4130 */  //   
      /* 4131 */ print("Vous avez raison,la curiosite est un  vilain defaut!!!",PRINT_LINEFEED,0);
      /* 4132 */ oric_wait(400);
      /* 4133 */ text();print(0);print(0);print(spc(12),"AU REVOIR",PRINT_LINEFEED,0);
	/* 4135 */ oric_wait(200);BP_20100();return;
	case 14:
      /* 4140 */ print("Vous avez raison d'attendre,mais cela ne pourra pas durer",0);
      /* 4141 */ print(" eternellement..",PRINT_LINEFEED,0);
	/* 4142 */ oric_wait(450);return;
	case 15:
      /* 4150 */ hires();print("Vous avez de la chance car ce coffre  etait ouvert.",PRINT_LINEFEED,0);oric_wait(400);
      /* 4152 */ print("Un message a l'interieur dit:",PRINT_LINEFEED,0);oric_wait(250);print("Ne respectez pas les ",0);
      /* 4154 */ print("couleurs du",PRINT_LINEFEED,0);print("code de la route...?",PRINT_LINEFEED,0);oric_wait(500);
	/* 4156 */ print("Tiens le coffre se referme",PRINT_LINEFEED,0);oric_wait(200);return;
	case 16:
      /* 4160 */ print("Maintenant,vous avez une lampe pleine de petrole",PRINT_LINEFEED,0);
	/* 4165 */ oric_wait(400);return;
	case 17:
	/* 4170 */ print("Vous n'avez rien pour transporter le  petrole",PRINT_LINEFEED,0);oric_wait(400);
	return;
	case 18:
      /* 4180 */ hires();print("Le briquet que vous aviez laisse      allume vient d'exploser",PRINT_LINEFEED,0);
      /* 4181 */ for (gs.N=1;gs.N<=10;gs.N++)
         {
         explode();oric_wait(6);
      }
      
	/* 4185 */ oric_wait(300);print("Ca tue l'etourderie.....",PRINT_LINEFEED,0);oric_wait(200);return;
	case 19:
      /* 4190 */ hires();print("A force de marcher en long et en      large dans cette maison,",PRINT_LINEFEED,0);
	/* 4195 */ oric_wait(300);print("vous sombrez dans un coma des plus    mortels...",PRINT_LINEFEED,0);oric_wait(300);return;
	case 20:
	/* 4200 */ print("L'eau coule...",PRINT_LINEFEED,0);return;
	case 21:
	/* 4210 */ hires();print("Vous avez les pieds trempes,et cela   vous rend tres malade...",PRINT_LINEFEED,0);
	play_sample("gagging");
	/* 4215 */ oric_wait(400);print("Vous mourez d'une triple pneumonie...!",PRINT_LINEFEED,0);oric_wait(300);return;
	case 22:
	/* 4220 */ print("Le titre est:",PRINT_LINEFEED,0);oric_wait(200);print("La mort a la premiere page.",PRINT_LINEFEED,0);oric_wait(300);return;
	case 23:
      // death
      play_sample("explosion");
	/* 4233 */ print("Le livre a explose lorsque vous l'avez ouvert...",PRINT_LINEFEED,0);oric_wait(400);return;
	case 24:
	/* 4240 */ print("Le papier indique:  Cherchez la clef.",PRINT_LINEFEED,0);oric_wait(300);return;
	case 25:
      /* 4250 */ print("La clef vous permettera de trouver le code de la porte d'entree.",PRINT_LINEFEED,0);
	/* 4255 */ oric_wait(400);return;
	case 26:
      /* 4260 */ hires();print("Il y a , a cote de la porte,un clavier numerique permettant ",0);
	/* 4265 */ print("d'entrer un code",PRINT_LINEFEED,0);oric_wait(400);return;
	case 27:
	/* 4270 */ print("Pour faire quoi..?",PRINT_LINEFEED,0);oric_wait(200);return;
	case 28:
	/* 4280 */ print("Il y a une odeur de gaz.",PRINT_LINEFEED,0);oric_wait(300);return;
	case 29:
	/* 4290 */ print("Apparement,il n'y a aucune odeur      mais...",PRINT_LINEFEED,0);oric_wait(300);return;
	case 30:
	/* 4300 */ print("C'est deja fait,espece de rigolo",PRINT_LINEFEED,0);oric_wait(300);return;
	case 31:
	/* 4310 */ print("Il faudrait peut etre du feu",PRINT_LINEFEED,0);oric_wait(300);return;
	case 32:
	/* 4320 */ print("La lampe ne contient pas de petrole",PRINT_LINEFEED,0);oric_wait(300);return;
	case 33:
	/* 4330 */ print("Vous ne l'avez pas",PRINT_LINEFEED,0);oric_wait(200);return;
	case 34:
	/* 4340 */ print("Le briquet est encore allume et il    eclaire la piece.",PRINT_LINEFEED,0);oric_wait(300);return;
	case 35:
	play_sample("molotov_cocktail");
      
      print("La torche etait piegee,elle vous      explose dans les mains..",PRINT_LINEFEED,0);
      /* 4357 */ oric_wait(400);return;
	case 36:
      /* 4360 */ print("La lampe est encore allumee,et elle   vous eclaire",PRINT_LINEFEED,0);oric_wait(300);return;
      case 37:
	// death
	play_sample("knife_launch");
      /* 4370 */ hires();print("Un nain vient de vous lancer un       poignard en plein coeur..",PRINT_LINEFEED,0);
      /* 4375 */ oric_wait(300);return;
      case 38:
	play_sample("scream_and_die");

      /* 4380 */ print("Un nain vient de se precipiter sur    vous,il s'empale sur votre",0);
      /* 4385 */ print(" ciseau",PRINT_LINEFEED,0);oric_wait(400);return;
	case 39:
	play_sample("scream_and_die");
      /* 4390 */ print("Un nain vient de se precipiter sur    vous,il s'empale sur votre",0);
      /* 4395 */ print(" couteau",PRINT_LINEFEED,0);oric_wait(400);return;
	case 40:
      /* 4400 */ hires();print("Vous venez de renverser le pot",PRINT_LINEFEED,0);oric_wait(150);return;
      case 41:
	play_sample("thunder");
      /* 4410 */ hires();print("La foudre vient de tomber sur la      maison",PRINT_LINEFEED,0);oric_wait(200);
      /* 4412 */ print("La maison n'existe plus,vous non plus",PRINT_LINEFEED,0);oric_wait(200);return;
	case 42:
      /* 4420 */ hires();print("A force de marcher dans le noir,vous  avez trebuche",PRINT_LINEFEED,0);oric_wait(200);
      /* 4425 */ print("Vous mourez d'une fracture du crane...",PRINT_LINEFEED,0);oric_wait(200);return;
	case 43:
      /* 4430 */ print("Vous ne pouvez pas travailler dans le noir...",PRINT_LINEFEED,0);oric_wait(300);return;
	case 44:
      /* 4440 */ print("La lumiere du briquet ne suffit pas   pour travailler...",PRINT_LINEFEED,0);oric_wait(400);return;
	case 45:
      /* 4450 */ print("Impossible !",PRINT_LINEFEED,0);oric_wait(100);return;
	case 46:
      /* 4460 */ print("Vous n'avez aucun outil..",PRINT_LINEFEED,0);oric_wait(250);return;
	case 47:
      /* 4470 */ print("Le teleporteur est en panne,donc les  boutons ne fonctionnent pas.",PRINT_LINEFEED,0);
      /* 4475 */ oric_wait(400);return;
 	case 48:
     play_sample("explosion");
      
      /* 4484 */ print("Le teleporteur vient d'exploser,vous  etes decompose..!",PRINT_LINEFEED,0);oric_wait(400);return;
 	case 49:
      /* 4491 */ play_sample("teleport");
     /* 4490 */ print("Le teleporteur se met en marche,vous  disparaissez",PRINT_LINEFEED,0);
      /* 4492 */ for (gs.N=1;gs.N<=12;gs.N++)
         {
         
         /* 4493 */ paper(4);oric_wait(8);paper(1);oric_wait(8);paper(3);oric_wait(8);paper(4);oric_wait(8);paper(1);oric_wait(8);
         /* 4494 */ 
      }
      paper(0);play(0,0,0,0);
      /* 4495 */ for (gs.N=500;(500<30)?(gs.N<=30):(gs.N>=30);gs.N+=-5)
         {
         
         /* 4497 */ sound(2,gs.N,12);play(2,0,0,100);
      }
      
      /* 4498 */ play(0,0,0,0);hires();return;
  	case 50:
	/* 4500 */ hires();
	play_sample("fuse_burning");
	print("Vous prenez du 30000 Volts dans les   doigts,",PRINT_LINEFEED,0);oric_wait(300);return;
  	case 51:
    /* 4510 */ print("Le placard est ferme a clef",PRINT_LINEFEED,0);oric_wait(150);return;
   	case 52:
	/* 4520 */ hires();
	play_sample("zombie");
	print("L'horrible monstre sorti du placard   vient de vous devorer",PRINT_LINEFEED,0);
      /* 4525 */ oric_wait(400);return;
  	case 53:
    /* 4530 */ print("Il ne fallait pas fuir",PRINT_LINEFEED,0);oric_wait(200);return;
   	case 54:
      /* 4540 */ hires();print("Vous avez raison d'utiliser le ciseau,le monstre est mort",PRINT_LINEFEED,0);
   /* 4545 */ oric_wait(400);return;
   	case 55:
   /* 4550 */ print("A l'interieur du placard,le No ",str$(gs.PL),"   est inscrit",PRINT_LINEFEED,0);oric_wait(300);
      /* 4555 */ print("Le placard se referme.",PRINT_LINEFEED,0);oric_wait(150);return;
   	case 56:
   /* 4560 */ play_sample("explosion");
      print("le pistolet a explose",PRINT_LINEFEED,0);oric_wait(200);
      /* 4565 */ return;
 	case 57:
     /* 4570 */ 
label_4570:
      hires();play_sample("explosion");
      /* 4575 */ print("le clavier numerique a explose",PRINT_LINEFEED,0);oric_wait(250);return;
   	case 58:
   /* 4580 */ hires();print("Le clavier numerique prend feu,      heureusement,vous aviez ",0);
      /* 4582 */ oric_wait(300);print("un pot plein",PRINT_LINEFEED,0);oric_wait(100);
      /* 4585 */ print("d'eau qui vous a permis d'eteindre ce feu",PRINT_LINEFEED,0);
      /* 4586 */ oric_wait(400);return;
  	case 59:
    /* 4590 */ hires();input("No DE CODE",str_temp,0);
      gs.ZC=atoi(str_temp);
      /* 4595 */ if (gs.ZC!=gs.PL)
         {
         goto label_4570;
      }
   	case 60:
   /* 4600 */ oric_wait(200);print("Le code est exact...",PRINT_LINEFEED,0);print("La porte s'ouvre...",PRINT_LINEFEED,0);
      /* 4603 */ oric_wait(400);
      /* 4605 */ draw_10000_outside_house();print("Vous voila en dehors de la maison...",PRINT_LINEFEED,0);
	/* 4608 */ BP_32000();
	  BP_20100();

   	case 61:
   /* 4610 */ hires();print("A l'interieur du placard,il y a un motqui parle d'un teleporteur",PRINT_LINEFEED,0);
      /* 4615 */ oric_wait(400);print("Tiens le placard se ferme tout seul...",PRINT_LINEFEED,0);oric_wait(150);return;
   	case 62:
   /* 4620 */ print("Avant de la poser par terre,il        faudrait peut etre l'enlever.",PRINT_LINEFEED,0);
      /* 4625 */ oric_wait(350);return;
  	case 63:
    /* 4630 */ hires();print("Il y a un horrible monstre devant vousqui est sortie du placard.",PRINT_LINEFEED,0);
      /* 4635 */ oric_wait(400);return;
      case 64:
	play_sample("rpg");
   /* 4640 */ hires();print("Le placard etait piege,vous n'auriez  pas du l'ouvrir",PRINT_LINEFEED,0);
      /* 4645 */ for (gs.N=1;gs.N<=30;gs.N++)
         {
         explode();oric_wait(7);
      }
      
	/* 4647 */ oric_wait(150);return;
      default:
	error(str$(__LINE__),": unsupported s value ",str$(s),0);
    }
  
      /* 6000 */ 
}

void analyse_input()
{
       //    ANALYSE DU MOT 
      /* 6010 */ gs.N = 0;gs.GN = 0;
  /* 6020 */ 
  do
    {
         gs.N = gs.N+1;
         /* 6030 */ 
    } 
    while(!(strneq(mid$(gs.str_X,gs.N,1)," ")));
      /* 6040 */ do{
         gs.GN = gs.GN+1;
         /* 6050 */ 
      } while(!(streq(mid$(gs.str_X,gs.N+gs.GN,1)," ")||streq(mid$(gs.str_X,gs.N+gs.GN,1),"")||gs.GN == 20));
      /* 6060 */ if (gs.GN>4)
         {
         gs.GN = 4;
      }
  /* 6070 */ strcpy(gv.str_X[0],mid$(gs.str_X,gs.N,gs.GN));
  /* 6080 */ do{
    gs.N = gs.N+1;
    /* 6090 */ 
  } while(!(streq(mid$(gs.str_X,gs.N,1)," ")||streq(mid$(gs.str_X,gs.N,1),"")));
  /* 6100 */ do{
    gs.N = gs.N+1;
    /* 6110 */ 
  } while(!(strneq(mid$(gs.str_X,gs.N,1)," ")));
  /* 6115 */ gs.GN = 0;
  /* 6120 */ do
    {
    gs.GN = gs.GN+1;
    /* 6130 */ 
  } while(!(streq(mid$(gs.str_X,gs.N+gs.GN,1)," ")||streq(mid$(gs.str_X,gs.N+gs.GN,1),"")||gs.GN == 20));
  /* 6140 */ if (gs.GN>4)
    {
      gs.GN = 4;
    }
  /* 6150 */ strcpy(gv.str_X[1],mid$(gs.str_X,gs.N,gs.GN));
  /* 6160 */ for (gs.W=1;gs.W<=2;gs.W++)
    {
      gs.N = 0;
      strcpy(gv.str_MO[(T_INT)gs.W-1],"00");
      /* 6170 */ 
      label_6170:
      gs.N = gs.N+1;
      /* 6180 */ if (gs.N>gs.V)
	{
	  gs.W = 3;continue;
	}
      /* 6190 */ if (streq(mid$(gv.str_known_keywords[gs.N-1],3,4),gv.str_X[(T_INT)gs.W-1]))
	{
	  goto label_6250;
	}
      /* 6200 */ goto label_6170;
      /* 6250 */ 
      label_6250:
      strcpy(gv.str_MO[(T_INT)gs.W-1],mid$(gv.str_known_keywords[gs.N-1],1,2));
      /* 6300 */ 

         
    }
      
  /* 6310 */ return;
}
static void wait_msg()
{
  /* 7007 */ if (strneq(key$(),""))
    {
      oric_wait(400);
    }
  else
    {
      oric_wait(100);
    }
}

T_INT object_in_this_room(T_INT object_number)
{
  return gv.object_room[object_number] == gs.SALLE;
}

T_INT player_holds_object(T_INT object_number)
{
 return  gv.object_room[object_number]==-1;
}

void decode_action()
{
    /* 1740 */ gs.BREAK = 0;
  switch(gs.LI)
    {
      case 0:
      // inventory
      gs.G = 0;gs.HH = 0;
      /* 1810 */ 
label_1810:
      gs.G++;
      /* 1820 */ if (player_holds_object(gs.G-1)) 
         {
         goto label_1840;
      }
      /* 1830 */ if (gs.G<gs.nb_objects)
         {
         goto label_1810;
      }
      else
         {
         goto label_1870; // no more objects to scan
      }
      /* 1840 */ 
label_1840:
      if (gs.HH == 0)
         {
         print("Vous detenez:",PRINT_LINEFEED,0);oric_wait(100);
      }
      /* 1850 */ gs.HH = 1;
      /* 1860 */ print(gv.str_object_description[gs.G-1],0);print(" ",0);oric_wait(150);
      /* 1865 */ if (gs.G<gs.V)
         {
         goto label_1810;
      }
      /* 1870 */ 
label_1870:
      if (gs.HH == 1)
         {
         print(".",PRINT_LINEFEED,0);return;
      }
	/* 1880 */ print("Vous ne detenez absolument rien!!!",PRINT_LINEFEED,0);oric_wait(200);
	break;
	case 1:
      /* 1900 */ if (gv.S[0]<5)
         {
         goto label_1930;
      }
      /* 1910 */ print("Il parait evident que vous ne pouvez  porter tant de choses!!",PRINT_LINEFEED,0);
      /* 1920 */ 
label_1920:
	oric_wait(250);gs.BREAK = 100;
	break;
      /* 1930 */ 
label_1930:
      if (gv.object_room[gs.N-1]!=-1)
         {
         goto label_1960;
      }
      /* 1935 */  //                  CE PROGRAMME EST LA PROPRIETE DE L.WEILL LORICIELS 
      /* 1940 */ print("Vous l'avez deja,vous etes etourdi et dans cette maison ce n'est pas",PRINT_LINEFEED,0);
      /* 1945 */ oric_wait(400);
      /* 1950 */ print("tres conseille...",PRINT_LINEFEED,0);goto label_1920;
      /* 1960 */ 
label_1960:
	gv.object_room[gs.N-1] = -1;gv.S[0]+=1;
	break;	
	case 2:
      /* 2000 */ if (gv.object_room[gs.N-1] != -1)
         {
         
      /* 2010 */ print("Comment voulez vous poser ce que vous n'avez pas",PRINT_LINEFEED,0);
      /* 2020 */ goto label_1920;
	    /* 2030 */ 
	    }

	gv.object_room[gs.N-1] = gs.SALLE;gv.S[0] = gv.S[0]-1;
	break;
	case 3:
	/* 2100 */ cls();
	print_player_state_4000(gs.N);
	/* 2110 */ break;
	case 4:
	/* 2200 */ gv.P[gs.N-1] = 1;
	break;
	case 5:
	/* 2300 */ gv.P[gs.N-1] = 0;
	break;
	case 6:
	/* 2400 */ gv.object_counter[gs.N-1] = val(mid$(gv.str_A[gs.N-1],gs.E+3,2));gs.E += 2;
	break;
	case 7:
      /* 2500 */ if (player_holds_object(gs.N-1))
         {
         gv.S[0] = gv.S[0]-1;
      }
	/* 2510 */ gv.object_room[gs.N-1] = 0;
	break;
	case 8:
	/* 2600 */ gs.SALLE = gs.N;
	break;
	case 9:
	/* 2700 */ print("D'accord...",PRINT_LINEFEED,0);

	/* 2710 */ oric_wait(150);gs.BREAK = 300;
	break;
	case 10:
      /* 2800 */ gs.BREAK = 500;
	/* 2810 */ 
	break;
	case 11:
	/* 2900 */ gs.BREAK = 530;
	break;
	case 12:
	/* 3000 */ gs.BREAK = 100;
	break;
	case 13:
	/* 3100 */ gs.BREAK = 1000; // death
	break;
	case 14:
	/* 3200 */ gv.object_room[gs.N-1] = gs.SALLE;
	break;
	case 15:
	/* 3300 */ strcpy(gs.str_X,gv.str_object_description[gs.N-1]);
	strcpy(gv.str_object_description[gs.N-1],gv.str_object_description[gs.N]);
	strcpy(gv.str_object_description[gs.N-1],gs.str_X)
	;break;
      default:
	error(str$(__LINE__),": unsupported LI value ",str$(gs.LI),0);
	break;
    }
}

void condition_matches(T_INT cond,const char *description)
{
  if (cond)
    {
      gs.OK = 1;
      (void)description;
      //printf("OK: %s: %d (object: %s)\n",description,gs.N-1,gv.str_object_description[gs.N-1]);
    }
  else
    {
      gs.OK = 0;
    }
}

void decode_condition()
{
    switch(gs.LI)
    {
      case 0: // A
     condition_matches (gs.N == gs.SALLE,"room matches");
	break;
      case 1: // B
	/* 1510 */
	condition_matches (player_holds_object(gs.N-1) || object_in_this_room(gs.N-1),"object held or in room");
         
	    break;
      case 2: // C
	/* 1520 */
	condition_matches (!player_holds_object(gs.N-1) && !object_in_this_room(gs.N-1),"object neither held nor in room");
         
	/* 1525 */ break;
	case 3:  // D
      /* 1530 */ condition_matches (player_holds_object(gs.N-1),"object held");
       
	/* 1535 */ break;
	case 4: // E
	/* 1540 */ 
	condition_matches (gv.P[gs.N-1] == 1,"??? 4");
         
	/* 1545 */ break;
	case 5: // F
      /* 1550 */ condition_matches (gv.P[gs.N-1] == 0,"??? 5");
         
	/* 1555 */ break;
	case 6: // G
	/* 1560 */ 
	condition_matches (gv.object_counter[gs.N-1] == 1,"object timeout reached");
	/* 1565 */ break;
      case 7: // H
	{
	  /* 1570 */ 
	  T_INT r = integer(rnd(1)*200); // not same number generator => too many deaths by thunder: 99+1 => 200 should fix this
	condition_matches (r<gs.N,"random");
        }	/* 1575 */ break;
	case 8: // I
	/* 1580 */ 
	condition_matches (gs.N!=gs.SALLE,"room does not match");
         
	/* 1585 */ break;
      default:
	print("unsupported condition",str$(gs.LI),0);
	gs.OK = 0;
    }
}


void display_room(int room_number)
{
  switch(room_number)
    {
      case 7000:
	draw_10000_outside_house();
	/* 7005 */ print("Vous etes devant le manoir du defunt",PRINT_LINEFEED,0);print(0);oric_wait(250);
	/* 7006 */ print(spc(12),"Dr  GENIUS",PRINT_LINEFEED,0);

	break;
      case 7010:
	draw_10100_hall();
	/* 7015 */ print("Vous etes dans le hall d'entree.",PRINT_LINEFEED,0);
	break;
      case 7020:
	draw_10200_2nd_floor_stairs();
	/* 7024 */ print("Vous etes en bas de l'escalier menant au 2eme etage",PRINT_LINEFEED,0);
	break;
      case 7030:
	gs.F1 = 0;draw_10300_dining_room_or_library();
	/* 7034 */ print("Vous etes dans la salle a manger.",PRINT_LINEFEED,0);
	break;
      case 7040:
	gs.F1 = 1;draw_10300_dining_room_or_library();
	/* 7043 */ print("Vous etes dans une bibliotheque sans  livre...!",PRINT_LINEFEED,0);
	break;
	case 7050: draw_10500_laundry_room();
	/* 7053 */ print("Vous etes dans une buanderie",PRINT_LINEFEED,0);
	break;
	case 7060: draw_10600_lounge();
	/* 7063 */ print("Vous etes dans le salon",PRINT_LINEFEED,0);
	break;
	case 7070: gs.LX = 0;draw_10700_bedroom();
	/* 7073 */ print("Vous etes dans une chambre.",PRINT_LINEFEED,0);
	break;
	case 7080: draw_10800_corridor();
	/* 7083 */ print("Vous etes dans un corridor",PRINT_LINEFEED,0);
	break;
	case 7090: gs.LX = 0;draw_10900_small_empty_bath_waiting_room();
	/* 7093 */ print("Vous etes dans une salle d'attente.",PRINT_LINEFEED,0);
	break;
	case 7100: gs.LX = 0;draw_11000_lobby_smoke_room();
	/* 7103 */ print("Vous etes dans le vestibule",PRINT_LINEFEED,0);
	break;
	case 7110: gs.LX = 2;draw_10700_bedroom();
	/* 7113 */ print("Vous etes dans la chambre d'amis.",PRINT_LINEFEED,0);
	break;
	case 7120: gs.LX = 1;draw_10700_bedroom();
	/* 7123 */ print("Vous etes dans une chambre.",PRINT_LINEFEED,0);
	break;
	case 7140: gs.LX = 2;draw_12200_room();
	/* 7143 */ print("Vous etes dans une petite salle",PRINT_LINEFEED,0);
	
	case 7150: draw_11500_laboratory();
	/* 7153 */ print("Vous etes dans le laboratoire du                Dr GENIUS",PRINT_LINEFEED,0);
	
	case 7160: gs.LX = 1;draw_10900_small_empty_bath_waiting_room();
	/* 7163 */ print("Vous etes dans une petite piece vide.",PRINT_LINEFEED,0);
	break;
	case 7170: draw_11700_dont_know_where();
	/* 7173 */ print("Vous etes...",PRINT_LINEFEED,0);oric_wait(300);print("Justement,vous ne savez pas ou vous   etes",PRINT_LINEFEED,0);
	break;
	case 7180: draw_11800_upstairs();
	/* 7183 */ print("Vous etes en haut de l'escalier",PRINT_LINEFEED,0);
	break;
	case 7190: gs.LX = 2;draw_10900_small_empty_bath_waiting_room();
	/* 7193 */ print("Vous etes dans la salle de bain",PRINT_LINEFEED,0);
	break;
	case 7200: gs.LX = 1;draw_12200_room();
	/* 7203 */ print("Vous etes dans le living room",PRINT_LINEFEED,0);
	break;
	case 7210: gs.LX = 1;draw_11000_lobby_smoke_room();
	/* 7213 */ print("Vous etes dans une piece enfumee...!",PRINT_LINEFEED,0);
	break;
      case 7220:
	gs.LX = 0;draw_12200_room();
	/* 7223 */ print("Vous etes dans une grande piece",PRINT_LINEFEED,0);
	break;
	case 7230:
	   draw_12300_storage_room();
	/* 7233 */ print("Vous etes dans une piece de rangement",PRINT_LINEFEED,0);
	break;
      case 7240:
	draw_12400_dressing_room();
	/* 7243 */ print("Vous etes dans le dressing",PRINT_LINEFEED,0);
	break;
	/* 8000 */ 
    }
  wait_msg();
}


void load_game_data()
{
       //  CHARGEMENT VARIABLES 
      /* 8001 */ restore();
      /* 8010 */ gs.V = 70;
      /* 8020 */ for (gs.N=1;gs.N<=gs.V;gs.N++)
         {
         
         /* 8030 */ data_read(data_array,total_nb_data,1,DT_STRING,gv.str_known_keywords[gs.N-1]);
         /* 8050 */ 
      }
      
      /* 8060 */  //$DATA$ 01N,01NORD,02S,02SUD,03E,03EST,04O,04OUEST,05MONT,05GRIM,06DESC 
      /* 8070 */  //$DATA$ 10PREN,10RAMA,11POSE,12OUVR,13FERM,14ENTR,14AVAN,15ALLU,16ETEI 
      /* 8080 */  //$DATA$ 17REPA,17DEPA,18LIS,19REGA,20RETO,21RENI,21SENS,22REMP,23VIDE 
      /* 8090 */  //$DATA$ 24INVE,24LIST,25RIEN,25ATTE,26POIG,27COUT,28TOUR,29LAMP,30CODE 
      /* 8100 */  //$DATA$ 31ESCA,32PIST,33PLAC,34TORC,35TELE,36MONS,37PETR,38POT,18LIT 
      /* 8110 */  //$DATA$ 39CLEF,40PAPI,41LIVR,42BRIQ,43COMB,44COFF,45ROUG,46BLEU,47VERT 
      /* 8120 */  //$DATA$ 48TITR,49ROBI,50CISE,51PORT 
      /* 8124 */  //$DATA$ 52ACTI,53JETE,53LANCE,54EAU,55ENFI,55PASS,56APPU,56ENFO,57ENLE 
      /* 8126 */  //$DATA$ 58RENT 
      /* 8130 */ gs.nb_objects = 25; // rooms containing objects
      /* 8140 */ for (gs.N=1;gs.N<=gs.nb_objects;gs.N++)
         {
         
         /* 8150 */ data_read(data_array,total_nb_data,1,DT_INT,&gv.object_room[gs.N-1]);

         /* 8170 */ 
      }
      
      /* 8180 */  //$DATA$ 06,05,05,08,08,00,00,11,11 
      /* 8190 */  //$DATA$ 13,20,18,16,16,16,16,00,21 
      /* 8200 */  //$DATA$ 00,22,25,12,00,25,00 
      /* 8210 */ for (gs.N=1;(gs.N<=gs.nb_objects);gs.N++)
         {
         
         /* 8220 */ data_read(data_array,total_nb_data,1,DT_STRING,gv.str_object_description[gs.N-1]);

         /* 8240 */ 
      }
      
      /* 8250 */  //$DATA$ UNE TORCHE ELECTRIQUE,UN ROBINET,UN CISEAU,UN TOURNEVIS 
      /* 8260 */  //$DATA$ UNE LAMPE A PETROLE,UNE LAMPE PLEINE,UNE LAMPE ALLUME,UN COUTEAU 
      /* 8270 */  //$DATA$ UN PAPIER,UN LIVRE,DU PETROLE DANS UN LAVABO BOUCHE 
      /* 8280 */  //$DATA$ UNE CLEF,UN BOUTON ROUGE,UN BOUTON BLEU 
      /* 8290 */  //$DATA$ UN BOUTON VERT,UN TELEPORTEUR,UN TELEPORTEUR REPARE 
      /* 8300 */  //$DATA$ UNE COMBINAISON ARGENTEE,UNE COMBINAISON ENFILEE,UN MONSTRE ALL'EST 
      /* 8310 */  //$DATA$ UN PISTOLET,UN BRIQUET,UN BRIQUET ALLUME,UN POT,UN POT PLEIN D'EAU 
      /* 8320 */ gs.M = 25;
      /* 8330 */ for (gs.N=1;gs.N<=gs.M;gs.N++)
         {
         
         /* 8340 */ data_read(data_array,total_nb_data,1,DT_STRING,gv.str_M[gs.N-1]);

      }
      
      /* 8370 */  //$DATA$ 00,0403030400,030200,04020305010600,04040107032000,020400 
      /* 8380 */  //$DATA$ 04080109020500,030700,04130207031000  
      /* 8390 */  //$DATA$ 0409021100,0110031200,041100,030900,0209031500,00,00 
      /* 8400 */  //$DATA$00,00,0122032100,040500,0125022200,012100  
      /* 8410 */  //$DATA$ 0124042200,022300,022100 
      /* 8430 */ gs.A = 128;
      /* 8440 */ for (gs.N=1;gs.N<=gs.A;gs.N++)
         {
         
         /* 8450 */ data_read(data_array,total_nb_data,1,DT_STRING,gv.str_A[gs.N-1]);
         /* 8470 */ 
      }
      
      /* 8480 */  //$DATA$ 1400A01.I02D02M.,0500A03D08.D03N.,0500A03E08E09D24.D04D05I19E02M. 
      /* 8485 */  //$DATA$ 0500A03E08D24.D04D06N. 
      /* 8490 */  //$DATA$ 0500A03E07.I19M.,0500A03E03.I19M.,0500A03.I19E02M.,0600A19D08.D03N. 
      /* 8500 */  //$DATA$0600A19E08E09D24.D04D05I03M.,0600A19E08D24.D04D06N.,0600A19.I03M. 
      /* 8505 */  //$DATA$ 0100A09E07B22.D07N. 
      /* 8510 */  //$DATA$ 0100A09E03B05.D07N.,0100A09.I14E02M.,0100A14.I16E02M. 
      /* 8515 */  //$DATA$ 0200A16E07B22.D07N.,0200A16E03B05.D07N. 
      /* 8520 */  //$DATA$ 0200A16.I14E02M.,0400A15E03B05.D07N.,0400A15E07B22.D07N. 
      /* 8522 */  //$DATA$ 0400A15.I14E02M.,0100A15E03.I17M.,0100A15E07.I17M.,0100A15.I17E02M. 
      /* 8525 */  //$DATA$ 0200A17.F01I15M. 
      /* 8530 */  //$DATA$ 0300A17.D08N.,0400A17.D09K.,0300A18.D10F03E01E02I17M. 
      /* 8531 */  //$DATA$ 0400A21E03.I19M. 
      /* 8535 */  //$DATA$ 0400A21E07.I19M.,0400A21.I19E02M.,0200A22E03.I19M.,0200A22E07.I19M. 
      /* 8540 */  //$DATA$ 0200A22.I19E02M.,0200A19.D11N.,0400A19.D11N.,0300A22.D12I23M. 
      /* 8541 */  //$DATA$ 2500A01.D13.,2500I01.D14K.,1244A03.D15M.,1034B01.B01J.,1027B08.B08J. 
      /* 8545 */  //$DATA$ 1028B04.B04J.,1029B05.B05J.,1032B21.B21J.,1038B24.B24J. 
      /* 8548 */  //$DATA$ 1039B12.B12J.,1040B09.B09J.,1041B10.B10J.,1043B18.B18J. 
      /* 8550 */  //$DATA$ 1050B03.B03J.,1042B22.B22J.,1037A20B05.H11P05E05D16K.,1037A20.D17K. 
      /* 8553 */  //$DATA$ 1134.C01J.,1127.C08J.,1128.C04J.,1129.C05J.,1132.C21J.,1138.C24J. 
      /* 8555 */  //$DATA$ 1143E09.D62K. 
      /* 8556 */  //$DATA$ 1139.C12J.,1140.C09J.,1141.C10J.,1143.C18J.,1150.C03J.,1142.C22J. 
      /* 8560 */  //$DATA$ 2400.A00L.,1249A05.E04D20G0405J.,1349A05.F04J.,2238A05E04.P24E08J. 
      /* 8563 */  //$DATA$ 2338A05E08.F08P24J.,2338E08.D21N.,1848B10.D22L.,1841B10.D23N. 
      /* 8566 */  //$DATA$1840B09.D24K.,2040B09.D25K.,1951A02.D26M.,1951.D27K.,2100A14.D28K. 
      /* 8570 */  //$DATA$ 2100.D29K.,1542C22.D33K.,1542E07.D30K.,1542A14.D07N. 
      /* 8575 */  //$DATA$ 1542A17E01.D10K.,1542E02.F02E07E06P22M.,1542.E07P22J. 
      /* 8580 */  //$DATA$ 1529C05.D33K.,1529E03.D30K.,1529F07.D31L.,1529F05.D32L. 
      /* 8590 */  //$DATA$ 1529E02.F02E03E06P06P05M.,1529.E03P06P05J. 
      /* 8595 */  //$DATA$ 1642C22.D33K.,1642F07.D30K.,1642E06E03.D36F07P22M. 
      /* 8597 */  //$DATA$ 1642E06.E02F07F06P22M. 
      /* 8600 */  //$DATA$ 1642.F07P22M.,1629C05.D33K.,1629F03.D30K.,1629E07E06.D34F03P05M. 
      /* 8605 */  //$DATA$ 1629E06.E02F06F03P05M.,1629.F03P05M.,1534B01.D35N. 
      /* 8610 */  //$DATA$ 1735I16.D45K.,1735E02.D43K.,1735F03.D44K.,1735C04.D46K. 
      /* 8615 */  //$DATA$ 1735.P16E10J.,5600A16F10.D47K.,5646A16.D48N.,5647A16.D48N. 
      /* 8620 */  //$DATA$ 5645A16F09.D50D06N.,5645A16.D49I18M.,5543D18E09.D30K. 
      /* 8625 */  //$DATA$ 5543D18.P18E09J.,574EXPLODEAND18F09.D30K.,5743D18.P18F09J. 
      /* 8630 */  //$DATA$ 1233A24C12.D51K.,1233A24C03.D52N.,1233A24.G0503E11D63K. 
      /* 8635 */  //$DATA$ 2636E11.D54F11D55K.,5350E11.D54F11D55K.,5232B21.D56N. 
      /* 8640 */  //$DATA$ 5830F08.D57.,5830.D58D59.,1233A06.D61M. 
      /* 8650 */  //$DATA$ 1233A25.D64N. 
      /* 8700 */ gs.PL = integer(rnd(1)*9000+1000);
      /* 8800 */ gs.C = 14;
      /* 8810 */ for (gs.N=1;gs.N<=gs.C;gs.N++)
         {
         
         /* 8820 */ data_read(data_array,total_nb_data,1,DT_STRING,gv.str_condition_group[gs.N-1]);

         /* 8840 */ 
      }
      
      /* 8850 */  //$DATA$ G03E03.D00N.,G04E04.D01N.,I14I16I17I19.F02.,G07E07.D18N.,GO1.D19N. 
      /* 8860 */  //$DATA$ H06C03C08.D37N.,H08D08.D39L.,H06D03.D38L.,G08E08B24.D40D21N. 
      /* 8870 */  //$DATA$ H02.D41N.,G09E02.D42N.,G05E11.D52N.,I24E11.D53D52N.,.L. 
      /* 9000 */ 
}

void BP_9000()
{
      text();
      /* 9005 */ gs.N = 48000;poke(gs.N,96);poke(gs.N+2,76);poke(gs.N+3,79);poke(gs.N+4,82);poke(gs.N+5,73);poke(gs.N+6,67);
      /* 9010 */ poke(gs.N+7,73);poke(gs.N+8,69);poke(gs.N+9,76);poke(gs.N+10,83);poke(gs.N+11,32);
      /* 9015 */ poke(gs.N+12,32);poke(gs.N+13,56);poke(gs.N+14,51);return;
      /* 10000 */ 
}

void draw_10000_outside_house()
{
      hires();ink(0);curset(100,190,1);
      /* 10005 */ draw(0,-60,1);draw(97,0,1);draw(7,10,1);draw(-97,0,1);draw(-7,-10,1);draw(15,-20,1);
      /* 10010 */ draw(-30,-50,1);draw(-15,20,1);draw(30,50,1);curset(100,190,1);draw(-30,-50,1);
      /* 10015 */ draw(0,-60,1);curset(100,190,1);draw(22,0,1);draw(0,-30,1);draw(15,0,1);draw(0,30,1);
      /* 10020 */ draw(60,0,1);draw(0,-50,1);curmov(0,-10,1);draw(0,-30,1);draw(-67,-40,1);
      /* 10025 */ draw(-23,30,1);draw(-30,-50,1);draw(23,-30,1);draw(30,50,1);curset(197,100,1);
      /* 10030 */ draw(-45,-60,1);draw(-52,-30,1);curset(85,60,1);draw(0,-6,1);curset(115,110,1);
      /* 10035 */ draw(0,-30,1);curmov(7,0,1);draw(7,-10,1);draw(7,10,1);draw(0,20,1);draw(-15,0,1);
      /* 10040 */ draw(0,-20,1);curmov(7,-10,1);draw(0,30,1);curset(182,100,1);draw(-30,0,1);
      /* 10045 */ draw(0,20,1);draw(30,0,1);draw(0,-20,1);curmov(-7,0,1);draw(0,20,1);curmov(-16,0,1);
      /* 10050 */ draw(0,-20,1);curset(122,190,1);draw(13,7,1);draw(0,-30,1);draw(-13,-7,1);
      /* 10055 */ curset(133,182,1);curset(182,150,1);draw(-30,0,1);draw(0,20,1);draw(30,0,1);
      /* 10060 */ draw(0,-20,1);curmov(-7,0,1);draw(0,20,1);curmov(-16,0,1);draw(0,-20,1);
      /* 10065 */ curset(78,130,1);draw(15,25,1);draw(0,-20,1);draw(-15,-25,1);draw(0,20,1);
      /* 10070 */ curmov(3,5,1);draw(0,-20,1);curmov(9,15,1);draw(0,20,1);ink(3);return;
      /* 10100 */ 
}

void draw_10100_hall()
{
      hires();ink(0);curset(15,190,1);draw(0,-160,1);draw(75,-20,1);draw(0,40,1);
      /* 10110 */ draw(0,-40,1);draw(60,0,1);draw(0,20,1);draw(10,10,1);draw(-40,0,1);draw(40,0,1);
      /* 10120 */ draw(0,3,1);draw(-40,0,1);draw(0,-3,1);draw(0,-10,1);draw(30,0,1);draw(0,-20,1);
      /* 10130 */ draw(82,20,1);draw(0,160,1);draw(-73,-126,1);draw(73,126,3);draw(-30,-50,3);
      /* 10140 */ draw(0,-80,1);draw(-22,-16,1);draw(0,58,1);curmov(19,-9,1);circle(1,1);
      /* 10145 */ curset(15,190,1);draw(37,-68,1);draw(0,-70,1);draw(18,-13,1);draw(0,50,1);
      /* 10150 */ draw(-18,33,3);draw(38,-71,1);draw(31,0,1);draw(0,-8,3);draw(0,23,1);draw(3,0,1);
      /* 10155 */ draw(0,-23,1);draw(0,8,1);draw(21,0,1);draw(0,-8,1);draw(0,8,1);draw(1,2,1);
      /* 10157 */ draw(0,-10,1);draw(0,10,1);
      /* 10160 */ draw(2,0,1);draw(0,-10,1);draw(0,8,1);draw(2,0,1);draw(0,-8,1);draw(0,8,1);
      /* 10165 */ draw(3,5,1);draw(0,-12,1);draw(0,20,1);draw(2,3,1);draw(0,-23,1);draw(0,23,1);
      /* 10170 */ draw(3,0,1);draw(0,-23,1);curset(67,68,1);circle(1,1);ink(3);return;
      /* 10200 */ 
}

void draw_10200_2nd_floor_stairs()
{
      hires();ink(0);curset(15,190,1);draw(0,-120,1);draw(105,-50,1);draw(60,0,1);
      /* 10205 */ draw(45,50,1);draw(0,110,1);draw(-45,-100,1);curmov(0,-60,1);draw(0,60,1);
      /* 10210 */ draw(-60,0,1);curmov(0,-60,1);draw(0,60,1);draw(-52,54,1);curset(15,190,1);
      /* 10215 */ draw(24,-26,1);draw(0,-104,1);draw(0,60,1);draw(27,14,1);draw(0,-10,1);
      /* 10220 */ draw(53,-50,1);draw(-6,-2,1);draw(-54,47,1);curmov(7,3,1);draw(-7,-3,1);
      /* 10225 */ draw(0,-10,1);draw(53,-44,1);draw(0,8,1);draw(0,-8,1);draw(-5,-2,1);draw(-56,40,1);
      /* 10230 */ draw(9,4,1);draw(-9,-4,1);draw(0,-10,1);draw(55,-36,1);draw(0,7,1);draw(0,-7,1);
      /* 10235 */ draw(-5,-2,1);draw(-56,33,1);draw(7,3,1);draw(-7,-3,1);draw(0,-10,1);
      /* 10240 */ draw(-5,-2,1);draw(5,2,1);draw(57,-30,1);draw(0,8,1);draw(0,-8,1);draw(-5,-2,1);
      /* 10245 */ draw(-57,25,1);draw(57,-25,1);draw(0,-7,1);draw(-57,24,1);draw(57,-24,1);
      /* 10250 */ draw(-5,-2,1);draw(-16,5,1);draw(16,-5,1);draw(0,-4,1);
      /* 10255 */ curset(211,146,1);draw(0,-64,1);draw(-15,-20,1);draw(0,52,1);curset(207,108,1);
      /* 10260 */ circle(1,1);curset(156,80,1);draw(24,0,0);draw(6,12,0);draw(-6,-12,0);draw(0,-40,0);
      /* 10265 */ draw(-26,0,1);draw(0,40,1);draw(5,12,1);draw(27,0,1);draw(0,-42,1);draw(-6,-10,1);
      /* 10270 */ draw(6,10,1);draw(-27,0,1);draw(-4,-10,1);draw(4,10,1);draw(0,42,1);
      /* 10275 */ curset(169,70,0);circle(4,1);curset(169,66,1);draw(0,-3,1);curset(169,74,1);
      /* 10280 */ draw(0,2,1);curset(165,70,1);draw(-2,0,1);curset(173,70,1);draw(2,0,1);
      /* 10285 */ curset(165,58,1);circle(1,1);curset(170,58,1);circle(1,1);curset(175,58,1);
      /* 10290 */ circle(1,1);ink(3);return;
      /* 10300 */ 
}

void draw_10300_dining_room_or_library()
{
      hires();ink(0);curset(15,190,1);draw(67,-110,1);draw(68,0,1);draw(82,110,1);
      /* 10305 */ draw(0,-140,1);draw(-82,-40,1);draw(0,70,0);draw(0,-70,1);draw(-68,0,1);
      /* 10310 */ draw(0,70,0);draw(0,-70,1);draw(-67,40,1);draw(0,140,1);curset(90,80,1);
      /* 10315 */ draw(0,-50,1);draw(24,0,1);draw(0,50,1);draw(-24,0,0);draw(18,10,1);draw(0,-50,1);
      /* 10320 */ draw(-18,-10,1);curset(105,64,1);
      /* 10325 */ curset(210,160,1);draw(0,-90,1);draw(-23,-14,1);draw(0,74,1);curset(205,110,1);
      /* 10330 */ curset(45,140,1);draw(0,-74,1);draw(18,-14,1);draw(0,60,1);curset(60,86,1);
      /* 10335 */ if (gs.F1 == 0)
         {
         ink(3);return;
      }
      /* 10400 */ ink(0);curset(124,80,1);draw(26,0,0);draw(0,-70,0);draw(0,70,0);
      /* 10401 */ curset(149,80,0);draw(12,14,0);
      /* 10402 */ curset(150,80,0);draw(10,14,0);
      /* 10405 */ curset(168,18,1);draw(0,86,1);draw(-38,0,1);draw(0,-86,1);draw(38,0,1);
      /* 10410 */ draw(-38,0,1);draw(-6,-8,1);draw(0,70,1);draw(6,24,1);
      /* 10415 */ curset(168,94,1);draw(-38,0,1);curset(168,94,1);draw(-8,-10,1);curset(168,84,1);
      /* 10420 */ draw(-38,0,1);curset(168,84,1);draw(-8,-10,1);curset(168,74,1);draw(-38,0,1);
      /* 10430 */ curset(168,74,1);draw(-9,-10,1);curset(168,64,1);draw(-38,0,1);curset(168,64,1);
      /* 10435 */ draw(-9,-10,1);curset(168,54,1);draw(-38,0,1);curset(168,54,1);draw(-18,-18,1);
      /* 10440 */ draw(-18,0,1);draw(18,0,1);draw(0,-16,1);
      /* 10445 */ ink(3);return;
      /* 10500 */ 
}

void draw_10500_laundry_room()
{
      hires();ink(0);curset(13,180,1);draw(0,-132,1);draw(63,-28,1);draw(83,0,1);
      /* 10505 */ draw(67,30,1);draw(0,130,1);draw(-11,-14,1);draw(-4,-4,0);draw(-17,-25,1);
      /* 10510 */ draw(-24,-36,0);draw(-12,-20,1);draw(0,-61,1);draw(0,61,1);draw(-83,0,1);
      /* 10515 */ draw(0,-60,1);draw(0,60,1);draw(-63,100,1);curset(22,90,1);draw(19,0,1);
      /* 10520 */ draw(0,-30,3);draw(-19,0,1);draw(0,30,1);draw(0,-30,1);draw(33,-20,1);
      /* 10525 */ draw(13,0,1);draw(0,20,1);draw(-28,30,1);draw(0,-30,1);draw(27,-20,1);
      /* 10530 */ draw(-11,9,1);draw(0,23,1);curset(52,64,1);curset(60,58,1);curset(157,100,1);
      /* 10535 */ draw(30,0,1);draw(29,34,1);draw(-40,0,1);draw(0,2,1);draw(40,0,1);draw(0,-2,1);
      /* 10540 */ draw(0,2,3);draw(-2,0,3);draw(0,34,1);draw(-3,0,1);draw(0,-34,1);draw(-30,0,1);
      /* 10545 */ draw(0,34,1);draw(-3,0,1);draw(0,-34,1);draw(-2,0,3);draw(-20,-34,1);draw(0,-2,1);
      /* 10550 */ draw(20,34,1);curset(159,104,1);draw(0,24,1);draw(3,0,1);draw(0,-18,1);
      /* 10555 */ ink(3);return;
      /* 10600 */ 
}

void draw_10600_lounge()
{
      hires();ink(0);curset(15,190,1);draw(0,-130,1);draw(75,-50,1);
      /* 10605 */ draw(52,0,1);draw(90,50,1);draw(0,130,1);draw(-22,-32,1);draw(0,-52,1);
      /* 10610 */ draw(-3,-5,1);draw(-42,-43,1);draw(-6,0,1);draw(-2,4,1);draw(0,10,1);draw(-12,0,1);
      /* 10613 */ draw(5,0,1);draw(-7,-10,1);draw(0,-50,1);draw(0,50,1);
      /* 10615 */ draw(-22,0,1);draw(0,-30,1);draw(-15,0,1);draw(13,3,1);draw(0,30,1);
      /* 10620 */ draw(-13,-3,1);draw(0,-30,1);draw(0,30,1);draw(-15,0,1);draw(0,-50,1);draw(0,50,1);
      /* 10625 */ draw(-23,40,1);draw(-22,0,1);draw(22,0,1);draw(0,-52,1);draw(-22,22,1);draw(0,70,1);
      /* 10630 */ draw(-30,50,1);curset(116,49,1);curset(210,158,1);draw(-45,0,1);draw(0,-18,1);
      /* 10633 */ draw(0,18,1);
      /* 10635 */ draw(-26,-60,0);draw(0,-10,1);draw(6,-16,1);draw(12,0,3);draw(4,4,1);draw(-12,0,1);
      /* 10640 */ draw(-4,-4,1);draw(4,4,1);draw(-6,14,1);draw(10,-2,1);draw(9,0,1);draw(0,-11,1);
      /* 10645 */ draw(0,11,1);draw(19,25,1);curset(165,140,1);draw(10,-20,1);draw(21,0,1);
      /* 10650 */ draw(-5,-7,1);draw(-21,0,1);draw(5,7,1);draw(-5,-7,1);draw(-7,14,1);
      /* 10655 */ draw(-20,-37,1);curset(165,140,1);draw(-26,-50,1);curset(165,158,1);
      /* 10660 */ draw(-26,-59,1);draw(0,-1,1);curset(196,120,1);draw(0,-14,1);draw(2,-5,1);
      /* 10665 */ draw(9,0,1);draw(-9,0,1);draw(-39,-43,1);
      /* 10670 */ ink(3);return;
      /* 10700 */ 
}

void draw_10700_bedroom()
{
      hires();ink(0);curset(15,190,1);draw(57,-106,1);draw(-57,106,1);draw(0,-150,1);
      /* 10705 */ draw(67,-30,1);draw(0,32,1);draw(0,-32,1);draw(75,0,1);draw(0,50,1);draw(0,-50,1);
      /* 10710 */ draw(75,30,1);draw(0,150,1);draw(-75,-130,1);curset(202,138,1);draw(0,-78,1);
      /* 10715 */ draw(-22,-18,1);draw(0,56,1);curset(157,60,1);draw(-9,0,1);draw(2,3,3);
      /* 10720 */ draw(-78,0,1);draw(0,8,1);draw(78,0,2);draw(0,-8,1);draw(0,20,1);
      /* 10725 */ draw(-78,0,1);draw(0,-38,1);draw(10,-12,1);curset(82,42,1);draw(-10,13,1);
      /* 10730 */ draw(6,1,1);draw(9,-13,1);draw(-4,-1,1);draw(4,1,1);draw(0,6,1);draw(-9,13,1);
      /* 10735 */ draw(0,1,1);draw(0,-7,1);draw(0,6,1);draw(9,-13,1);draw(55,0,1);draw(8,15,1);
      /* 10738 */ if (gs.LX == 2)
         {
         goto label_10745;
      }
      /* 10740 */ curset(197,93,1);circle(1,1);
      /* 10745 */ 
label_10745:
      if (gs.LX == 0)
         {
         ink(3);return;
      }
      /* 10750 */ if (gs.LX == 1)
         {
         goto label_10780;
      }
      /* 10755 */ curset(202,137,0);draw(0,-77,0);draw(-22,-18,0);draw(0,55,0);
      /* 10760 */ curset(30,162,1);draw(26,-49,0);draw(0,1,0);draw(-26,49,0);
      /* 10765 */ curset(30,162,1);draw(0,-80,1);draw(22,-22,1);draw(0,9,1);draw(0,-9,1);
      /* 10770 */ draw(-22,22,1);draw(27,-15,1);draw(0,77,1);draw(-25,18,1);curset(51,111,1);
      /* 10775 */ circle(1,1);ink(3);return;
      /* 10780 */ 
label_10780:
      curset(30,50,1);draw(0,40,1);draw(22,-24,1);draw(0,-29,1);draw(-22,14,1);
      /* 10785 */ draw(12,-8,3);draw(0,33,1);
      /* 10790 */ ink(3);return;
      /* 10800 */ 
}

void draw_10800_corridor()
{
      hires();ink(0);curset(15,190,1);draw(0,-140,1);draw(82,-40,1);draw(53,0,1);
      /* 10805 */ draw(75,40,1);draw(0,140,1);draw(-30,-50,1);draw(0,-80,1);draw(-26,-22,1);
      /* 10810 */ draw(0,60,1);draw(26,0,1);draw(-26,0,1);draw(-19,-30,1);draw(0,-58,1);
      /* 10812 */ draw(0,58,1);draw(-22,0,1);
      /* 10815 */ draw(0,-40,1);draw(-15,0,1);draw(13,4,1);draw(0,40,1);draw(-13,-4,1);draw(0,-40,1);
      /* 10820 */ draw(0,40,1);draw(-15,0,1);draw(0,-58,1);draw(0,58,1);draw(-30,42,1);draw(-22,0,1);
      /* 10825 */ draw(22,0,1);draw(0,-68,1);draw(-22,16,1);draw(0,86,1);draw(-30,43,1);
      /* 10830 */ curset(123,52,1);ink(3);return;
      /* 10900 */ 
}

void draw_10900_small_empty_bath_waiting_room()
{
      hires();ink(0);curset(232,190,1);draw(0,-150,1);draw(-82,-30,1);draw(0,50,1);
      /* 10905 */ draw(0,-50,1);draw(-60,0,1);draw(0,50,1);draw(0,-50,1);draw(-75,30,1);draw(0,150,1);
      /* 10910 */ if (gs.LX == 0)
         {
         draw(75,-130,1);draw(-23,40,3);goto label_10920;
      }
      /* 10915 */ draw(30,-52,1);draw(-30,52,1);draw(75,-130,3);draw(-23,40,1);draw(-22,0,1);
      /* 10918 */ draw(22,0,1);
      /* 10920 */ 
label_10920:
      draw(0,-58,1);draw(-22,16,1);draw(0,80,1);
      /* 10925 */ curset(90,60,1);draw(60,0,1);draw(82,130,1);
      /* 10930 */ if (gs.LX == 0)
         {
         curset(63,78,1);circle(1,1);ink(3);return;
      }
      /* 10935 */ if (gs.LX == 1)
         {
         ink(3);return;
      }
      /* 10940 */ curset(210,100,1);draw(-15,0,1);draw(-3,-10,1);draw(18,0,1);draw(0,10,1);
      /* 10945 */ draw(0,-10,1);draw(-27,-25,1);draw(0,5,1);draw(20,20,1);draw(-20,-20,1);
      /* 10950 */ draw(-8,0,1);draw(8,0,1);draw(0,-5,1);draw(-12,0,1);draw(21,25,1);draw(-21,-25,1);
      /* 10955 */ draw(-1,0,1);draw(2,8,1);draw(21,26,1);
      /* 10960 */ ink(3);return;
      /* 11000 */ 
}

void draw_11000_lobby_smoke_room()
{
      hires();ink(0);curset(15,190,1);draw(0,-150,1);draw(82,-30,1);draw(0,40,1);
      /* 11005 */ draw(0,-40,1);draw(60,0,1);draw(0,40,1);draw(0,-40,1);draw(75,30,1);draw(0,150,1);
      /* 11010 */ draw(-22,-42,1);draw(0,-75,1);draw(-23,-21,1);draw(0,52,1);draw(23,0,1);
      /* 11015 */ draw(-23,0,1);draw(-30,-54,1);draw(-60,0,1);draw(-82,140,0);draw(82,-140,1);
      /* 11020 */ draw(15,0,1);draw(0,-24,1);draw(12,0,1);draw(0,24,1);curmov(-3,-12,1);
      /* 11025 */ if (gs.LX == 0)
         {
         ink(3);return;
      }
      /* 11030 */ curset(187,104,1);draw(22,0,0);curmov(-22,0,1);draw(23,44,1);curmov(-5,-45,1);
      /* 11035 */ circle(1,1);curset(30,50,1);draw(0,40,1);draw(37,-34,1);draw(0,-25,1);draw(-37,20,1);
      /* 11040 */ draw(21,-10,3);draw(0,31,1);ink(3);return;
      /* 11500 */ 
}

void draw_11500_laboratory()
{
      hires();ink(0);curset(15,180,1);draw(0,-140,1);draw(67,-30,1);draw(75,0,1);
      /* 11505 */ draw(67,30,1);draw(0,150,1);draw(-42,-70,1);draw(0,-70,1);draw(-24,-20,1);
      /* 11510 */ draw(0,-20,1);draw(0,20,1);draw(-45,0,1);draw(-15,20,1);draw(85,0,1);draw(-33,0,1);
      /* 11515 */ draw(0,70,1);draw(33,0,1);draw(-34,0,1);draw(-14,-40,1);draw(0,-30,1);draw(0,30,1);
      /* 11520 */ draw(-22,0,1);draw(0,-30,1);draw(0,30,1);draw(-15,40,1);draw(0,-70,1);draw(0,30,1);
      /* 11525 */ draw(-15,0,1);draw(0,-70,1);draw(0,70,1);draw(-67,100,1);
      /* 11530 */ curset(167,60,1);circle(1,1);curmov(0,10,1);circle(1,1);curmov(0,10,1);circle(1,1);
      /* 11540 */ curmov(0,20,1);draw(6,2,1);draw(-6,-2,1);circle(8,1);
      /* 11545 */ poke(43346,1);poke(43348,3);poke(43386,1);poke(43388,3);
      /* 11550 */ poke(43746,4);poke(43748,3);poke(43786,4);poke(43788,3);
      /* 11555 */ poke(44146,2);poke(44148,3);poke(44186,2);poke(44188,3);
      /* 11560 */ ink(3);return;
      /* 11700 */ 
}

void draw_11700_dont_know_where()
{
      hires();ink(0);curset(15,190,1);draw(0,-140,1);draw(60,-40,1);draw(75,0,1);
      /* 11705 */ draw(82,40,1);draw(0,140,1);draw(-45,-72,3);draw(0,-54,1);draw(-15,-14,1);
      /* 11710 */ draw(0,44,1);curset(232,190,1);draw(-82,-130,1);draw(0,-50,1);draw(0,50,1);
      /* 11715 */ draw(-75,0,1);draw(0,-50,1);draw(0,50,1);draw(-4,10,1);draw(9,0,1);draw(-23,0,1);
      /* 11720 */ draw(-34,50,1);draw(39,0,1);draw(18,-50,1);draw(0,4,1);draw(-18,52,1);
      /* 11725 */ draw(0,-6,1);draw(0,6,1);draw(-39,0,1);draw(0,-6,1);
      /* 11730 */ curset(15,190,1);draw(15,-33,1);draw(0,-30,1);
      /* 11735 */ draw(4,0,3);draw(0,37,1);draw(-4,0,1);draw(0,-7,1);draw(0,7,1);draw(4,0,1);
      /* 11740 */ draw(3,-8,1);draw(0,-29,1);draw(15,0,3);draw(0,37,1);draw(4,0,1);draw(0,-37,1);
      /* 11745 */ draw(0,37,1);draw(3,-8,1);draw(0,-29,1);draw(-14,0,3);draw(-7,16,1);
      /* 11750 */ curset(74,93,1);draw(0,12,1);draw(3,0,1);draw(0,-19,1);draw(0,19,1);
      /* 11755 */ draw(2,-7,1);draw(0,-18,1);
      /* 11760 */ curset(184,86,1);ink(3);return;
      /* 11800 */ 
}

void draw_11800_upstairs()
{
      hires();ink(0);curset(15,199,1);draw(0,-160,1);draw(75,-30,1);draw(52,0,1);
      /* 11805 */ draw(90,30,1);draw(0,160,1);curset(15,199,1);draw(75,-140,1);draw(0,-50,1);
      /* 11810 */ draw(0,50,1);draw(15,0,1);draw(0,-30,1);draw(12,4,1);draw(0,30,1);draw(-12,-4,1);
      /* 11815 */ draw(0,-30,1);draw(15,0,1);draw(0,30,1);draw(22,0,1);draw(0,-50,1);draw(0,50,1);
      /* 11820 */ draw(36,55,1);draw(21,0,1);draw(-21,0,1);draw(0,-60,1);draw(21,15,1);draw(0,74,1);
      /* 11825 */ draw(6,8,1);draw(-106,0,1);draw(-15,46,1);draw(15,-46,1);draw(0,10,1);draw(106,0,1);
      /* 11830 */ draw(0,-10,1);draw(0,10,1);draw(4,5,1);draw(-112,0,1);draw(2,-5,1);draw(-2,5,1);
      /* 11835 */ draw(0,10,1);draw(112,0,1);draw(0,-10,1);draw(0,10,1);draw(4,6,1);draw(-119,0,1);
      /* 11840 */ draw(3,-6,1);draw(-3,6,1);draw(0,10,1);draw(119,0,1);draw(0,-10,1);draw(0,10,1);
      /* 11845 */ draw(4,6,1);draw(-126,0,1);draw(3,-6,1);curset(115,49,1);
      /* 11850 */ ink(3);return;
      /* 12200 */ 
}

void draw_12200_room()
{
      hires();ink(0);curset(15,190,1);draw(0,-150,1);draw(75,-30,1);draw(0,50,1);
      /* 12205 */ draw(0,-50,1);draw(60,0,1);draw(0,50,1);draw(0,-50,1);draw(82,30,1);draw(0,150,1);
      /* 12210 */ draw(-82,-130,1);draw(-45,0,1);draw(0,-30,1);draw(15,0,1);draw(0,30,1);draw(-30,0,1);
      /* 12215 */ draw(-30,53,1);if (gs.LX == 2)
         {
         draw(-23,0,1);draw(23,0,1);
      }
      /* 12220 */ draw(0,-60,1);draw(-23,17,1);draw(0,80,1);
      /* 12225 */ if (gs.LX!=2)
         {
         draw(23,-37,1);draw(-23,37,1);
      }
      /* 12230 */ draw(-22,40,1);if (gs.LX!=2)
         {
         curset(57,88,1);circle(1,1);
      }
      /* 12235 */ if (gs.LX!=0)
         {
         curset(117,45,1);circle(1,1);ink(3);return;
      }
      /* 12240 */ curset(105,60,1);draw(15,0,0);curmov(-15,0,1);draw(0,-30,1);draw(12,3,1);
      /* 12245 */ draw(0,30,1);draw(-12,-4,1);curset(115,48,1);ink(3);return;
      /* 12300 */ 
}

void draw_12300_storage_room()
{
      hires();ink(0);curset(15,190,1);draw(0,-150,1);draw(67,-30,1);draw(67,0,1);
      /* 12310 */ draw(82,30,1);draw(0,150,1);draw(-65,-104,1);draw(0,-54,1);draw(-16,-10,1);
      /* 12315 */ draw(0,-10,1);draw(0,10,1);draw(-37,0,1);draw(-3,10,1);draw(0,54,1);draw(56,0,1);
      /* 12320 */ draw(0,-54,1);draw(-28,0,1);draw(0,54,1);draw(0,-54,1);draw(-28,0,1);draw(0,30,1);
      /* 12325 */ draw(-27,0,1);draw(0,-50,1);draw(0,50,1);draw(-67,130,1);
      /* 12330 */ curset(133,60,1);circle(1,1);curmov(11,0,1);circle(1,1);
      /* 12340 */ ink(3);return;
      /* 12400 */ 
}

void draw_12400_dressing_room()
{
      hires();ink(0);curset(30,164,1);draw(-15,26,1);draw(0,-143,1);draw(15,-7,1);
      /* 12405 */ draw(0,124,1);draw(15,0,1);draw(0,-124,1);draw(-15,0,1);draw(15,0,1);draw(45,-30,1);
      /* 12410 */ draw(0,13,1);draw(0,-13,1);draw(60,0,1);draw(0,50,1);draw(0,-50,1);draw(82,30,1);
      /* 12415 */ draw(0,150,1);draw(-82,-130,1);draw(-50,0,1);draw(-32,104,1);draw(-22,0,1);
      /* 12420 */ draw(22,0,1);draw(0,-92,1);draw(-22,0,1);draw(44,-48,1);draw(10,0,1);draw(-32,48,1);
      /* 12425 */ draw(32,-48,1);draw(0,35,1);draw(-14,46,3);draw(0,-61,1);
      /* 12430 */ curset(81,88,1);circle(1,1);curmov(11,-21,1);circle(1,1);ink(3);return;
      /* 13000 */ 
}

void draw_grave()
{
      hires();ink(0);curset(75,62,1);draw(-5,0,1);draw(-55,108,1);draw(37,-10,1);
      /* 13005 */ draw(30,-90,1);draw(-19,5,1);draw(19,-5,1);draw(20,5,1);draw(-12,94,1);
      /* 13007 */ draw(-38,-10,1);draw(38,10,1);draw(0,20,1);draw(-75,0,1);draw(0,-20,1);
      /* 13010 */ draw(0,20,1);draw(75,0,1);draw(15,-112,1);draw(0,-15,1);
      /* 13012 */ draw(-3,14,1);draw(3,-14,1);draw(-13,0,1);
      /* 13015 */ draw(0,-12,1);draw(0,12,1);draw(-4,10,1);draw(0,-22,1);draw(12,0,1);
      /* 13020 */ draw(4,-7,1);draw(0,-15,1);draw(-4,6,1);draw(0,16,1);draw(0,-16,1);draw(-12,0,1);
      /* 13025 */ draw(4,-6,1);draw(12,0,1);draw(-12,0,1);draw(0,-16,1);draw(-4,6,1);draw(0,16,1);
      /* 13030 */ draw(0,-16,1);draw(-12,0,1);draw(4,-6,1);draw(12,0,1);draw(-12,0,1);draw(-4,6,1);
      /* 13035 */ draw(0,16,1);draw(-12,0,1);draw(4,-6,1);draw(7,0,1);draw(-7,0,1);draw(-4,6,1);
      /* 13040 */ draw(0,16,1);draw(12,0,1);draw(0,22,1);
      /* 13045 */ ink(3);return;
      /* 20000 */ 

}

void play_sarabande()
{
  //      SARABANDE 
  /* 30005 */ restore();
  // skip the game data
  /* 30010 */ for (gs.N=1;gs.N<=41;gs.N++)
    {
      data_read(data_array,total_nb_data,7,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y);
    }
      
  /* 30015 */ for (gs.N=1;gs.N<=75;gs.N++)
    {
         
      /* 30020 */ data_read(data_array,total_nb_data,5,DT_NUMERIC,&gs.A1,DT_NUMERIC,&gs.A2,DT_NUMERIC,&gs.A3,DT_NUMERIC,&gs.A4,DT_NUMERIC,&gs.A5);
      /* 30025 */ data_read(data_array,total_nb_data,5,DT_NUMERIC,&gs.A6,DT_NUMERIC,&gs.A7,DT_NUMERIC,&gs.A8,DT_NUMERIC,&gs.A9,DT_NUMERIC,&gs.A0);
      /* 30030 */ music(1,gs.A1,gs.A2,gs.A3);music(2,gs.A4,gs.A5,gs.A6);music(3,gs.A7,gs.A8,gs.A9);play(7,0,0,100);oric_wait(gs.A0);
      if (key$()[0]!=0) break;
      /* 30032 */ if (gs.N>70)
	{
	  break;
	}
      /* 30035 */ play(0,0,0,0);
    }
  play(0,0,0,0);
}

void play_badinerie()
{
       //      BADINERIE 
      /* 31005 */ restore();
      /* 31010 */ for (gs.N=1;gs.N<=148;gs.N++)
         {
         data_read(data_array,total_nb_data,7,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y);
      }
      
      /* 31012 */ data_read(data_array,total_nb_data,1,DT_STRING,gs.str_Y);
      /* 31015 */ for (gs.N=1;gs.N<=97;gs.N++)
         {
         
         /* 31020 */ data_read(data_array,total_nb_data,3,DT_NUMERIC,&gs.A1,DT_NUMERIC,&gs.A2,DT_NUMERIC,&gs.A3);
         /* 31022 */ music(1,gs.A1,gs.A2,10);play(1,0,0,100);oric_wait(gs.A3);
         /* 31025 */ play(0,0,0,0);
      }
      

      /* 31170 */ return;
      /* 32000 */ 
}

void BP_32000()
{
       //    TEA FOR TWO 
      /* 32001 */ restore();
      /* 32002 */ for (gs.N=1;gs.N<=166;gs.N++)
         {
         data_read(data_array,total_nb_data,8,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y);
      }
      
      /* 32005 */ for (gs.N=1;gs.N<=110;gs.N++)
         {
         
         /* 32010 */ data_read(data_array,total_nb_data,3,DT_NUMERIC,&gs.A1,DT_NUMERIC,&gs.A2,DT_NUMERIC,&gs.A3);
         /* 32015 */ music(1,gs.A1,gs.A2,10);play(1,0,0,100);oric_wait(gs.A3);
         /* 32020 */ play(0,0,0,0);
         /* 32022 */ 
      }
      
      /* 32025 */  //$DATA$ 4,6,45,4,3,15,4,5,45,4,3,15,4,6,45,4,3,15,4,5,45  
      /* 32030 */  //$DATA$ 4,1,15,4,5,45,4,1,15,4,3,45,4,1,15,4,5,45,4,1,15 
      /* 32035 */  //$DATA$4,3,45,4,1,15,4,6,45,4,3,15,4,5,45,4,3,15,4,6,45 
      /* 32040 */  //$DATA$4,3,15,4,5,45,4,1,30,4,10,30,4,10,22,4,7,8,4,9,30,4,9,22,4,7,8 
      /* 32045 */  //$DATA$4,10,30,4,10,22,4,7,8,4,9,30,4,9,22,4,5,8,4,9,30,4,7,22,4,5,8 
      /* 32050 */  //$DATA$4,7,30,4,7,22,4,5,8,4,9,30,4,9,22,4,5,8,4,7,30,4,7,22,4,5,8 
      /* 32055 */  //$DATA$4,10,30,4,10,22,4,7,8,4,9,30,4,9,22,4,7,8,4,10,30,4,10,22,4,7,8 
      /* 32060 */  //$DATA$4,9,30,4,9,30,5,2,120,5,1,60,4,6,45,4,3,15,4,5,45,4,3,15,4,6,45 
      /* 32065 */  //$DATA$4,3,15,4,5,45,4,1,15,4,5,45,4,1,15,4,3,45,4,1,15,4,5,45,4,1,15 
      /* 32070 */  //$DATA$4,3,45,4,1,15,4,6,45,4,3,15,4,5,45,4,3,15,4,6,45,4,3,15,4,5,45 
      /* 32075 */  //$DATA$4,1,30,5,3,45,5,3,15,5,1,45,5,1,15,4,11,45,4,11,15,4,10,45,4,10,15 
      /* 32080 */  //$DATA$5,1,45,5,1,15,4,11,45,4,11,15,4,10,45,4,10,15,4,8,45,4,8,15 
      /* 32085 */  //$DATA$4,6,45,4,3,15,4,5,45,4,3,15 
      /* 32090 */  //$DATA$4,6,45,4,3,15,4,5,45,4,10,15,4,6,120 
      /* 33000 */ restore();text();print(chr$(17),PRINT_LINEFEED,0);
      /* 33002 */ BP_50000();
      /* 33005 */ for (gs.N=1;gs.N<=331;gs.N++)
         {
         data_read(data_array,total_nb_data,5,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y);
      }
      data_read(data_array,total_nb_data,3,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y,DT_STRING,gs.str_Y);
      /* 33010 */ for (gs.N=46360;gs.N<=46423;gs.N++)
         {
         
         /* 33020 */ data_read(data_array,total_nb_data,1,DT_NUMERIC,&gs.GH);poke(gs.N,gs.GH);
         /* 33030 */ 
      }
      
      /* 33040 */  //$DATA$ 1,2,4,4,4,2,3,1,48,32,24,16,32,16,32,32 
      /* 33050 */  //$DATA$ 2,6,9,8,8,8,4,6,16,16,8,40,26,12,10,16 
      /* 33060 */  //$DATA$ 7,8,8,16,16,8,28,0,48,8,8,4,4,2,3,0 
      /* 33070 */  //$DATA$ 3,4,4,6,2,2,3,0,48,24,8,4,4,4,6,0 
      /* 33100 */ for (gs.N=48802;gs.N<=48836;gs.N+=2)
         {
         
         /* 33110 */ poke(gs.N,35);poke(gs.N+1,36);poke(gs.N+40,37);poke(gs.N+41,38);poke(gs.N+80,39);poke(gs.N+81,40);
         /* 33120 */ oric_wait(48836-gs.N);
         /* 33130 */ poke(gs.N,32);poke(gs.N+40,32);poke(gs.N+80,32);
         /* 33140 */ poke(gs.N+1,35);poke(gs.N+2,36);poke(gs.N+41,37);poke(gs.N+42,38);poke(gs.N+81,41);poke(gs.N+82,42);
         /* 33150 */ oric_wait(48836-gs.N);
         /* 33160 */ poke(gs.N+1,32);poke(gs.N+41,32);poke(gs.N+81,32);
         /* 33170 */ 
      }
      
      /* 33180 */ poke(gs.N,32);poke(gs.N+40,32);poke(gs.N+80,32);
      /* 33190 */ AP_F89B();
      /* 33200 */ return;
      /* 40000 */ 

}

void BP_50000()
{
      print(0);print(" Cela est exceptionnel,vous etes le   premier a etre sorti vivant",0);
      /* 50010 */ print(" de cette",PRINT_LINEFEED,0);print(0);print("maison , mais je serais vous , je me ",PRINT_LINEFEED,0);print("mettrais a ",0);
      /* 50020 */ print("courrir car un nain rode",PRINT_LINEFEED,0);print(0);print("peut etre dans les parages",PRINT_LINEFEED,0);
      /* 50030 */ return;
      /* 51000 */ 
}

void disclaimer()
{
      cls();paper(3);ink(0);print(0);print(0);print("ATTENTION .................",PRINT_LINEFEED,0);
      /* 51005 */ print(0);print("L'utilisation de ce programme est",PRINT_LINEFEED,0);
      /* 51010 */ print(0);print("deconseillee aux personnes sensibles",PRINT_LINEFEED,0);
      /* 51020 */ print(0);print("aux enfants en bas age,ainsi qu'a",PRINT_LINEFEED,0);
      /* 51030 */ print(0);print("toute personne susceptible d'avoir",PRINT_LINEFEED,0);
      /* 51040 */ print(0);print("des malaises cardiaques.",PRINT_LINEFEED,0);
      /* 51050 */ print(0);print("Nous ne pourrions etre tenu responsa-",PRINT_LINEFEED,0);
      /* 51060 */ print(0);print("bles,des troubles physiques ou mentaux",PRINT_LINEFEED,0);
      /* 51070 */ print(0);print("provoques par votre echec dans",PRINT_LINEFEED,0);
      /* 51080 */ print(0);print("le Manoir du Dr GENIUS ............. ",PRINT_LINEFEED,0);
      /* 51099 */ oric_wait(400);
      /* 51100 */ return;
   
}
