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
	ENABLE_IRQ();
	
	
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
	//set_next_tick_and_enable_timer_irq();
	ENABLE_IRQ();
	//sinon pas de processus en attente -> on fait rien
}


void mtx_init(struct mtx_s* mutex){
	//Le mutex est un sémaphore initialisé à 1
	mutex->disponible = 1;
	mutex->proprietaire=NULL;
    mutex->list_wait_pcb = NULL;
}

int rechercheTousBloques(){
	struct pcb_s* temp = current_process;
	do{
		if(temp->state !=WAITING){
			return 0;
		}
		temp=temp->next;
	}
	while(temp != current_process );
	return 1;
}
/**
 * 
 * @return 0 si non trouvé, 1 sinon
 * */
int recherche(struct pcb_s* pcb_courant, struct pcb_s* pcb_recherche){
	int i=0;
	for (i=0; i<NB;i++){
		
		if(mutexs[i].proprietaire == pcb_courant){
			struct wait_list* waiting = mutexs[i].list_wait_pcb;
			while (waiting!=NULL){
				if(waiting->pcb == pcb_recherche ){
					return 1;
				}
				waiting = waiting->next;
			}
			
		}
		
	}
	return 0;
}

/** Recherche d'un processus dans la file d'attente d'un mutex
 * @return 1 si trouvé 0 sinon
 * */
int rechercheDansFileAttente(struct mtx_s* mutex,struct pcb_s* pcb_recherche){
	struct wait_list* waiting = mutex->list_wait_pcb;
	while (waiting!=NULL){
		if(waiting->pcb == pcb_recherche ){
				return 1;
		}
		waiting = waiting->next;
	}
	return 0;
}
 
/**
 * 
 * @return 0 si non trouvé, 1 sinon
 * */
int recherche2(struct pcb_s* pcb_courant, struct pcb_s* pcb_recherche){
	if(pcb_recherche->state==WAITING){
	int i=0;
	for (i=0; i<NB;i++){
		//On regarde si le processus est en attente chez le demandeur du verrou
		if(mutexs[i].proprietaire == pcb_courant){
			if( rechercheDansFileAttente(&mutexs[i],pcb_recherche)){
				return 1;
			}
			
		}
		//On regzrde si le processus est indirectement en attente
		else if (mutexs[i].proprietaire->state == WAITING){
			if(rechercheDansFileAttente(&mutexs[i],pcb_recherche)){
				return recherche2(pcb_courant,mutexs[i].proprietaire);
			}
		}
		
	}
}
	return 0;
}
/**
 * Retourne 0 si pas de problème et 1 si un interblocage a été détecté
 * */
int mtx_lock(struct mtx_s* mutex){
	
	int interblocage =0;
	DISABLE_IRQ();
	
	//si le mutex est locked
	if(mutex->disponible == 0 && current_process!=mutex->proprietaire)
	{	
		//Ajout d'un processus à la liste des processus en attente
		//l'ajout se fait toujours en tete de liste
		struct wait_list* list = (struct wait_list*) malloc_alloc(sizeof(struct wait_list));
		list->pcb = current_process;
		list->next = mutex->list_wait_pcb;
		mutex->list_wait_pcb = list;
		
		current_process->state = WAITING;
		
		/* DETECTION DE L'INTERBLOCAGE*/
		interblocage= recherche2(current_process,mutex->proprietaire);
		interblocage = interblocage || rechercheTousBloques();
		if( interblocage){
			return 1;
		}
		else{
			//Sauvegarde du pcb qui attend car current_process change au cours du temps
			struct pcb_s* the_waiting_pcb = current_process;
			ENABLE_IRQ();
			while(the_waiting_pcb->state == WAITING);
			DISABLE_IRQ();
		}
	}
	//ENABLE_IRQ();
	
	
	//On actualise le statut du verrou
	mutex->proprietaire=current_process;
	mutex->disponible=0;
	ENABLE_IRQ();
	return 0;
	
}
void mtx_unlock(struct mtx_s* mutex){
	DISABLE_IRQ();
	struct wait_list* temp;
	
	//Seul le propriétaire peut libérer les process en attente
	if( current_process==mutex->proprietaire ){
		//On supprime le proprietaire
		mutex->proprietaire=NULL;
		mutex->disponible=1;
	
		// il reste des processus en attente
		if(mutex->list_wait_pcb!=NULL )
		{

			mutex->list_wait_pcb->pcb->state = READY;
			temp = mutex->list_wait_pcb; //sauvegarde pour libérer la mémoire
			//on pointe vers le prochain process en attente
			mutex->list_wait_pcb = mutex->list_wait_pcb->next;
			
			//libération de la mémoire
			malloc_free(temp);
		}
	}
	//set_next_tick_and_enable_timer_irq();
	ENABLE_IRQ();
	//sinon pas de processus en attente -> on fait rien
	
	
}

