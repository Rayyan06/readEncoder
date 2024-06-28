#include <stdint.h>

#ifndef DATAPOINT_H
#define DATAPOINT_H
typedef struct
{
    uint16_t time;    /* In milliseconds */
    int16_t position; /* In Ticks  */
} DataPoint;

#endif