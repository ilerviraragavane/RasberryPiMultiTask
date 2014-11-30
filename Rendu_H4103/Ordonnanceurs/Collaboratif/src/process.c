#include "process.h"
#include "sched.h"

void ctx_switch()
{
	__asm("push {r0-r12,lr}");
	__asm("mov %0,sp" : "=r"(current_process->sp));
	// change le current
	current_process = next_running;
	__asm("mov sp, %0" : : "r"(current_process->sp));
	if (current_process->state == NEW)
	{
		start_current_process();
	}
	else
	{
		__asm("pop {r0-r12,lr}");		
	}
}

void init_pcb(pcb_s* pcb, func_t f, unsigned int stack_size, void* args)
{
	 uint32_t* stack;// = AllocateMemory(stack_size);
	 stack = malloc_alloc(stack_size);
	 pcb->state = NEW;
	 pcb->sp = &stack[stack_size-1]; // *(stack+stack_size)
	 pcb->args = args;
	 pcb->entry_point=f;
}
