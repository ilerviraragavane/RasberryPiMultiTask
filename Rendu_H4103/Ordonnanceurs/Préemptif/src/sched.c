#include "sched.h"
#include <stdlib.h>
#include <stdio.h>
#include "allocateMemory.h"
#include "hw.h"

struct pcb_s idle;
struct  pcb_s* first = (struct pcb_s *) NULL;


void create_process(int stack_size,func_t f, void* args)
{
	struct pcb_s* new_pcb = (struct pcb_s*) AllocateMemory(sizeof(struct pcb_s));

	init_pcb(new_pcb, f, args, stack_size);
	
	if( first == NULL){
		first = new_pcb; 
		first->next = first;
	}
	else
	{
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
	idle.next = first;
}


void schedule()
{
	// S'il existe des processus
	if (first != NULL)
	{

		struct pcb_s * temp;
		if (current_pcb == &idle)
		{
			temp = first;
		}
		else
		{
			temp = current_pcb;
		}
		
		// On garde le point de départ pour savoir quand on a fait une boucle
		struct pcb_s* temp_init = temp;

		// On passe les processus à ne pas exécuter
		while( (temp->next->etat == TERMINATED || temp->next->etat == WAITING)
				&& temp->next != temp_init )
		{
			// Si le process est terminé
			if (temp->next->etat == TERMINATED)
			{
				// Si c'est le premier de la liste, il faut déplacer le 
				//	pointeur de idle en plus de libérer les ressources
				if (temp->next == first)
				{
					// S'il n'y a plus qu'un processus
					if(first->next == first)
					{
						// on désalloue
						FreeAllocatedMemory((uint32_t*)first->sp);
						FreeAllocatedMemory((uint32_t*)first);
						first = NULL;
						idle.next = first;
					}
					else
					{
						struct pcb_s* nouveau_first = first->next;
						// on désalloue
						FreeAllocatedMemory((uint32_t*)first->sp);
						FreeAllocatedMemory((uint32_t*)first);
						
						first = nouveau_first;
						temp->next = first;
						idle.next = first;
					}
					next_running = &idle;
				}
				else
				{			
					//Sauvegarde du suivant du process à supprimer
					struct pcb_s* nouveau_suivant = temp->next->next;
					
					// on désalloue
					FreeAllocatedMemory((uint32_t*)temp->next->sp);
					FreeAllocatedMemory((uint32_t*)temp->next);
					//On met a jour l'enchainement de la liste
					temp->next = nouveau_suivant;
				}
			}
			// Sinon le processus est dans l'état WAITING, 
			//	donc on passe simplement au suivant
			else
			{
				temp = temp->next;
			}
		}

		// Avec la boucle précédente, on ne gère pas le cas 
		//	où le processus courant est terminé ou en attente
		if (temp->next == temp_init && temp_init->etat != READY && temp_init != NEW)
		{
			if (temp_init->etat == TERMINATED)
			{
				if (temp_init == first)
				{
					if(first->next == first)
					{
						// on désalloue
						FreeAllocatedMemory((uint32_t*)first->sp);
						FreeAllocatedMemory((uint32_t*)first);
						first = NULL;
						idle.next = first;
					}
					else
					{
						struct pcb_s* nouveau_first = first->next;
						// on désalloue
						FreeAllocatedMemory((uint32_t*)first->sp);
						FreeAllocatedMemory((uint32_t*)first);
						
						first = nouveau_first;
						temp->next = first;
						idle.next = first;
					}
					next_running = &idle;
				}
				else
				{			
					//Sauvegarde du suivant du process à supprimer
					struct pcb_s* nouveau_suivant = temp_init->next;
					
					// on désalloue
					FreeAllocatedMemory((uint32_t*)temp_init->sp);
					FreeAllocatedMemory((uint32_t*)temp_init);
					//On met a jour l'enchainement de la liste
					temp->next = nouveau_suivant;
				}
			}
			next_running = &idle;
		}
		else
		{
			//ON lance le processus suivant
			next_running = temp->next;
		}

	}
	else
	{
		next_running = &idle;
	}
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
	
