#include "oric_basic.h"
#include "oric_video.h"
#include "oric_sound.h"
#include "oric_machine.h"
#include "oric_timer.h"

#include "key_mapping.h"

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>

#ifdef __APPLE__
#include "SDL.h"
#include <dlfcn.h>
#endif

#undef main

void basic_entrypoint(int cheat,int language);
void demo_basic_entrypoint();


static void unmap_keys()
{
    get_key_mapping()->nb_mapping = 0;

}


static int map_key(char x)
{
  int kc=read_keycode();
  if (kc!=27)
    {
  get_key_mapping()->keys[get_key_mapping()->nb_mapping].keycode=kc;
  
      get_key_mapping()->keys[get_key_mapping()->nb_mapping++].mapped=x;
    }
  return (kc!=27);
}
static void map_keys()
{
  get_key_mapping()->nb_mapping = 0;
  cls();
  while(1)
    {
  print(NULL);
      print("Press the '1' key on your keyboard (used for selecting torches)",NULL);
      if (map_key('1')) break;
    }
  
  print(NULL);
  while(1)
    {
  print(NULL);
      print("Press the '2' key on your keyboard (used for selecting the iron key)",NULL);
      if (map_key('2')) break;
    }
  print(NULL);
  while(1)
    {
  print(NULL);
      print("Press the '3' key on your keyboard (used for selecting the golden key)",NULL);
      if (map_key('3')) break;
    }
  print(NULL);
  while(1)
    {
   print(NULL);
     print("Press the '4' key on your keyboard (used for selecting potions)",NULL);
      if (map_key('4')) break;
    }
  
  print(NULL);
  while(1)
    {
   print(NULL);
     print("Press the '5' key on your keyboard (used for selecting crowbars)",NULL);
      if (map_key('5')) break;
    }
  
  print(NULL);
  while(1)
    {
  print(NULL);
  print("Press the '0' key on your keyboard (used for emptying selection)",NULL);
 if  (map_key('0')) break;
    }
  
  print(NULL);
  while(1)
    {
   print(NULL);
 print("Press the key for crouching",NULL);
      if (map_key('A')) break;
    }
  
  print(NULL);
  while(1)
    {
  print(NULL);
      print("Press the key for drinking",NULL);
      if (map_key('B')) break;
    }
  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for long jump",NULL);
      if (map_key('S')) break;
    }
  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for high jump",NULL);
      if (map_key('D')) break;
    }
  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for rope climbing",NULL);
      if (map_key('G')) break;
    }
  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for inventory",NULL);
      if (map_key('I')) break;
    }
  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for reading",NULL);
      if (map_key('L')) break;
    }
  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for opening",NULL);
      if (map_key('O')) break;
    }
					  
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for taking",NULL);
      if (map_key('P')) break;
    }
    
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for quitting/suicide",NULL);
      if (map_key('Q')) break;
    }
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for saving game",NULL);
      if (map_key('W')) break;
    }
  print(NULL);
  while(1)
    {
      print(NULL);
      print("Press the key for loading game",NULL);
      if (map_key('R')) break;
    }
							     
  set_key_mapping(get_key_mapping()->keys, get_key_mapping()->nb_mapping);
  // save mapping
  
}


void quit_callback()
{

    //save_hiscore();

    sound_close();
}

int main(int argc, char **argv)
{
  VideoParams vp;
  int c;
  int cheat = 0;
  int intro = 1;
  int language=0;
  T_STRING k;
#ifdef __APPLE__
    void* cocoa_lib;
    
    cocoa_lib = dlopen( "/System/Library/Frameworks/Cocoa.framework/Cocoa", RTLD_LAZY );
    void (*nsappload)(void);
    nsappload = (void(*)()) dlsym( cocoa_lib, "NSApplicationLoad");
    nsappload();
#endif

  memset(&vp,0,sizeof(vp));

  vp.mask_color = -1;  
  vp.window_title = "L'Aigle d'or V1.0";
  vp.update_rate = 30;
  vp.double_size = 1;
  vp.draw_only_changes = 1;
  vp.extended_colors = 1;
  
  set_pen(8,0xd2691e); // chocolate
  set_pen(9,0x000090); // dark blue
  set_pen(10,0xC0C0FF); // gray
  set_pen(11,0x009999); // teal
  set_pen(12,0xFFA500); // orange
  while ((c = getopt (argc, argv, "csfn")) != -1)
    {
         switch (c)
           {
	     case 's':
	       vp.double_size = 0;
	       break;
	     case 'c':
	       cheat = 1;
	       break;
	     case 'n':
	       intro = 0;
	       break;
	     case 'f':
	       vp.full_screen = 1;
	       break;
	     case '?':
	       fprintf (stderr, "Unknown option `-%c'.\n", optopt);
	       exit(1);
	   }
    }

    vp.quit_callback = &quit_callback;

  cpu_init();
  basic_init();
 
  
  video_init(&vp);
  paper(4);ink(7);
  load_key_mapping();

  while(1)
    {
      cls();
      print(NULL);
      print(NULL);
      print("1. FRANCAIS",NULL);
      print(NULL);
      print(NULL);
      print("2. ENGLISH",NULL);
      print(NULL);
      print(NULL);
      print("K. KEY MAPPING",NULL);
      print(NULL);
      print(NULL);
      if (get_key_mapping()->nb_mapping)
	{
	  print("ESC. CLEAR MAPPING",NULL);
	  print(NULL);
	  print(NULL);
	}
      
      print("IF PRESSING 1 or 2 DOES NOT WORK",NULL);
      print(NULL);
      print("PLEASE DO A KEY MAPPING.",NULL);
      get(k);
      if (k[0]=='1')
	{
	  language = 0;
	  break;
	}
      else if (k[0]=='2')
	{
	  language = 1;
	  break;
	} 
      else if (k[0]=='K') {
	map_keys();
	save_key_mapping();
      } 
      else if (k[0]==27) {
	print(NULL);
	print(NULL);
	print("MAPPING CLEARED",NULL);
	oric_wait(100);
	unmap_keys();
	save_key_mapping();
      } 
      
    }
  
  
  if (intro)
    {
      sound_init(1);
      demo_basic_entrypoint(language);
      play(0,0,0,0);
    }
  else
    {
      sound_init(0);
    }
  
  basic_entrypoint(cheat,language);


    return 0;
}
