#include "process.h"
#include "./scheduler/yield.h"

struct pcb_s* ready_queue; // liste de tous les pcb
struct pcb_s* current_pcb; // le pcb courant, en cours

pcb_s* create_process(func_t f, void* argument,uint32_t stack_size)
{
	// on alloue de la mémoire pour le PCB (ordonnanceur)
	struct pcb_s* pcb = (pcb_s*)AllocateMemory(sizeof(int));

	// on alloue la mémoire pour la pile
	uint32_t * stack = AllocateMemory(stack_size);

	// on créé un processus
	// /!\ la pile rajoute des elements par le bas, donc il faut mettre SP en faut de la pile, sinon on en pile dans le vide !!
	pcb->SP = &stack[stack_size -1];
	pcb->process_state = NEW;
	pcb->function = f;
	pcb->args = argument;

	// on ajoute le process à l ordonnanceur
	if(ready_queue != NULL)
	{
		pcb->next_running = ready_queue->next_running;
		ready_queue->next_running = pcb;
	}
	// si c'est le tout premier element quon ajoute
	else
	{
		ready_queue = pcb;
		ready_queue->next_running = pcb;
	}

	return pcb;
}

void start_scheduler()
{
	// je démarre
	pcb_s dummy_pcb;
	uint32_t dummy_stack[20];
	current_pcb = &dummy_pcb;
	current_pcb->SP = &dummy_stack[19];
	current_pcb->next_running = ready_queue;
	
	// pour les diodes du Rasberry
	init_hw();
	
	yield();	
}
