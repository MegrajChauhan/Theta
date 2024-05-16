#include "../includes/array.h"

array *init_array(size_t init_capacity)
{
    array *arr = (array *)malloc(sizeof(array));
    if (arr == NULL)
        return NULL;
    arr->data = (void **)malloc(sizeof(void *) * init_capacity);
    if (arr->data == NULL)
    {
        free(arr);
        return NULL;
    }
    arr->capacity = init_capacity;
    arr->array_size = 0;
    return arr;
}

void destroy_array(array *arr)
{
    if (surelyF(arr == NULL))
        return;
    if (surelyT(arr->data != NULL))
        free(arr->data);
    free(arr);
}

ret_t push_to_array(array *arr, void *data)
{
    if (is_array_full(arr))
    {
        void **temp = arr->data;
        size_t new_len = arr->capacity + (arr->capacity / 2);
        arr->data = (void **)realloc(arr->data, sizeof(void *) * new_len);
        if (data == NULL)
        {
            arr->data = temp;
            return FAILURE;
        }
        arr->capacity = new_len;
    }
    arr->data[arr->array_size] = data;
    arr->array_size++;
    return SUCCESS;
}

void *pop_from_array(array *arr)
{
    if (is_array_empty(arr))
        return NULL;
    arr->array_size--;
    return arr->data[arr->array_size];
}

bool_t array_contains(array *arr, void *val, cmp fnc)
{
    // arr will not be NULL and so will be fnc
    for (size_t i = 0; i < arr->array_size; i++)
    {
        if (fnc(arr->data[i], val) == true)
            return true;
    }
    return false;
}

void destroy_elements(array *arr)
{
    for (size_t i = 0; i < arr->array_size; i++)
    {
        if (surelyT(arr->data[i] != NULL))
            free(arr->data[i]);
    }
}
