#include "process.h"
#include "allocateMemory.h"



void init_pcb(struct pcb_s* pcb, func_t f, void * args, uint32_t stack_size)
{
	unsigned int* alloue = (unsigned int*)AllocateMemory(stack_size);
	
	pcb->sp = alloue;
	pcb->pc = f;
	pcb->etat = NEW;
	pcb->args = args;
	pcb->next=pcb;  //par default, le pcb suivant est lui-même
}

