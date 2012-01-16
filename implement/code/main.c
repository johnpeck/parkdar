/* main.c */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "LCD_driver.h"
#include "LCD_functions.h"

/* Global variables */


/* Definitions for led() */
#define ON 1
#define OFF 0

int main(void) {
    uint16_t adc_data = 0;
    
    sei(); // Enable interrupts
    fosc_cal(); // Set calibrated 1MHz system clock
    portb_init(); // Set up port B
    usart_init(); // Set up the USART
    timer2_init();  // Set up, stop, and reset timer2
    timer0_init();
    usart_puts("Initialize ADC\r\n");
    adc_init();
    usart_puts("Initialize LCD\r\n");
    lcd_init(); // From LDC_driver
    usart_puts("Start main loop\r\n");
    lcd_puts("Hello",0); // From LCD_functions
    timer2_start(); // Start stimulus
    adc_mux(1); // Switch to the voltage reader at J407
    for(;;) {
        adc_read(&adc_data);
        adc_report(adc_data);
        OCR0A = (uint8_t)(adc_data);
    }// end main for loop
} // end main

/* led()
 * Turns the led on or off
 * 1 -- LED turns on
 * 0 -- LED turns off */
 void led(uint8_t state) {
    if (state == 1)
        PORTB |= _BV(PB4);
    else
        PORTB &= ~(_BV(PB4));
}

/* portb_init()
 * I have an LED at PB4, and I'm using it as the output compare pin
 *     for timer0.
 * I'm using PB7 as the output compare pin for timer2 */
void portb_init(void) {
    DDRB |= (1<<DDB4); // Set bit 4 of port B for output
    DDRB |= (1<<DDB7); // Set bit 7 of port B for output
}

/* timer2_init()
 * Timer 2 is an 8-bit timer that can be operated asynchronously.  I'll
 *     use it clocked by the 32.768 kHz crystal on the butterfly.
 * In CTC mode, the OC2A pin will toggle when the timer reaches the
 *     compare register value.  The pin has to toggle twice to reach
 *     its original state, so the square wave frequency at OC2A will be
 *     32.768kHz / (2 * OCR2A) with no prescaler. */
void timer2_init(void) {
    /* The recommended procedure for switching to an asychronous clock
     * source:
     * 1. Disable interrupts from timer 2 compare match and overflow */
    TIMSK2 = 0;
    /* 2. Set timer 2 to be clocked from the TOSC1 pin */
    ASSR = (1<<AS2);
    /* 3a. Set timer 2 prescaler. No prescaler is 001. */
    TCCR2A = (0<<CS22) | (0<<CS21) | (1<<CS20);
    /* 3b. Set Clear on Timer match mode */
    TCCR2A |= (1<<WGM21) | (0<<WGM20);
    /* 3c. Set the OC2A pin to toggle on compare match */
    TCCR2A |= (0<<COM2A1) | (1<<COM2A0);
    /* 4. Set timer 2's output compare register */
    OCR2A = 33;
    /* 5. Set the initial counter register value */
    TCNT2 = 0;
    /* 6. Look at the asynchronous status register to figure out if
     *    timer 2 has settled in to normal operation.  This means that
     *    the counter value is being automatically updated (TCN2UB = 0),
     *    the configuration register is ready to take new values,
     *    (TCR2UB = 0), and that the output compare register is ready
     *    to take new values (OCR2UB = 0). */
     while((ASSR & 1<<TCN2UB) | (ASSR & 1<<TCR2UB) | (ASSR & 1<<OCR2UB));
     /* 7. Clear timer 2 interrupt flags */
     TIFR2 = 0;
     /* 8. Enable output compare match interrupt if necessary using:
      * TIMSK2 = 1 << OCIE2A */
     timer2_stop(); // Stop the counter
     TCNT2 = 0; // Reset the counter
}

/* timer2_start()
 * Simply writes a prescaler value to start timer2.  */
void timer2_start(void) {
    /* Set timer 2 prescaler.  No prescaler is 001. */
    TCCR2A |= (0<<CS22) | (0<<CS21) | (1<<CS20);
    /* Look at the asynchronous status register to figure out if timer 2
     * has settled in to normal operation.  This means that the counter
     * value is being automatically updated (TCN2UB = 0) and that the
     * configuration register is ready to take new values (TCR2UB = 0). */
    while((ASSR & 0x01) | (ASSR & 0x04));
}

/* timer2_stop() */
void timer2_stop(void) {
    TCCR2A &= ~( (1<<CS22) | (1<<CS21) | (1<<CS20) );
}

/* timer0_init() 
 * This timer will supply the modulation tone pulses that will indicate
 * the proximity of an obstacle.  It should be set up for phase-correct
 * PWM mode.  This init function will both set up and start the timer. */
void timer0_init(void) {
    /* Set timer0 prescaler and set phase-correct PWM mode (WGM 01).
     * Prescalers:
     * 100 -- fc/256 for ~130ms period
     * 101 -- fc/1024 for ~512ms period */
    TCCR0A = (1<<CS02) | (0<<CS01) | (1<<CS00) | (0<<WGM01) | (1<<WGM00);
    /* Set compare output mode to clear the 0C0A pin after an up-counting 
     * match, and set it after a down-counting match. */
    TCCR0A |= (1<<COM0A1) | (0<<COM0A0);
    /* Set timer0's initial output compare register */
    OCR0A = 25;
    /* Clear timer0 interrupt flags */
    TIFR0 = 0;
    /* Enable output compare match interrupt. */
    TIMSK0 = 1 << OCIE0A;
    TCNT0 = 0; // Reset the counter
}

/* timer0_stop() */
void timer0_stop(void) {
    TCCR0A &= ~( (1<<CS02) | (1<<CS01) | (1<<CS00) );
}

/* usart_receive
 * Simple USART receive function based on polling of the receive
 * complete (RXCn) flag.  The Butterfly has only one USART, so n will
 * always be zero.  The USART must be initialized before this can be used.
 *
 * The function simply waits for data to be present in the receive buffer
 * by checking the RXCn flag.  This flag is set when data is present. */
unsigned char usart_receive(void) {
    while( !(UCSR0A & (1<<RXC0)));
    return UDR0;
}

/* usart_putc(char data)
 * Sends a character to the USART */
void usart_putc(char data) {
    /* Wait for empty transmit buffer */
    while( !( UCSR0A & (1<<UDRE0)) );
    /* Put data into buffer -- sends the data */
    UDR0 = data;
}

/* usart_puts(char s[])
 * Sends a string over the USART by repeatedly calling usart_putc() */
void usart_puts(char s[]) {
    int i = 0;
    while(i < 64) // don't get stuck if it is a bad string
    {
        if( s[i] == '\0' ) break; // quit on string terminator
        usart_putc(s[i++]);
    }
}

/* Initialize the ADC.  */
void adc_init(void) {
    /* The butterfly has Vcc connected to AVcc via a low-pass filter.
     * It also has a shunt capacitor at the Aref pin.  So I can use the
     * voltage at AVcc as the reference. */
    ADMUX = (0<<REFS1) | (1<<REFS0);

    /* Right-justify the data in the high and low registers by clearing
     * ADLAR.  This is a 10-bit ADC. */
    ADMUX &= ~(_BV(ADLAR));

    /* The butterfly's analog input is connected to ADC1 on pin 60.
     * This makes a nice initialization value. */
    ADMUX |= 1;

    /* Enable the ADC and set the fosc --> fsar prescaler.
     * The SAR conversion requires between 50 and 200kHz for 10-bit
     * resolution, but can be set as high as fosc/2 for lower resolution.
     * Set the prescaler to 8 to get fsar = 125kHz.
     * Normal conversions take 13 cycles, or 13*8us = 104us for
     * fsar = 125kHz.  The maximum conversion rate is thus fsar / 13
     * or 9.6kHz for fsar = 125kHz. */
    ADCSRA = (1<<ADEN) | (0<<ADPS1) | (1<<ADPS0);

    /* Disable auto-triggering.  We'll trigger the ADC manually. */
    ADCSRA &= ~(_BV(ADATE));

    /* The first ADC conversion will take 25 ADC clock cycles instead of
     * the normal 13.  The first one initializes the ADC. Take a single
     * conversion for this initialization step. */
    ADCSRA |= (1<<ADSC);

    /* The ADIF bit in the ADCSRA register will be set when the conversion
     * is finished. Wait for conversion to finish. */
    while(!(ADCSRA & (1<<ADIF)));
}

/* Set the mux channel for the ADC input.
 * channel = 0 -- ADC0
 * channel = 1 -- ADC1 (Voltage at J407 divided by 6)
 * ...
 * channel = 7 -- ADC7
 *
 * The mux selection overrides any data direction selection made with
 * DDRF.  See section 13.3 of the datasheet. */
void adc_mux(uint8_t channel) {
    ADMUX &= (1<<REFS1) | (1<<REFS0) | (1<<ADLAR);
    ADMUX |= channel;
}

/* adc_read() 
 * Reads raw data from the ADC selected by adc_mux(). */
void adc_read(uint16_t *adc_data) {
    uint16_t adc_temp = 0;

    /* Enable the ADC.  It seems like I already did this in adc_init(),
     * but the part locks up if I don't also do it here. */
    ADCSRA |= _BV(ADEN);

    ADCSRA |= (1<<ADSC);  // do single conversion
    while(!(ADCSRA & (1<<ADIF)));  // wait for conversion to finish
    adc_temp = ADCL;            // read out ADCL register
    adc_temp += (ADCH << 8);    // read out ADCH register
    *adc_data = adc_temp;
}

/* adc_report()
 * Writes the ADC value to the USART and the LCD */
void adc_report(uint16_t repdata) {
    char s[10];

    //usart_puts("The ADC value is 0x");
    sprintf(s,"%x ",repdata);
    usart_puts(s); // Send to the USART
    usart_puts("\r\n");
    sprintf(s,"0x%x",repdata);
    lcd_puts(s,0); // Send to the LCD
}

/* usart_init()
 * Initialize the USART.  The butterfly only has one USART, so all the
 * n values for configuration registers are 0. */
void usart_init(void) {
    /* Set the USART baudrate registers for 9600.  With a fosc of 1MHz,
     * and double speed operation enabled, this means UBRR0 = 12.  UBRR
     * is a 12-bit register, so it has a high and a low byte. */
    UBRR0H = 0;
    UBRR0L = 12;

    /* Set double speed mode. */
    UCSR0A = (1<<U2X0);

    /* Enable the receiver and transmitter.  Leave an option to enable
     * the receive complete interrupt enable and the data register
     * empty interrupt enable. */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(0<<RXCIE0)|(0<<UDRIE0);

    /* Set the USART to asynchronous at 8 bits no parity and 1 stop bit */
    UCSR0C = (0<<UMSEL0)|(0<<UPM00)|(0<<USBS0)|(3<<UCSZ00)|(0<<UCPOL0);
}

/* fosc_cal(void)
 * This sets the frequency of the system clock provided by the internal
 * RC oscillator.  Since this frequency depends on voltage, time, and
 * temperature, the actual frequency is calibrated by comparing the
 * resulting system clock with the 32kHz crystal clock source.   In the
 * end, we'll have a 1MHz system clock to within about 2% */
void fosc_cal(void) {
    unsigned char calibrate = 0; // This is zero while calibrating.
    int temp;
    unsigned char tempL;

    /* The CLKPCE bit must be written to logic one to enable changing
     * the CLKPS bits.  This bit can only be written to one if the others
     * in CLKPR are simultaneously written to zero.  The CLKPR bits must
     * be written immediatly after changes are enabled, otherwise CLKPCE
     * will be cleared by hardware in 4 cycles. */
    CLKPR = (1<<CLKPCE);

    /* The atmega169 is shipped with the internal RC oscillator selected.
     * This gives a system clock of ~8MHz if no prescalers are applied.
     * The CKDIV8 fuse is set by default, which causes the reset
     * value of CLKPR to be 0011.  This gives a factor-of-8 prescaler
     * for the system clock, making it ~1MHz.  This next line just calls
     * attention to this without changing any defaults. */
    CLKPR = (1<<CLKPS1) | (1<<CLKPS0);

    /* Disable interrupts from timer 2 compare match and overflow */
    TIMSK2 = 0;

    /* The butterfly board has a 32kHz crystal connected between the
     * TOSC1 and TOSC2 pins.  This can be used as a clock source for
     * timer 2.  This is called running the timer asynchronously, and is
     * set by setting the AS2 bit in ASSR while clearing all others. */
    ASSR = (1<<AS2);

    /* Set timer 2 compare value. */
    OCR2A = 200;

    /* Disable interrupts from timer 1 compare match and overflow */
    TIMSK0 = 0;

    /* Start timer 1 with the system clock source and no prescaler */
    TCCR1B = (1<<CS10);
    /* Start timer 2 with whatever clock source it has been set to
     * (the 32kHz oscillator in this case).  No prescaler. */
    TCCR2A = (1<<CS20);

    /* Look at the asynchronous status register to figure out if timer 2
     * has settled in to normal operation.  This means that the counter
     * value is being automatically updated (TCN2UB = 0) and that the
     * configuration register is ready to take new values (TCR2UB = 0). */
    while((ASSR & 0x01) | (ASSR & 0x04));

    // wait for external crystal to stabilise
    /* The board has just been turned on, so we need to wait for the
     * crystal to stabilize.  The _delay_loop_2 function takes a 16-bit
     * (the 2 is for two bytes) integer for a countdown timer.  The timer
     * is decremented every 4 system clocks.  So a 1MHz clock would give
     * a maximum delay of 65536 * 1us * 4 = 262ms */
    for(int i = 0; i < 10; i++)
            _delay_loop_2(30000); // About 100ms

    while(!calibrate) {
        /* Disable all interrupts by clearing the global interrupt mask.
         * Look in avr/interrupt.h for a better description of cli() */
        cli();

        /* Clear timer 1 and timer 2 interrupt flag registers.  Strangely
         * enough, bits in these registers are cleared when logic ones
         * are written to them. */
        TIFR1 = 0xFF;
        TIFR2 = 0xFF;

        /* Reset timers 1 and 2 by clearing their counting registers.
         * Timer 1 is 16-bit, and thus has two counting bytes.  Timer 2
         * is 8-bit. */
        TCNT1H = 0;
        TCNT1L = 0;
        TCNT2 = 0;

        /* The OCF2A flag will be set when timer 2 reaches the OCR2A
         * compare value. Busy loop until timer 2 reaches this value. An
         * OCR2A value of 200 should cause a 6104us delay */
        while ( !(TIFR2 && (1<<OCF2A)) );

        TCCR1B = 0; // Stop timer 1
        sei(); // Enable all interrupts again.

        /* If timer 1 overflowed, it's going way too fast. Set temp to
         * the maximum value to avoid having to deal with it.  If it
         * didn't overflow, read the value into temp. */
        if ( (TIFR1 && (1<<TOV1)) ) {
            temp = 0xFFFF;
        }
        else {
            tempL = TCNT1L;
            temp = TCNT1H;
            temp = (temp << 8);
            temp += tempL;
        }

        /* These timer boundaries are somewhat mysterious.  A 1MHz clock
         * should give 6104 counts in temp for OCR2A=200.  But there's
         * some extra cycles spent between when timer 2 overflows and
         * when timer 1 can be stopped. */
        if (temp > 6250) {
            OSCCAL--;   // The internRC oscillator runs to fast, decrease the OSCCAL
        }
        else if (temp < 6120) {
            OSCCAL++;   // The internRC oscillator runs to slow, increase the OSCCAL
        }
        else
            calibrate = 1;//TRUE;   // the interRC is correct

        TCCR1B = (1<<CS10); // Restart timer 1
    }
}



/* Interrupt
 * Find the name of this interrupt signal in iom169p.h and not pa.  Why
 * not?  We define the mcu name to be atmega169p in the makefile, not
 * atmega169pa. See the naming convention outlined at
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 * to make sure you don't use depricated names. */
ISR(TIMER0_COMP_vect) {
    // led(ON);
    /* The interrupt code goes here. */
    // led(OFF);
}
