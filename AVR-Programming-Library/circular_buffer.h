#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
#include <stdint.h>
#include "DataPoint.h"

#define TX_BUFFER_SIZE 4
// Buffer for encoder data
typedef struct
{
    DataPoint data[TX_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} circular_buffer_t;

void initBuffer(volatile circular_buffer_t *buffer);

/* returns true if the buffer is empty */
uint8_t buffer_is_empty(volatile circular_buffer_t *buffer);


/* writes data to buffer */
uint8_t buffer_write(volatile circular_buffer_t *buffer, DataPoint data);

/* Retrieves a datapoint from the buffer */
DataPoint buffer_get(volatile circular_buffer_t *buffer);

#endif