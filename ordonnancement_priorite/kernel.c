#include "hw.h"
#include "sched.h"
#include "malloc.h"
#include "sem.h"


void processA (void)
{
	int k=0;
	
	while (1) {
		k=0;
		while(k++ < 200000);
		
		led_on();
		
		k=0;
		while(k++ < 2000000);
	}
}

void processB (void)
{
	int k=0;
	
	while (1) {
		k=0;
		while(k++ < 200000);
		
		led_off();
		
		k=0;
		while(k++ < 2000000);
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

	
  create_process(&processA, 512, 1);
  create_process(&processB, 512, 0);
  
  start_sched();
  
  return 0;
}
