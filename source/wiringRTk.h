#ifndef _WIRINGRTK_H_
int wiringPiSetup (void);
int wiringPiSetupGpio (void);
int wiringPiSetupPhys (void);
int wiringPiSetupSys (void);
void pinMode (int pin, int mode);
void pullUpDnControl (int pin, int pud);
void digitalWrite (int pin, int value);
int digitalRead (int pin);
unsigned int millis (void);
unsigned int micros (void);
void delay (unsigned int howLong);
void delayMicroseconds (unsigned int howLong);

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

#endif

