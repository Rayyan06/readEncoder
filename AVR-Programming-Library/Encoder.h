#include <stdint.h>

#ifndef ENCODER_H
#define ENCODER_H

/* Wiring Constants */
#define ENCODER_A_PIN 2
#define ENCODER_B_PIN 3

#define ENCODER_PORT PORTD

/* Definitions for the quadrature encoder state machine */
#define DIR_NONE 0x00 /* No step yet */
#define DIR_CW 0x10   /* Clockwise step */
#define DIR_CCW 0x20  /* Counterclockwise step*/

#define R_START 0x3     /* 0011 */
#define R_CW_BEGIN 0x1  /* 0001 */
#define R_CW_NEXT 0x0   /* 0000 */
#define R_CW_FINAL 0x2  /* 0010 */
#define R_CCW_BEGIN 0x6 /* 0110 */
#define R_CCW_NEXT 0x4  /* 0100 */
#define R_CCW_FINAL 0x5 /* 0101 */
#define R_ILLEGAL 0x7   /* 0111 */


// State Table
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

/* Structures */
typedef struct
{
    volatile int16_t position;
    volatile uint8_t state;
} Encoder;


/* Initialize */
static inline void initEncoder(void);

/* Read the current state of the encoder pins A & B */
static inline uint8_t readEncoderPinState(void);

/* Update the state of the encoder. State is in the format of 0b000(DIRECTION_BIT)00(A_PIN)(B_PIN)*/
static inline void updateEncoderState(Encoder* encoder);

/* Update the position of the encoder based on the state */
static inline void updateEncoderPosition(Encoder* encoder);

void onEncoderInterrupt(Encoder *encoder);

#endif