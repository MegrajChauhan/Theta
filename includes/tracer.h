#ifndef _TRACER_
#define _TRACER_

#include <stdlib.h>
#include <string.h>
#include "../utils/types.h"
#include "../utils/config.h"

typedef struct tracer_stack tracer_stack;

/*
  0 means no members.
  1 means 1 member but to access it do 1-1 = 0
*/

struct tracer_stack
{
  void **nodes;
  size_t stack_size;
  size_t stack_top;
};

#define is_stack_full(ptr) ptr->stack_top == ptr->stack_size
#define is_stack_empty(ptr) ptr->stack_top == 0

tracer_stack *init_tracer_stack(size_t);

void destroy_tracer_stack(tracer_stack *);

void *pop_tracer_node(tracer_stack *);

ret_t push_tracer_node(tracer_stack *, void *);

void free_remaining_tracer_nodes(tracer_stack *);

#endif