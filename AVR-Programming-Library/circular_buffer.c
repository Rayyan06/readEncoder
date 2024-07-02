#include "circular_buffer.h"

void initBuffer(volatile circular_buffer_t *buffer)
{
    buffer->head = 0;
    buffer->tail = 0;
}

uint8_t buffer_is_empty(volatile circular_buffer_t *buffer)
{
    return (buffer->head == buffer->tail);
};
uint8_t buffer_write(volatile circular_buffer_t *buffer, DataPoint data)
{
    // Calculate the next write index
    uint8_t next_head = (buffer->head + 1) % TX_BUFFER_SIZE;
    uint8_t buffer_is_full = (next_head == buffer->tail);

    if (!buffer_is_full)
    {
        // Update the data at the head pointer
        buffer->data[buffer->head] = data;

        // Update the head pointer forword, or loop around
        buffer->head = next_head;
        return 1;
    }
    return 0;
}

DataPoint buffer_get(volatile circular_buffer_t *buffer)
{
    DataPoint point = buffer->data[buffer->tail];

    // Update the tail pointer
    buffer->tail = (buffer->tail + 1) % TX_BUFFER_SIZE;

    return point;
}