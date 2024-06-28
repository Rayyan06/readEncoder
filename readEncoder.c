/* Reads data from Encoder and writes to serial port in this format */

#include "readEncoder.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include "USART.h"

/* ------------- Global Variables -------------- */

// Initialize our encoder
volatile Encoder encoder = {(int16_t)0, (uint8_t)0};

/* Initialization functions */
static inline void initEncoderInterrupts()
{

    EIMSK |= (1 << INT0) | (1 << INT1); /* enable INT0 & INT1 */
    EICRA |= (1 << ISC00);              /* trigger INT0 when button state changes */
    EICRA |= (1 << ISC10);              /* trigger INT1 when button state changes */
}

/* Encoder Functions */
// TODO: Move all this to a dedicated file
static inline uint8_t readEncoderPinState()
{
    uint8_t pin_a = (PIND & (1 << ENCODER_A_PIN)) >> (ENCODER_A_PIN - 1);
    uint8_t pin_b = (PIND & (1 << ENCODER_B_PIN)) >> ENCODER_B_PIN;

    return pin_a | pin_b;

    /*
    return (PIND & (1 << ENCODER_A_PIN) ? 0x02  : 0x00) |
           ((PIND & (1 << ENCODER_B_PIN)) ? 0x01 : 0x00);
    */
}

static inline void updateEncoderState()
{
    encoder.state = state_table[encoder.state & 0x07][readEncoderPinState()];
}
static inline void updateEncoderPosition()
{
    if (encoder.state & DIR_CW)
    {
        printString("++");
        encoder.position++;
    }
    else if (encoder.state & DIR_CCW)
    {
        printString("--");
        encoder.position--;
    }

    printWord(encoder.position); // Print low byte
    printString(" ");
    printBinaryByte(encoder.state);
    printString("\r\n");
}

void onEncoderInterrupt()
{
    updateEncoderState();
    updateEncoderPosition();
}

//  ----------- Interrupt Service Routines ----------- //
ISR(INT0_vect) /* on change of encoder pin A */
{

    onEncoderInterrupt();
}

ISR(INT1_vect) /* on change of encoder pin B*/
{
    onEncoderInterrupt();
}

int main()
{

    ENCODER_PORT |=
        (1 << ENCODER_A_PIN) | (1 << ENCODER_B_PIN); /* enable pull-up resistors for encoder */

    initUSART();
    initEncoderInterrupts();
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