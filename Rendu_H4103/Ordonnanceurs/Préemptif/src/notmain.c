/*#include "process.h"*/
#include "sched.h"
#include <stdlib.h>
#include <stdio.h>
#include "hw.h"
#include <unistd.h>

struct pcb_s ctx_A;
struct pcb_s ctx_B;


void
double_led()
{
  while (1) {
	 led_on();
	 int i = 0;
	 while(i++<200000);
	 i = 0;
	 led_off();
	 while(i++<200000);
	 led_on();
	 i = 0;
	 while(i++<200000);
	 led_off();
	 i = 0;
	 while(i++<4000000);
  }
}


void
simple_led()	
{
  while (1) {
	led_on();
	int i = 0;
	while(i++<400000);
	led_off();
	i = 0;
	while(i++<4000000);
  }
}

//------------------------------------------------------------------------
int
notmain ( void )
{
	DISABLE_IRQ();
   
    init_hw();

	create_process(1024, double_led, NULL);
	create_process(1024, simple_led, NULL);
	create_process(1024, double_led, NULL);
	create_process(1024, simple_led, NULL);

	start_sched();
	
  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
