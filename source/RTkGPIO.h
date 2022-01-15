#ifndef _RTKGPIO_H_
//setup wiringRTk using wiringPi pin layout
//returns 0 for success -1 for failure.
int RTk_wiringRTkSetup();
//setup wiringRTk using broadcom pin layout
//returns 0 for success -1 for failure.
int RTk_wiringRTkSetupGpi();
//setup wiringRTk using physical pin layout
//returns 0 for success -1 for failure.
int RTk_wiringRTkSetupPhys();
//setup pin for INPUT or OUTPUT
void RTk_pinMode(int pin, int mode);
//return whether input pin is HIGH or LOW
int RTk_digitalRead(int pin);
//set output pin to HIGH or LOW
void RTk_digitalWrite(int pin, int value);
//set pull up, down or no resistor on pin
void RTk_pullUpDnControl(int pin, int pud);
//get time since any setup functions called in milliseconds
unsigned int RTk_millis (void);
//get time since any setup functions called in microseconds
unsigned long RTk_micros (void);
//wait in milliseconds
void RTk_delay (unsigned int howLong);
//wait in microseconds
//use system time loop for less than 100 micoseconds
//for greater than 100 use nanosleep
void RTk_delayMicroseconds (unsigned long howLong);
//close rtk serial device and cleanup wiringRTk
void RTk_closeWiringRTk();

//wiringPi api values
#ifndef PHYS

#define PHYS 0
#define BCM 1
#define WPI 2
#endif

#ifndef INPUT

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1
#define PUD_OFF 0
#define PUD_DOWN 1
#define PUD_UP 2

#endif

//RTk.GPIO api values
#define NUMPINS 28
#define CHSTART 'a'
#define DIGITAL_READ_SIZE 4

#define GPIO_MODE_INPUT 'I'
#define GPIO_MODE_OUTPUT 'O'

#define GPIO_VALUE_HIGH '1'
#define GPIO_VALUE_LOW '0'

#define GPIO_READ '?'
#define GPIO_PULL_DOWN 'D'
#define GPIO_PULL_UP 'U'
#define GPIO_PULL_NONE 'N'

#endif
