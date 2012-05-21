#include <stdio.h>
#include <stdarg.h> // Allows functions to accept an indefinite number of arguments
#include <avr/io.h>
#include "pd_usart.h"

/* pgmspace.h
 * Contains macros and functions for saving and reading data out of
 * flash.
 */
#include <avr/pgmspace.h>

/* Send a formatted string to the USART interface */
uint8_t usart_printf (const char *fmt, ...) { 
    va_list args; 
    uint8_t i; 
    char printbuffer[USART_TXBUFFERSIZE]; 

    va_start (args, fmt); 

    /* For this to work, printbuffer must be larger than 
    * anything we ever want to print. 
    */ 
    i = vsprintf (printbuffer, fmt, args); 
    va_end (args); 

    /* Print the string */ 
    usart_puts(printbuffer); 
    return 0; 
}

/* Send a format string stored in flash memory to the USART interface.
 */
uint8_t usart_printf_p(const char *fmt, ...) {
    va_list args; 
    uint8_t i; 
    char printbuffer[USART_TXBUFFERSIZE]; 

    va_start (args, fmt); 

    /* For this to work, printbuffer must be larger than 
    * anything we ever want to print. 
    */ 
    i = vsprintf_P (printbuffer, fmt, args); 
    va_end (args); 

    /* Print the string */ 
    usart_puts(printbuffer); 
    return 0; 
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
void usart_puts(const char s[]) {
    int i = 0;
    while(i < 64) // don't get stuck if it is a bad string
    {
        if( s[i] == '\0' ) break; // quit on string terminator
        usart_putc(s[i++]);
    }
}

/* usart_puts_p(const char *data)
 * Sends strings stored in flash memory to the usart.
 */
void usart_puts_p(const char *data_ptr) {
    uint8_t txdata = 1; // Dummy initialization value
    while ( txdata != 0x00 ) {
        txdata = pgm_read_byte(data_ptr);
        usart_putc(txdata);
        data_ptr++;
    }
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

    /* Configure USART control register B */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0); // Enable receiver and transmitter.
    UCSR0B |= (1<<RXCIE0); // Enable receive complete interrupts.
    UCSR0B |= (0<<TXCIE0); // Enable transmit complete interrupts (not right now).
    UCSR0B |= (0<<UDRIE0); // Enable data registter empty interrupts (not right now).

    /* Set the USART to asynchronous at 8 bits no parity and 1 stop bit */
    UCSR0C = (0<<UMSEL0)|(0<<UPM00)|(0<<USBS0)|(3<<UCSZ00)|(0<<UCPOL0);
}
