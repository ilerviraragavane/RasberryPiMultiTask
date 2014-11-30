#include "sched.h"
#include <stdlib.h>
#include <stdio.h>
#include "allocateMemory.h"
#include "hw.h"

struct pcb_s idle;
struct pcb_s* first = (struct pcb_s *) NULL;

const int TIME_SLICE[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};


void init_pcb(struct pcb_s* pcb, func_t f, void * args, unsigned int stack_size, int priority)
{
	unsigned int* alloue = (unsigned int*) AllocateMemory(stack_size);
	
	pcb->sp = alloue;
	pcb->pc = f;
	pcb->etat = NEW;
	pcb->args = args;
	pcb->next=pcb;  //par default, le pcb suivant est lui-même
	
	/*Set Priority and time_slice*/
  pcb->priority = priority;
  pcb->time_slice = 0;
}

void create_process(func_t f, int stack_size, int priority)
{
	struct pcb_s* new_pcb = (struct pcb_s*) AllocateMemory(sizeof(struct pcb_s));

	init_pcb(new_pcb, f, NULL, stack_size, priority);
	
	if( first == NULL){
		first = new_pcb; 
		first->next = first;
	}
	else{
		struct pcb_s* temp = first->next;
		new_pcb->next = first;
		new_pcb->etat=NEW;

		//Recherche de l'ancien dernier pcb
		while(temp->next != first)
		{
			temp = temp->next;
		}

		//On ajoute le nouveau pcb a la fin de la liste
		temp->next = new_pcb;
		first = new_pcb;
	}
}

/* On remet le time_slice à 0 
 * si tous les processus ont dépassé leur quantum*/
void refresh_time_slice(struct pcb_s* f)
{
	struct pcb_s *pcb;
	pcb = f->next;
	
	while(pcb != f && pcb->time_slice > TIME_SLICE[pcb->priority])
		pcb = pcb->next;
	
	if(pcb == f && f->time_slice > TIME_SLICE[f->priority])
	{
		pcb = f->next;
		f->time_slice = 0;
		while(pcb != f)
		{
			pcb->time_slice = 0;
			pcb = pcb->next;
		}			
	}
}

void schedule()
{

	struct pcb_s * temp = first;
	 struct pcb_s *priority_pcb = first;
	
	// on supprime de la liste des process
	while(temp->next != first){
		
		if(temp->next->etat == TERMINATED)
		{
			//Sauvegarde du suivant du process à supprimer
			struct pcb_s* nouveau_suivant = temp->next->next;
			
			// on désalloue
			FreeAllocatedMemory((uint32_t*) temp->next->sp);
			FreeAllocatedMemory((uint32_t*) temp->next);
			
			//On met a jour l'enchainement de la liste
			temp->next = nouveau_suivant;
		}
		else
		{
			if((temp->next->etat != WAITING 
				&& temp->next->priority >= priority_pcb->priority 
				&& temp->next->time_slice < TIME_SLICE[temp->next->priority])
				|| (priority_pcb->time_slice > TIME_SLICE[priority_pcb->priority]))
			{
				priority_pcb = temp->next;
			}
        	temp = temp->next;
		}
		
	}
		
	if(priority_pcb == current_pcb)
	{
		priority_pcb->time_slice++;
	}

	//ON lance le processus suivant
	next_running = priority_pcb;

	refresh_time_slice(first);
}

 
/* Lance un processus pour la première fois
*/
void start_current_process()
{	

	current_pcb->etat= READY;

	current_pcb->pc();

	//On indique que le pprocessus est terminé
	current_pcb->etat=TERMINATED;
	//On fait un switch manuel sur le prochain processus lorsque que l'on termine le processus
	ctx_switch();

}

void start_sched(){
	current_pcb = &idle;
  	idle.next = first;

	ENABLE_IRQ();

	while(1)
	{
		// pour attendre les interruptions
	}
}
	
