#include "sched.h"
#include "hw.h"
#include <stdlib.h>
#include "malloc.h"


void
funcA()
{
  while ( 1 ) {
    led_on();
    int cptA = 0;
    while (cptA < 2000000) {
		cptA ++;
	}
    yield();
  }
}

void
funcB()
{

  while ( 1 ) {
    led_off();
	int cptB = 0;
    while (cptB < 2000000) {
		cptB ++;
	}
    yield();
  }
}

void
funcC()
{

  while ( 1 ) {
    led_on();
	int cptC = 0;
    while (cptC < 2000000) {
		cptC ++;
	}
    yield();
  }
}

void
funcD()
{

  while ( 1 ) {
    led_off();
	int cptD = 0;
    while (cptD < 2000000) {
		cptD ++;
	}
    yield();
  }
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  DISABLE_IRQ();
  init_hw();
  malloc_init((void *) HEAP_START);
	
  create_process(&funcA,512);
	create_process(&funcB, 512);
	
	create_process(&funcC,512);
	create_process(&funcD, 512);

	start_sched();

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
