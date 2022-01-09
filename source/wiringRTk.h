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

