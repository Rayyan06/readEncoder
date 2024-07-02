#include "Encoder.h"
#include <avr/io.h>
#include "USART.h"

const uint8_t state_table[8][4] = {
    // R_CW_NEXT 00
    {R_CW_NEXT,
     R_CW_BEGIN | DIR_CCW,
     R_CW_FINAL | DIR_CW,
     R_START /* Invalid*/},
    // R_CW_BEGIN 01
    {R_CW_NEXT | DIR_CW, R_CW_BEGIN, R_CW_FINAL /*Invalid*/, R_START | DIR_CCW},
    {R_CW_NEXT | DIR_CCW, R_CW_BEGIN /* Invalid */, R_CW_FINAL, R_START | DIR_CW},    // R_CW_FINAL 02
    {R_START /* Invalid */, R_CW_BEGIN | DIR_CW, R_CCW_BEGIN | DIR_CCW, R_START},     // R_START  03
    {R_CCW_NEXT, R_CCW_FINAL | DIR_CCW, R_CCW_BEGIN | DIR_CW, R_START /* Invalid */}, // R_CCW_NEXT 04
    {R_CCW_NEXT | DIR_CW, R_CCW_FINAL, R_CCW_BEGIN /* Invalid */,
     R_START | DIR_CCW}, // R_CCW_FINAL 05
    {R_CCW_NEXT | DIR_CCW, R_CCW_NEXT /* Invalid */, R_CCW_BEGIN,
     R_START | DIR_CW},                   // R_CCW_BEGIN 06
    {R_START, R_START, R_START, R_START}, // R_ILLEGAL 07
};

/* Initialization functions */
inline void initEncoder()
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
uint8_t readEncoderPinState()
{
    uint8_t pin_a = (PIND & (1 << ENCODER_A_PIN)) >> (ENCODER_A_PIN - 1);
    uint8_t pin_b = (PIND & (1 << ENCODER_B_PIN)) >> ENCODER_B_PIN;

    return pin_a | pin_b;

    /*
    return (PIND & (1 << ENCODER_A_PIN) ? 0x02  : 0x00) |
           ((PIND & (1 << ENCODER_B_PIN)) ? 0x01 : 0x00);
    */
}

void updateEncoderState(volatile Encoder *encoder)
{
    encoder->state = state_table[encoder->state & 0x07][readEncoderPinState()];
}

void updateEncoderPosition(volatile Encoder *encoder)
{
    if (encoder->state & DIR_CW)
    {
        encoder->position++;
    }
    else if (encoder->state & DIR_CCW)
    {
        encoder->position--;
    }

    // printWord(encoder->position); // Print low byte
}

void onEncoderInterrupt(volatile Encoder *encoder)
{
    updateEncoderState(encoder);
    updateEncoderPosition(encoder);
}