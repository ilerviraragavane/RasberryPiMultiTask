#include "./hw/hw.h"
#include "scheduler/malloc.h"
#include "./music/music.h"
#include "./scheduler/yield.h"
#include "process.h"

extern void play_music();

//------

void
processus_A( void* args )
{
	int i = 0;
	while ( 1 ) 
	{
		for(i=0;i<100000;i++);
		led_on();
		for(i=0;i<100000;i++);
		led_off();
	}
}

void
processus_B( void* args )
{
	while ( 1 ) 
	{
		led_off();
	}
}


//------------------------------------------------------------------------
int
notmain ( void )
{
  init_hw();
  music_init();
  
  DISABLE_IRQ();
  malloc_init((void *) HEAP_START);

  play_music();
  //create_process(processus_A, NULL, 512, 2);
  //create_process(processus_A, NULL, 512, 3);

  //create_process(play_music, NULL, 2048, 1);

  //create_process(processus_B, NULL, 512, 3);
  //create_process(processus_B, NULL, 512, 4);

  start_scheduler();

  return(0);
}

