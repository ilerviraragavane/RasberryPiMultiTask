#include "process.h"
#include "sched.h"
#include "hw.h"
#include <stdlib.h>
#include "malloc.h"

#define STACK_SIZE 128


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

//------------------------------------------------------------------------
int
notmain ( void )
{
  malloc_init((void *) HEAP_START);
	create_process(funcA,NULL);
	create_process(funcB, NULL);
	start_sched();

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
