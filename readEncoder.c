/* Reads data from Encoder and writes to serial port in this format */

#include "readEncoder.h"
#include <stdint.h>
#include "DataPoint.h"
#include "Encoder.h"
#include "circular_buffer.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>
#include "USART.h"

/* ------------- Global Variables -------------- */

// Initialize our encoder
volatile Encoder encoder = {(int16_t)0, (uint8_t)0};
volatile circular_buffer_t tx_buffer = {
    .head = 0,
    .tail = 0,
    .data = {{0, 0}, {0, 0}, {0, 0}, {0, 0}}};

/* Timer 1 is for timing the difference between interrupt triggers, and the real time with CTC */
static inline void initTimer1(void)
{
    /* Timer 1 is the 16 Bit timer */
    TCCR1B |= (1 << WGM12); /* CTC Mode */
    /* Normal mode (default), just counting */
    TCCR1B |= (1 << CS11) | (1 << CS10); /* Set the CPU Prescaler at /64 prescale, 16 Mhz / 64 =
                                            250000 ticks per second or 250 ticks per millisecond */
    // Set the compare value
    OCR1A = COMPARE_VALUE;
    // // Enable Compare Match A interrupt
    TIMSK1 |= (1 << OCIE1A);
}

//  ----------- Interrupt Service Routines ----------- //
ISR(INT0_vect) /* on change of encoder pin A */
{
    onEncoderInterrupt(&encoder);

    // Create a new data point with the current position value and timer value
    DataPoint datapoint = {encoder.position, TCNT1};

    // Add the data point to the transmission buffer
    buffer_write(&tx_buffer, datapoint);

    // Enable the USART Data Register Empty Interrupt to start transmission
    UCSR0B |= (1 << UDRIE0);
}

ISR(INT1_vect) /* on change of encoder pin B*/
{
    onEncoderInterrupt(&encoder);

    // Create a new data point with the current position value and timer value
    DataPoint datapoint = {encoder.position, TCNT1};

    // Add the data point to the transmission buffer
    buffer_write(&tx_buffer, datapoint);

    // Enable the USART Data Register Empty Interrupt to start transmission
    UCSR0B |= (1 << UDRIE0);
}

ISR(TIMER1_COMPA_vect)
{
    // Create a new data point with the current timer value and encoder position
    DataPoint datapoint = {encoder.position, COMPARE_VALUE};

    // Add the data point to the transmission buffer
    buffer_write(&tx_buffer, datapoint);

    // Enable the USART Data Register Empty Interrupt to start transmission
    UCSR0B |= (1 << UDRIE0);

    // Toggle LED on PB5 to indicate timer interrupt occurred
    PORTB ^= (1 << PB5);
}
/* USART Data Register Empty Interrupt Service Routine */
ISR(USART_UDRE_vect)
{
    static DataPoint current_data;
    static uint8_t checksum;
    static uint8_t index = 0;

    if (index == 0)
    {
        // Check if the buffer is not empty
        if (buffer_is_empty(&tx_buffer))
        {
            UCSR0B &= ~(1 << UDRIE0);
            return;
        }
        // Read the data from the buffer, only do this once initially
        current_data = buffer_get(&tx_buffer);
    }

    checksum = 0x03;

    switch (index)
    {
    case 0:
        UDR0 = 0x02; // Start Byte
        index++;
        break;
    case 1:
        UDR0 = (uint8_t)((current_data.position >> 8) & 0xFF); // Position High Byte
        index++;
        break;
    case 2:
        UDR0 = (uint8_t)(current_data.position & 0xFF); // Position Low Byte
        index++;
        break;
    case 3:
        UDR0 = (uint8_t)((current_data.time >> 8) & 0xFF); // Time High Byte
        index++;
        break;
    case 4:
        UDR0 = (uint8_t)(current_data.time & 0xFF); // Time Low Byte
        index++;
        break;
    case 5:
        UDR0 = checksum; //  checksum byte
        index = 0;
        break;
    }
    // UDR0 = 0x02;
    // UDR0 = (uint8_t)(current_data.position & 0xFF);
    // UDR0 = ((uint8_t)(current_data.position >> 8) & 0xFF);
    // UDR0 = (uint8_t)(current_data.time & 0xFF);
    // UDR0 = (uint8_t)((current_data.time >> 8) & 0xFF);
    // UDR0 = checksum; // checksum byte

    // If buffer is empty after sending low byte, disable UDRE interrupt
    if (buffer_is_empty(&tx_buffer) && index == 0)
    {
        UCSR0B &= ~(1 << UDRIE0);
    }
}

int main()
{
    DDRB |= (1 << DDB5);

    ENCODER_PORT |=
        (1 << ENCODER_A_PIN) | (1 << ENCODER_B_PIN); /* enable pull-up resistors for encoder */

    initUSART();
    initEncoder();
    initTimer1();
    initBuffer(&tx_buffer);

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