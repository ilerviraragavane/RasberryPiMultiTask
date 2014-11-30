#include "sched.h"


#define STACK_SIZE 128


void create_process(func_t f, void* args)
{
	pcb_s* pcb;
	pcb = (pcb_s*) malloc_alloc(sizeof(pcb_s));

	init_pcb(pcb,f,STACK_SIZE, args);
	
	if (premier == NULL)
	{
		premier = pcb;
		premier->next = pcb;
		dernier = pcb;
	}
	else
	{
		dernier->next = pcb;
		dernier = pcb;
		pcb->next = premier;
	}
}


void yield()
{

	pcb_s * temp;
	if (current_process == &idle)
	{
		if(premier == NULL)
		{
			return;
		}
		else
		{
			temp = premier;
		}
	}
	else
	{
		temp = current_process->next;
	}
	
	while (temp->state == TERMINATED)
	{
		pcb_s* n = temp->next;
		
		/* Free memory */
		malloc_free((char*) temp->sp);
		malloc_free((char*) temp);
		
		if (premier == dernier)
		{
			temp = &idle;
			premier = NULL;
			dernier = NULL;
			idle.next = NULL;
		}
		else
		{
			if ( temp == premier )
			{
				premier = n;
				idle.next = n;
			}
			if ( temp == dernier )
			{
				dernier = n;
			}
			current_process->next = n;
			temp = n;
		}
	}
	
	next_running = temp;

	ctx_switch();
}


void start_current_process()
{
	current_process->state = READY;
	current_process->entry_point();
	current_process->state = TERMINATED;
	while(1)
		yield();
}

void start_sched()
{
	current_process = &idle;
	idle.state = READY;
	idle.next = premier;
	
	// pour les diodes du Rasberry
	init_hw();
	
	while(1)
	{
		yield();
	}
}
