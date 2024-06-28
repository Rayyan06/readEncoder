/* Functions to initialize, send, receive over USART

   initUSART requires BAUD to be defined in order to calculate
     the bit-rate multiplier.
 */
#include <stdint.h>

#ifndef BAUD      /* if not defined in Makefile... */
#define BAUD 9600 /* set a safe default baud rate */
#endif

/* These are defined for convenience */
#define USART_HAS_DATA bit_is_set(UCSR0A, RXC0)
#define USART_READY bit_is_set(UCSR0A, UDRE0)

/* Takes the defined BAUD and F_CPU,
   calculates the bit-clock multiplier,
   and configures the hardware USART                   */
void initUSART(void);

/* Blocking transmit and receive functions.
   When you call receiveByte() your program will hang until
   data comes through.  We'll improve on this later. */
void transmitByte(uint8_t data);

void printBinaryByte(uint8_t byte);

/* Prints a string to the serial terminal */
void printString(const char myString[]);

void printWord(uint16_t word);