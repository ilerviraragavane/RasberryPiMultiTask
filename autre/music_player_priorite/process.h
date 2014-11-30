#ifndef PROCESS_H
#include <stdint.h>
#include <stdlib.h>
#define PROCESS_H

extern struct pcb_s* ready_queue;
extern struct pcb_s* current_pcb;

typedef void (*func_t) (void*); 
typedef enum state {NEW, RUNNING, TERMINATED, WAITING} state;
 
// structure de la pile avec pointeur sur le suivant pour faire une boucle (ordonnancement)
typedef struct pcb_s
{
	uint32_t* SP; // Pointeur sur la pile
	
	enum state process_state; //etat du processus
	struct pcb_s* next_running; // pcb_s suivant
	
	func_t function; //fonction que le processus va exécuter
	void* args; // les arguments de la fonction	
	
	unsigned int size; // taille de la pile associée
	char* stack_base; // adresse de l'entrée de la pile

	int priorite;
}pcb_s;


int create_process(func_t f, void* argument, unsigned size, int priorite);
void start_current_process();


void start_scheduler();
void schedule();


#endif
