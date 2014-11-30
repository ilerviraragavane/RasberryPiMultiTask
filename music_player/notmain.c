#include "hw/hw.h"
#include "music/music.h"
#include "scheduler/sched.h"

extern void play_music();

//------

void
processus_A( )
{
  while ( 1 ) {
    int i = 0;
    while(i++<100000);
      led_on();
    i=0;
    while(i++<100000);
      led_off();
  }
}

void
processus_B( )
{
  while ( 1 ) {
    led_off();
  }
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  DISABLE_IRQ();
  hw_init();
  music_init();

  /* switch_init(); */

  //play_music();
  create_process(processus_A, 512,0);

  create_process(play_music, 2048,9);

  create_process(processus_A, 512,0);

  start_sched();

  return(0);
}

