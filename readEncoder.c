/* Reads data from Encoder and writes to serial port in this format */

#include "readEncoder.h"
#include <stdint.h>
#include "DataPoint.h"
#include "Encoder.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include "USART.h"

/* ------------- Global Variables -------------- */

// Initialize our encoder
volatile Encoder encoder = {(int16_t)0, (uint8_t)0};

//  ----------- Interrupt Service Routines ----------- //
ISR(INT0_vect) /* on change of encoder pin A */
{

    onEncoderInterrupt(&encoder);
}

ISR(INT1_vect) /* on change of encoder pin B*/
{
    onEncoderInterrupt(&encoder);
}

int main()
{

    ENCODER_PORT |=
        (1 << ENCODER_A_PIN) | (1 << ENCODER_B_PIN); /* enable pull-up resistors for encoder */

    initUSART();
    initEncoder();

    sei(); /* enable global interrupts */

    //  Full Speed
    clock_prescale_set(clock_div_1);

    //  Initialize State with a initial read
    encoder.state = readEncoderPinState();

    while (1)
    {
        /* Nothing to do here */
    }
    return 0;
}