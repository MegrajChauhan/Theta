#ifndef _ARRAY_
#define _ARRAY_

// A dynamic array

#include <stdlib.h>
#include "../utils/config.h"
#include "../utils/types.h"

typedef struct array array;
typedef bool_t (*cmp)(void *, void*);

/*
 Instead of calling it a dynamic array, it is better to call it a store house.
 When something needs to temporarily store it's data somewhere, it can utilize an array.
 After pushing, it will not have to worry about that pointer
*/
struct array
{
    void **data;
    size_t array_size;
    size_t capacity; // We increase the capacity by half
};

#define is_array_full(arr) arr->array_size == arr->capacity
#define is_array_empty(arr) arr->array_size == 0

array *init_array(size_t);

void destroy_array(array *);

ret_t push_to_array(array *, void *);

void *pop_from_array(array *);

bool_t array_contains(array *,void*, cmp);

void destroy_elements(array *);

#endif