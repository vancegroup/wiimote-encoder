#ifndef wiimote_h

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define twi_port PORTC
#define twi_ddr DDRC
#define twi_scl_pin 5
#define twi_sda_pin 4

#define dev_detect_port PORTD
#define dev_detect_ddr DDRD
#define dev_detect_pin 4

// initialize wiimote interface with id (6 bytes), starting data (6 bytes),
// and calibration data (6 bytes) (and a callback on requests)
void wm_init(unsigned char * id, unsigned char * t, unsigned char * cal_data, unsigned char calib_size, void (*function)(void));

// set button data (6 bytes)
void wm_newaction(unsigned char * d);

#define wiimote_h
#endif
