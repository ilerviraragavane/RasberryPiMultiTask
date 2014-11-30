#include "sched.h"
#include "malloc.h"
#include "hw.h"
#include "dispatcher.h"

struct pcb_s idle;
struct pcb_s* ready_queue = (struct pcb_s *)0;
struct pcb_s* current_process = (struct pcb_s *) NULL;

const int TIME_SLICE[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

extern void consommateur();

#define PRINT(MSG) ;
#define EXIT(CODE) ;

void
start_current_process()
{
  current_process->state = READY;
  current_process->entry_point();

  /* The process is terminated */
  current_process->state = TERMINATED;
  yield();
}

int
init_process(struct pcb_s *pcb, int stack_size, func_t* f, int priority)
{	
  /* Function and args */
  pcb->entry_point = f;

  /* Stack allocation */
  pcb->size=stack_size;
  pcb->stack_base = malloc_alloc(stack_size);
  if(!pcb->stack_base)
    return 0;

  /* State and context */
  pcb->state = NEW;
  pcb->sp = ((uint32_t*) (pcb->stack_base + stack_size)) - 1;

  /* Fill in the stack with CPSR and PC */
  *(pcb->sp) = 0x53;
  pcb->sp --;
  *(pcb->sp) = (unsigned int) &start_current_process;
  
  /*Set Priority and time_slice*/
  pcb->priority = priority;
  pcb->time_slice = 0;
  
  return 1;
}

int
create_process(func_t* f, unsigned size, int priority)
{
  struct pcb_s *pcb;
  pcb = (struct pcb_s*) malloc_alloc(sizeof(struct pcb_s));

  if(!pcb)
    return 0;

  if (! ready_queue) {/* First process */
    ready_queue = pcb;
  } else {
    pcb->next = ready_queue->next;
  }
  
  ready_queue->next = pcb;
  return init_process(pcb,size,f,priority);
}

/* On remet le time_slice à 0 
 * si tous les processus ont dépassé leur quantum*/
void refresh_time_slice(struct pcb_s* first)
{
	struct pcb_s *pcb;
	pcb = first->next;
	
	while(pcb != first && pcb->time_slice > TIME_SLICE[pcb->priority])
		pcb = pcb->next;
	
	if(pcb == first && first->time_slice > TIME_SLICE[first->priority])
	{
		pcb = first->next;
		first->time_slice = 0;
		while(pcb != first)
		{
			pcb->time_slice = 0;
			pcb = pcb->next;
		}			
	}
}

void
schedule()
{
  struct pcb_s* pcb;
  struct pcb_s* pcb_init;
  struct pcb_s *priority_pcb;

  priority_pcb = current_process;
  
  /*Refresh du tim_slice*/
  //refresh_time_slice(current_process);
  
  if(current_process == &idle)
	pcb_init = current_process->next;
  else
	pcb_init = current_process;
  pcb = current_process;

  /* Start by eliminating all zombies (rhaaaaa !) */
  while (pcb->next->state == TERMINATED) {    
    /* If no process to be executed -> note that and leave loop */
    if (pcb->next == pcb) {
      pcb = NULL;
      break;

    } else {
      /* Particular case of the head */
      if (pcb->next == ready_queue)
	ready_queue = pcb;    
      

      /* Free memory */
      malloc_free((char*) pcb->next->stack_base);
      malloc_free((char*) pcb->next);
      
      /* Remove pcb from the list (FIXME : could be done after the loop) */
      pcb->next = pcb->next->next;

      /* Get next process */
      pcb = pcb->next;
    }
  }

  if (pcb != NULL) {
    /* On parcours la liste jusqu'à trouver un processus non bloqué 
     * et de priorité supérieur au processus courant*/
    pcb = pcb->next;
    /*Pour gérer le cas ou on switch à partir du idle*/
    if(pcb == pcb_init)
		pcb = pcb->next;
    while(pcb != pcb_init)
    {
		if((pcb->state != WAITING && pcb->priority >= priority_pcb->priority && pcb->time_slice < TIME_SLICE[pcb->priority])
			|| (priority_pcb->time_slice > TIME_SLICE[priority_pcb->priority]))
		{
			priority_pcb = pcb;
		} 
        pcb = pcb->next;
    }

    /* Si tous les processus sont bloqués -> on le note */
    if(priority_pcb == pcb_init)
      pcb_init->time_slice++;
	}

  if(pcb == NULL) {   /* Si pas de processus à élire -> idle */
    ready_queue = NULL;
    current_process = &idle;
  } else {            /* Sinon -> le processus élu est le suivant */
      current_process = priority_pcb;
  }
  
  refresh_time_slice(current_process);
}

void
yield()
{
  ctx_switch();
}

void
start_sched()
{
  current_process = &idle;
  idle.next = ready_queue;

  ENABLE_IRQ();

  while(1) {
    yield();
  }
}
