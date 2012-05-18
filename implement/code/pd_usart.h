/* pd_usart.h
 * 
 * Used to set up the parkdar USART. */
 
#define USART_TXBUFFERSIZE 80


/* Send a format string to the USART interface */
uint8_t usart_printf (const char *fmt, ...);

/* Send a format string stored in flash memory to the USART interface.
 */
uint8_t usart_printf_p(const char *fmt, ...);

/* usart_receive
 * Simple USART receive function based on polling of the receive
 * complete (RXCn) flag.  The Butterfly has only one USART, so n will
 * always be zero.  The USART must be initialized before this can be used.
 *
 * The function simply waits for data to be present in the receive buffer
 * by checking the RXCn flag.  This flag is set when data is present. */
unsigned char usart_receive(void);

/* usart_putc(char data)
 * Sends a character to the USART */
void usart_putc(char data);

/* usart_puts(char s[])
 * Sends a string over the USART by repeatedly calling usart_putc() */
void usart_puts(const char s[]);

/* usart_puts_p(const char *data)
 * Sends strings stored in flash memory to the usart.
 */
void usart_puts_p(const char *data);


/* usart_init()
 * Initialize the USART.  The butterfly only has one USART, so all the
 * n values for configuration registers are 0. */
void usart_init(void);
