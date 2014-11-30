#include "yield.h"
#include "allocateMemory.h"

struct pcb_s* current_pcb;

void yield()
{
	// stocker les registres dans le stack qui est pointe par sp
	__asm volatile( "push {r0-r12, lr}" );
	// mettre dans current_pcb.SP la valeur de sp
	__asm( "mov %0 , sp" : "=r"(current_pcb->SP) );

	// si le processus suivant le prochain est terminated je l enleve de l ordonnanceur et je nettoie la memoire
	while(current_pcb->next_running ->process_state == TERMINATED)
	{
                // on conserve un pointeur sur notre PCB à détruire
                pcb_s* pcb_destruct = current_pcb->next_running;
                // on retire le pcb à détruire de la boucle et la referme
                current_pcb->next_running = pcb_destruct->next_running;
                // On détruit les élément du pcb_destruct
                FreeAllocatedMemory(pcb_destruct);
	}

	// faire pointer le contexte courant sur pcb
	current_pcb = current_pcb->next_running;
	// aller a la ligne pcb.SP
	__asm( "mov sp, %0" : : "r"(current_pcb->SP) );

	// si le contexte est NEW on le rempli
	if (current_pcb->process_state == NEW)
	{
		// changer l etat du processus
		current_pcb->process_state = RUNNING;
		// on lance la fonction associee à current_pcb
		current_pcb->function(current_pcb->args);	
		// si jamais la fonction fini à un jour
		current_pcb->process_state = TERMINATED;
		yield();	
	}

	// sinon on pop
	else
	{
		// recuperer de la pile, qui est pointee par sp, le contenu des registres
		__asm volatile( "pop {r0-r12, lr}" );
	}
}

void ctx_switch()
{
	yield();
}
