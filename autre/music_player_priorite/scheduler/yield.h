#ifndef DISPATCHER_H
#define DISPATCHER_H

// fonction appeler à chaque interruption
void __attribute__((naked)) ctx_switch();

#endif
