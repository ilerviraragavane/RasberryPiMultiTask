#include "sem.h"
#include "malloc.h"
#include "hw.h"
#include <stdio.h>

void sem_init(struct sem_s* sem, unsigned int val)
{
    sem->val = val;
    sem->list_wait_pcb = NULL;
}

void sem_down(struct sem_s* sem)
{
	DISABLE_IRQ();
	sem->val--;
	//si plus de jetons disponibles on place le processus en attente
	if(sem->val < 0)
	{
		//Ajout d'un processus à la liste des processus en attente
		//l'ajout se fait toujours en tete de liste
		struct wait_list* list = (struct wait_list*) malloc_alloc(sizeof(struct wait_list));
		list->pcb = current_process;
		list->next = sem->list_wait_pcb;
		sem->list_wait_pcb = list;
		
		current_process->state = WAITING;
		//Sauvegarde du pcb qui attend car current_process change au cours du temps
		struct pcb_s* the_waiting_pcb = current_process;
		
		ENABLE_IRQ();
		while(the_waiting_pcb->state == WAITING);
		
	}
	
}

void sem_up(struct sem_s* sem)
{
	DISABLE_IRQ();
	struct wait_list* temp;
	sem->val++;
	
	// il reste des processus en attente
	if(sem->val < 1)
	{
		sem->list_wait_pcb->pcb->state = READY;
		temp = sem->list_wait_pcb; //sauvegarde pour libérer la mémoire
		//on pointe vers le prochain process en attente
		sem->list_wait_pcb = sem->list_wait_pcb->next;
		
		//libération de la mémoire
		malloc_free(temp);
	}
	ENABLE_IRQ();
	//sinon pas de processus en attente -> on fait rien
}

