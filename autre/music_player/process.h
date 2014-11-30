#ifndef PROCESS_H

#include <stdint.h>
#include <stdlib.h>
#include "./scheduler/allocateMemory.h"

#define PROCESS_H

typedef void (*func_t) (void*); 
typedef enum state {NEW, RUNNING, TERMINATED} state;
 
// structure de la pile avec pointeur sur le suivant pour faire une boucle (ordonnancement)
typedef struct pcb_s
{
	uint32_t* SP; //SP
	enum state process_state; //etat du processus
	struct pcb_s* next_running;
	func_t function; //fonction que le processus va exécuter
	void* args; // les arguments de la fonction
}pcb_s;

pcb_s* create_process(func_t f, void* argument, uint32_t stack_size);

void start_scheduler();

#endif
