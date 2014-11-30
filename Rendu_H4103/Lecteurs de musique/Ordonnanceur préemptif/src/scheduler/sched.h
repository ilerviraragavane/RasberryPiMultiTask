#include "dispatcher.h"

#define NB_REG 14

typedef enum Etat {NEW, READY, RUNNING, WAITING, TERMINATED} Etat;
typedef void (*func_t)(void);

struct pcb_s{
	func_t pc;
	unsigned int* sp;
	int registres [NB_REG];
	void* args;
	
	Etat etat;
	
	struct pcb_s* next;
};

struct pcb_s* current_pcb;
struct pcb_s* next_running;

void init_pcb(struct pcb_s* pcb, func_t f, void* args,unsigned int stack_size);

void create_process(func_t f, int stack_size);
void schedule();
void start_current_process();
void start_sched();
