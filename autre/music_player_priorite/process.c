#include "process.h"
#include "scheduler/malloc.h"
#include "hw.h"
#include "./scheduler/yield.h"

struct pcb_s* ready_queue = (struct pcb_s *)0; // liste de tous les pcb
struct pcb_s* current_pcb = (struct pcb_s *) NULL; // le pcb courant, en cours
struct pcb_s idle;

extern void producteur();

#define PRINT(MSG) ;
#define EXIT(CODE) ;

int create_process(func_t f, void* argument, unsigned size, int priorite)
{
	// CREER LE PCB ET L AJOUTER A READY QUEUE
	// on alloue de la mémoire pour le PCB
	struct pcb_s* pcb;
	pcb = (struct pcb_s*) malloc_alloc(sizeof(struct pcb_s));

	if(!pcb)
	{
		return 0;
	}
	
	if (! ready_queue) 
	{
		// premier process de la ready queue
		ready_queue = pcb;
	} else 
	{
		pcb->next_running = ready_queue->next_running;
	}
	ready_queue->next_running = pcb;
	
	
	// INITIALISER LE PCB
	// la priorite des processus
	pcb->priorite = priorite;

	// sa fonction et les arguments associés
	pcb->function = f;
	pcb->args = argument;
	
	// on alloue la mémoire pour la pile
	pcb->size = size;
	pcb->stack_base = malloc_alloc(size);	
	if(!pcb->stack_base)
    {
		return 0;
	}
	
	// etat et contexte de la PCB
	pcb->process_state = NEW;
	pcb->SP = ((uint32_t*) (pcb->stack_base + size)) - 1; // /!\ la pile rajoute des elements par le bas, donc il faut mettre SP en faut de la pile, sinon on en pile dans le vide !!
	
	// remplir la pile avec CPSR et PC
	*(pcb->SP) = 0x53;
	pcb->SP --;
	*(pcb->SP) = (unsigned int) &start_current_process;

	return 1;
}


void start_current_process()
{
	// si le contexte est NEW on le rempli
	// changer l etat du processus
	current_pcb->process_state = RUNNING;
	// on lance la fonction associée à current_pcb
	current_pcb->function(current_pcb->args);	
	// si jamais la fonction fini un jour
	current_pcb->process_state = TERMINATED;
	ctx_switch();
}


void start_scheduler()
{
	current_pcb = &idle;
	idle.next_running = ready_queue;
	idle.priorite = 1337;

	ENABLE_IRQ();

	ctx_switch();
}


void schedule()
{
	struct pcb_s* pcb;
	struct pcb_s* pcb_init;
	struct pcb_s* pcb_best;
	
	pcb_init = current_pcb;
	pcb = pcb_init->next_running;
  
	// si le processus suivant le prochain est terminated je l enleve de l ordonnanceur et je nettoie la memoire
	while(current_pcb->next_running ->process_state == TERMINATED) 
	{
		if (pcb->next_running == pcb) 
		{
			pcb = NULL;
			break;
		}
		else 
		{
			// cas particulier où il ne restait qu un element dans la ready_queue
			if (pcb->next_running == ready_queue)
			{
				ready_queue = pcb;    
			}

			// supprimer le PCB de la liste
			pcb->next_running = pcb->next_running->next_running;

			// liberer la memoire
			malloc_free((char*) pcb->next_running->stack_base);
			malloc_free((char*) pcb->next_running);

			// aller au processus suivant
			pcb = pcb->next_running;
			}
	}

	
	if (pcb != NULL) 
	{
		// On parcours la liste jusqu a trouver un processus non bloque
		pcb = pcb->next_running;    
		while(pcb->process_state == WAITING && pcb != pcb_init)
		{
			pcb = pcb->next_running;
		}
		
		// Si tous les processus sont bloques -> "on le note"
		if(pcb->process_state == WAITING)
		{
		  pcb = NULL;
		}
		else
		{
			pcb_best = pcb;
			pcb_init = pcb_best;
			pcb = pcb_init->next_running;
			// compare tous les pcb sauf pcb_init car il sort du while
			while(pcb != pcb_init)
			{
				if(pcb->priorite < pcb_best->priorite)
				{
					if(pcb->process_state != WAITING)
					{
						pcb_best = pcb;
					}
				}
				pcb = pcb->next_running;			
			}
		}
	}

	
	if(pcb_best == NULL) 
	{   
		// Si pas de processus à élire -> idle
		ready_queue = NULL;
		current_pcb = &idle;
	} 
	else 
	{            
		// Sinon -> le processus élu est le suivant
		if(pcb_best->priorite == 1 && pcb_best->process_state == RUNNING)
		{
			pcb_best->process_state = WAITING;
		}
		else 
		{
			pcb_init = current_pcb->next_running;
			pcb = pcb_init->next_running;
			while(pcb != pcb_init)
			{
				if(pcb->process_state == WAITING)
				{
					pcb->process_state = RUNNING;
				}
				pcb = pcb->next_running;
			}			
		}
		current_pcb = pcb_best;	
	}
}


