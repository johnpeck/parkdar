/* main.h */

void portb_init(void);
void timer2_init(void);
void timer2_start(void);
void timer2_stop(void);
void adc_init(void);
void adc_read(int16_t *adc_data, uint8_t avgnum);
void adc_mux(uint8_t channel);
void range_report(int16_t *adc_data);
void adc_report(int16_t data);
void getdata(void);
void fosc_cal(void);
void led(uint8_t state);
void timer0_init(void);
void timer0_start(void);
void timer0_stop(void);
