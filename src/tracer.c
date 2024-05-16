#include "../includes/tracer.h"

tracer_stack *init_tracer_stack(size_t init_size)
{
    tracer_stack *st = (tracer_stack *)malloc(sizeof(tracer_stack));
    if (st == NULL)
        return NULL;
    st->nodes = (void **)malloc(sizeof(void *) * init_size);
    if (st->nodes == NULL)
    {
        free(st);
        return NULL;
    }
    st->stack_size = init_size;
    st->stack_top = 0;
    return st;
}

void destroy_tracer_stack(tracer_stack *st)
{
    if (surelyF(st == NULL))
        return;
    if (surelyT(st->nodes != NULL))
        free(st->nodes);
    free(st);
}

void *pop_tracer_node(tracer_stack *st)
{
    // NULL would mean the stack is empty
    if (is_stack_empty(st))
        return NULL;
    st->stack_top--;
    return st->nodes[st->stack_top];
}

ret_t push_tracer_node(tracer_stack *st, void *data)
{
    if (is_stack_full(st))
    {
        // resize
        // if failed, return FAILURE else SUCCESS
        void **temp = st->nodes;
        size_t new_len = (st->stack_size + (st->stack_size / 2));
        st->nodes = (void **)realloc(st->nodes, sizeof(void *) * new_len); // increase by half of original length
        if (st->nodes == NULL)
        {
            st->nodes = temp; // restore for later cleanup
            return FAILURE;
        }
        st->stack_size = new_len;
    }
    st->nodes[st->stack_top] = data;
    st->stack_top++;
    return SUCCESS;
}