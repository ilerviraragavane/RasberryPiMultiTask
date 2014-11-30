#include "hw.h"
#include "sched.h"
#include "malloc.h"
#include "sem.h"


int mangeurs[6]={0};
int repas=0;
struct mtx_s mutex_led;

void prendre_fourchette(int numero){
	int interblocage = mtx_lock(&mutexs[numero]);
	if( interblocage){
		//Si il ya interblocage, on le signale par un allumage long de la lampe
		
		int k=0;
	
		led_off();
		for(k=0;k<5000000 ;k++);
		
		
	}	
}
void poser_fourchette(int numero){
	mtx_unlock(&mutexs[numero]);
}

void manger (int philosophe)
{
	//On clignote autant de fois que le num du philosphe
	int i=0;
	int k =0;
	
	mangeurs[philosophe]=1;
	
	
	/*A DECOMMENTER POUR QUE RASPBERRY CLIGNOTE*/
	//DISABLE_IRQ();
	mtx_lock(&mutex_led);
	for (i=0; i<philosophe; i++){
		k=0;
		led_on();
		while ( k++<700000);
		
		k=0;
		led_off();
		while ( k++<700000);
	}
	k=0;
	while ( k++<3200000);
	mtx_unlock(&mutex_led);
	repas++;
	
	//ENABLE_IRQ();
	mangeurs[philosophe]=0;
}

void penser(void){
	int k=0;
	while (k++<120);
}
void philosopher(void)
{
	while (1){
		int philosophe = *(int*)current_process->args;
		penser();
		prendre_fourchette(philosophe-1);
		prendre_fourchette(philosophe<NB?philosophe:0);
		
		manger(philosophe);
		
		poser_fourchette(philosophe-1);
		poser_fourchette(philosophe<NB?philosophe:0);
			
	}
	
	
}
void balayer(){
	int i=0;
	while (1){
		i+=2;
		i--;
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

	
	int nbPhilo[NB] = {0};
	int i=0;
	
	
	for(i=0; i<NB; i++){
		mtx_init(&mutexs[i]);
		nbPhilo[i]=i+1;
		create_process(&philosopher, 512,&nbPhilo[i]);
	}
	mangeurs[0]=-1;
	mtx_init(&mutex_led);
	//create_process(&balayer, 512,NULL);


	start_sched();
  
  return 0;
}
