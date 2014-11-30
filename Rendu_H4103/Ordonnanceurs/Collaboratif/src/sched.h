#include "process.h"
#include <stdlib.h>
#include "malloc.h"
#include "hw.h"

void create_process(func_t f, void* args);
void schedule();
void start_current_process();
void start_sched();

pcb_s idle;

pcb_s* premier;
pcb_s* dernier;

