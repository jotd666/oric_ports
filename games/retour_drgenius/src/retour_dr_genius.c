#include "oric_basic.h"
#include "oric_video.h"
#include "oric_sound.h"
#include "oric_machine.h"
#include "oric_timer.h"

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>


#undef main

void basic_entrypoint();

void quit_callback()
{

    //save_hiscore();

    sound_close();
}

int main(int argc, char **argv)
{
    VideoParams vp;
    int c;
    //int enhanced = 0;

    memset(&vp,0,sizeof(vp));

  vp.mask_color = -1;
  vp.window_title = "Return of Dr Genius V0.1";
  vp.update_rate = 30;
  vp.double_size = 1;
  vp.draw_only_changes = 1;
  
  while ((c = getopt (argc, argv, "sf")) != -1)
    {
         switch (c)
           {
	     case 's':
	       vp.double_size = 0;
	       break;
/*	     case 'e':
	       enhanced = 1;
	       vp.double_size = 1;
	       break;*/
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
 
  
  video_init(&vp);

  sound_init(1);
  basic_entrypoint();


    return 0;
}
