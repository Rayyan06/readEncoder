#include "Encoder.h"
#include <avr/io.h>
#include "USART.h"
/* Initialization functions */
static inline void initEncoder()
{
    ENCODER_PORT |=
        (1 << ENCODER_A_PIN) | (1 << ENCODER_B_PIN); /* enable pull-up resistors for encoder */

    /* Initialize Interrupts*/
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

static inline void updateEncoderState(Encoder *encoder)
{
    encoder->state = state_table[encoder->state & 0x07][readEncoderPinState()];
}

static inline void updateEncoderPosition(Encoder *encoder)
{
    if (encoder->state & DIR_CW)
    {
        printString("++");
        encoder->position++;
    }
    else if (encoder->state & DIR_CCW)
    {
        printString("--");
        encoder->position--;
    }

    printWord(encoder->position); // Print low byte
    printString(" ");
    printBinaryByte(encoder->state);
    printString("\r\n");
}

void onEncoderInterrupt(Encoder *encoder)
{
    updateEncoderState(encoder);
    updateEncoderPosition(encoder);
}