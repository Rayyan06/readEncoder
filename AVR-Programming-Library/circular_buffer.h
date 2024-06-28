#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
#include <stdint.h>
#include "DataPoint.h"

#define TX_BUFFER_SIZE 64



// Buffer for encoder data
typedef struct
{
    DataPoint data[TX_BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
} circular_buffer_t;

/* writes data to buffer */
void buffer_write(volatile circular_buffer_t *buffer, DataPoint data);

/* Retrievs */
DataPoint buffer_get(volatile circular_buffer_t *buffer);
#endif