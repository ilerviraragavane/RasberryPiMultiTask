#ifndef SEM_H
#define SEM_H
#include "sched.h"


struct wait_list{
	struct pcb_s* pcb;
	struct wait_list* next;
};

struct sem_s{
    int val;
    struct wait_list* list_wait_pcb;
};



void sem_init(struct sem_s* sem, unsigned int val);
void sem_down(struct sem_s* sem);
void sem_up(struct sem_s* sem);

#endif //SEM_H
