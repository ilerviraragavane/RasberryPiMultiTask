#include "process.h"
//#include "dispatcher.h"

#define STACK_SIZE 128

struct ctx_s ctx_A;
struct ctx_s ctx_B;

void
funcA()
{
  switch_to(&ctx_B);
  switch_to(&ctx_B);
  switch_to(&ctx_B);
  switch_to(&ctx_B);
  switch_to(&ctx_B);
  switch_to(&ctx_B);
}

void
funcB()
{
  switch_to(&ctx_A);
  switch_to(&ctx_A);
  switch_to(&ctx_A);
  switch_to(&ctx_A);
  switch_to(&ctx_A);
  switch_to(&ctx_A);
}

//------------------------------------------------------------------------
int
notmain ( void )
{
  init_ctx(&ctx_B, funcB, STACK_SIZE);
  
  start_ctx(&ctx_A, funcA);

  /* Pas atteignable vues nos 2 fonctions */
  return(0);
}
