
/*
  Quick and dirty functions that make serial communications work.

  Note that receiveByte() blocks -- it sits and waits _forever_ for
   a byte to come in.  If you're doing anything that's more interesting,
   you'll want to implement this with interrupts.

   initUSART requires BAUDRATE to be defined in order to calculate
     the bit-rate multiplier.  9600 is a reasonable default.

  May not work with some of the older chips:
    Tiny2313, Mega8, Mega16, Mega32 have different pin macros
    If you're using these chips, see (e.g.) iom8.h for how it's done.
    These old chips don't specify UDR0 vs UDR1.
    Correspondingly, the macros will just be defined as UDR.
*/

#include "USART.h"
#include <avr/io.h>
#include <util/setbaud.h>

void initUSART(void)
{                         /* requires BAUD */
    UBRR0H = UBRRH_VALUE; /* defined in setbaud.h */
    UBRR0L = UBRRL_VALUE;
#if USE_2X
    UCSR0A |= (1 << U2X0);
#else
    UCSR0A &= ~(1 << U2X0);
#endif
    /* Enable USART transmitter/receiver */
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); /* 8 data bits, 1 stop bit */
}

void transmitByte(uint8_t data)
{
    /* Wait for empty transmit buffer */
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = data; /* send data */
}

/* Here are a bunch of useful printing commands */

void printBinaryByte(uint8_t byte)
{
    /* Prints out a byte as a series of 1's and 0's */
    uint8_t bit;
    for (bit = 7; bit < 255; bit--)
    {
        if (bit_is_set(byte, bit))
            transmitByte('1');
        else
            transmitByte('0');
    }
}
void printString(const char myString[])
{
    uint8_t i = 0;
    while (myString[i])
    {
        transmitByte(myString[i]);
        i++;
    }
}

void printWord(uint16_t word)
{

    transmitByte('0' + (word / 10000));       /* Ten-thousands */
    transmitByte('0' + ((word / 1000) % 10)); /* Thousands */
    transmitByte('0' + ((word / 100) % 10));  /* Hundreds */
    transmitByte('0' + ((word / 10) % 10));   /* Tens */
    transmitByte('0' + (word % 10));          /* Ones */
}
