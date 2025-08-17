#pragma once
#include "types.h"

typedef struct {

    u8* buffer;
    u32 element_size;
    u32 num_elements;

    volatile u32 base_index;
    volatile u32 occupied;

} fifo_t;

void fifo_init(fifo_t* fifo, u8* buffer, u32 element_size, u32 num_elements);
bool fifo_push(fifo_t* fifo, u8* new_element, bool override);
bool fifo_pop(fifo_t* fifo, u8* element);
bool fifo_peek(fifo_t* fifo, u8* element);
void fifo_clear(fifo_t* fifo);

u32 fifo_num_elements(fifo_t* fifo);
u32 fifo_num_free(fifo_t* fifo);
bool fifo_is_empty(fifo_t* fifo);
bool fifo_is_full(fifo_t* fifo);