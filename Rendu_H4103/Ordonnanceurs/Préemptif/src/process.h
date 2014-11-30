#define STACK_SIZE 128
#define NB_REG 14
#include <stdint.h>

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

void init_pcb(struct pcb_s* pcb, func_t f, void* args,uint32_t stack_size);


