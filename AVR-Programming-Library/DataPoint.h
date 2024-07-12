#include <stdint.h>

#ifndef DATAPOINT_H
#define DATAPOINT_H
typedef struct
{
    int16_t position; /* In ticks */
    uint16_t time;    /* In milliseconds  */
} DataPoint;

#endif