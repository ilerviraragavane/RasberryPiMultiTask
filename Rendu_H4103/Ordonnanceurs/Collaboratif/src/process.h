#include "malloc.h"
#include <stdlib.h>
#include <stdint.h>

#ifndef PROCESS_H
#define PROCESS_H

typedef void (*func_t)(void);

typedef enum {NEW, READY, RUNNING, WAITING, TERMINATED} State;

typedef struct pcb_struct {
	State state;
	func_t entry_point;
	uint32_t* sp;
	struct pcb_struct* next;
	void* args;
} pcb_s;

pcb_s* current_process;
pcb_s* next_running;

#endif

void ctx_switch();

void init_pcb(pcb_s* pcb, func_t f, unsigned int stack_size, void* args);

void start_pcb(pcb_s* pcb, func_t f);
