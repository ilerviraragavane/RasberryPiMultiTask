#include "sched.h"
#include <stdio.h>
#include "hw.h"

void __attribute__((naked)) ctx_switch()
{
	// permet de sauvegarder le point de retour avt interruption et de basculer sur le contexte d'exécution système
	__asm volatile("sub lr, lr, #4");
	__asm volatile("srsdb sp!, #0x13");
	__asm volatile("cps #0x13");
	
	/* Sauvegarde du contexte : adresse de l'instruction en cours + pointeur de pile */
	// Il faut d'abord pusher les registres car la commande mov "salit" les registres
	__asm volatile("push    {r0-r12, lr}");

	  /* Disable interrupts */
  	DISABLE_IRQ();

	__asm("mov %0, sp" : "=r"(current_pcb->sp));


	
	/* Changement de contexte */
	//current_pcb->etat=READY; //l'ancien processus repasse en état Ready
	schedule();
	current_pcb=next_running;	

	/* Restauration du nouveau contexte */

	__asm("mov sp, %0" : : "r"(current_pcb->sp));

	set_next_tick_and_enable_timer_irq();
	ENABLE_IRQ();
	
	// Si c'est la 1ère fois que le processus est appelé, on appele start()
	if(current_pcb->etat == NEW){
		// start_current_process();
		start_current_process();
	}
	else {
		__asm volatile( "pop {r0-r12, lr}" ); // on ne restitue les registres que si le process n'est pas nouveau 	
	}
		
		
	__asm volatile ("rfefd sp!");
	
}



