#include "dispatcher.h"

void create_process(int stack_size, func_t f, void* args);
void schedule();
void start_current_process();
void start_sched();
