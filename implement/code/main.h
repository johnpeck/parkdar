/* main.h */

void portb_init(void);
void timer2_init(void);
void timer2_start(void);
void timer2_stop(void);
void adc_init(void);
void adc_read(uint16_t *adc_data);
void adc_mux(uint8_t channel);
void adc_report(uint16_t data);
void getdata(void);
void fosc_cal(void);
void usart_putc(char );
void usart_puts(char *);
void usart_init(void);
unsigned char usart_receive(void);
void led(uint8_t state);
void timer0_init(void);
void timer0_start(void);
void timer0_stop(void);
