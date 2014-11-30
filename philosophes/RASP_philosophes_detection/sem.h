#ifndef SEM_H
#define SEM_H
#include "sched.h"
#define NB 5


struct wait_list{
	struct pcb_s* pcb;
	struct wait_list* next;
};

struct sem_s{
    int val;
    struct wait_list* list_wait_pcb;
};


struct mtx_s{
    int disponible; //0 si verrouillé, 1 si déverrouillé
    struct pcb_s* proprietaire;
    struct wait_list* list_wait_pcb;
};
struct mtx_s mutexs[NB]; // fourchettes

void sem_init(struct sem_s* sem, unsigned int val);
void sem_down(struct sem_s* sem);
void sem_up(struct sem_s* sem);

void mtx_init(struct mtx_s* mutex);
int mtx_lock(struct mtx_s* mutex);
void mtx_unlock(struct mtx_s* mutex);

#endif //SEM_H
