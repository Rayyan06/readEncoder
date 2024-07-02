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
extern const uint8_t state_table[][4];

/* Structures */
typedef struct
{
    volatile int16_t position;
    volatile uint8_t state;
} Encoder;


/* Initialize */
void initEncoder(void);

/* Read the current state of the encoder pins A & B */
uint8_t readEncoderPinState(void);

/* Update the state of the encoder. State is in the format of 0b000(DIRECTION_BIT)00(A_PIN)(B_PIN)*/
void updateEncoderState(volatile Encoder* encoder);

/* Update the position of the encoder based on the state */
void updateEncoderPosition(volatile Encoder* encoder);

void onEncoderInterrupt(volatile Encoder *encoder);

#endif