#include "hw.h"
#include "sched.h"
#include "malloc.h"
#include "sem.h"


void processA (void)
{
	int k=0;
	
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
	 while(i++<200000);
	}
}

void processB (void)
{
  while (1) {
	led_on();
	int i = 0;
	while(i++<600000);
	led_off();
	i = 0;
	while(i++<600000);
	}
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
start_kernel ( void )
{
  DISABLE_IRQ();
  init_hw();
  malloc_init((void *) HEAP_START);

	
  create_process(&processA, 512, 5);
  create_process(&processB, 512, 0);
  
  start_sched();
  
  return 0;
}
