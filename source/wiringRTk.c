#include "RTkGPIO.h"

int wiringPiSetup (void) {
	return RTk_wiringRTkSetup();
}
int wiringPiSetupGpio (void) {
	return RTk_wiringRTkSetupGpi();
}
int wiringPiSetupPhys (void) {
	return RTk_wiringRTkSetupPhys();
}
int wiringPiSetupSys (void) {
	return RTk_wiringRTkSetupGpi();
}
void pinMode (int pin, int mode) {
	RTk_pinMode(pin, mode);
}
void pullUpDnControl (int pin, int pud) {
	RTk_pullUpDnControl(pin, pud);
}
void digitalWrite (int pin, int value) {
	RTk_digitalWrite(pin, value);
}
int digitalRead (int pin) {
	return RTk_digitalRead(pin);
}
unsigned int millis (void) {
	return RTk_millis ();
}
unsigned int micros (void) {
	return (unsigned int) RTk_micros ();
}
void delay (unsigned int howLong) {
	RTk_delay (howLong);
}
void delayMicroseconds (unsigned int howLong) {
	RTk_delayMicroseconds (howLong); 
}
