#ifndef READENCODER_H
#define READENCODER_H

/* Our CPU clock speed */
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define PRESCALER_VALUE 64
#define SAMPLE_RATE 100 // Interrupt every 1000ms

#define TIMER_TICKS_PER_MILLISECOND (F_CPU / PRESCALER_VALUE) / 1000
/* Compare Value is the Timer count speed in millisecond, multiplied by the Timer Period*/
#define COMPARE_VALUE TIMER_TICKS_PER_MILLISECOND *SAMPLE_RATE

/* Initializes the Timer with some settings */
static inline void initTimer1(void);

#endif