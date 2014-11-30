#include "./hw/hw.h"
//#include "./scheduler/malloc.h"
#include "./music/music.h"
#include "./scheduler/yield.h"
#include "process.h"

extern void play_music();

//------

void
processus_A( void* args )
{
  while ( 1 ) {
    led_on();
    int i = 0;
    while(i++<2000000);
    yield();
  }
}

void
processus_B( void* args )
{
  while ( 1 ) {
    led_off();
    int i = 0;
    while(i++<2000000);
    yield();
  }
}


//------------------------------------------------------------------------
int
notmain ( void )
{
  init_hw();
  music_init();
  
  DISABLE_IRQ();
  //malloc_init((void *) HEAP_START);

  /* hw_init(); */
  /* led_init(); */
  /* switch_init(); */

  //play_music();
  create_process(processus_A, NULL, 512);
  //create_process(play_music, NULL, 2048);
  create_process(processus_B, NULL, 512);

  start_scheduler();

  return(0);
}

