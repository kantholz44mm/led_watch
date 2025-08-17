#include "fifo.h"
#include <string.h>

void fifo_init(fifo_t* fifo, u8* buffer, u32 element_size, u32 num_elements)
{
    fifo->buffer = buffer;
    fifo->element_size = element_size;
    fifo->num_elements = num_elements;
    fifo->base_index = 0;
    fifo->occupied = 0;
}

bool fifo_push(fifo_t* fifo, u8* new_element, bool override)
{
    if(fifo_is_full(fifo))
    {
        if(override)
        {
            fifo_pop(fifo, NULL);
        }
        else
        {
            return false;
        }
    }

    u32 write_index = ((fifo->base_index + fifo->occupied) % fifo->num_elements) * fifo->element_size;
    memcpy(&fifo->buffer[write_index], new_element, fifo->element_size);
    fifo->occupied++;

    return true;
}

bool fifo_pop(fifo_t* fifo, u8* element)
{
    if(fifo_is_empty(fifo))
    {
        return false;
    }

    if(element != NULL)
    {
        u32 read_index = fifo->base_index * fifo->element_size;
        memcpy(element, &fifo->buffer[read_index], fifo->element_size);
    }

    fifo->base_index++;
    fifo->occupied--;

    if(fifo->base_index >= fifo->num_elements)
    {
        fifo->base_index = 0U;
    }

    return true;
}

bool fifo_peek(fifo_t* fifo, u8* element)
{
    if(fifo_is_empty(fifo))
    {
        return false;
    }

    if(element != NULL)
    {
        u32 read_index = fifo->base_index * fifo->element_size;
        memcpy(element, &fifo->buffer[read_index], fifo->element_size);
    }

    return true;
}

void fifo_clear(fifo_t* fifo)
{
    fifo->occupied = 0;
}

u32 fifo_num_elements(fifo_t* fifo)
{
    return fifo->occupied;
}

u32 fifo_num_free(fifo_t* fifo)
{
    return fifo->num_elements - fifo->occupied;
}

bool fifo_is_empty(fifo_t* fifo)
{
    return fifo->occupied == 0U;
}

bool fifo_is_full(fifo_t* fifo)
{
    return fifo->occupied == fifo->num_elements;
}