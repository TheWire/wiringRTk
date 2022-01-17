# wiringRTk
C library using [WiringPi][WiringPi] interface for [RTk.GPIO][RTk.GPIO].

Programs using basic wiringPi functionality can be linked to wiringRTk without code modification.

Alternate interface allows use of both wiringPi for rapsberry pi GPIO pins and RTk.GPIO together.

## Installation
---
```
make
sudo make install
```

## Example Code
---
### WiringPi interface

#### Header files

```
#include <wiringRTk.h>
```
or you can use header from [wiringPi][WiringPi] library
```
#include <wiringPi.h>
```
#### Setup  
wiringPi [pin][WiringPiPins] numbering
```
wiringPiSetup();
```
raspberry pi physical pins
```
wiringPiSetupPhys();
```
Broadcom pins
```
wiringPiSetupGpio();
//or
wiringPiSetupSys();
```
#### Pin Mode
```
//pin 8
pinMode(8, OUTPUT);
pinMode(8, INPUT);
```
#### Ouput
```
digitalWrite(8, HIGH);
digitalWrite(8, LOW);
```
#### Input
pullup or down
```
pullUpDnControl(8, PUD_DOWN);
pullUpDnControl(8, PUD_UP);
```
write
```
//HIGH or LOW
int val = digitalRead(8);
```
#### Delay
milliseconds
```
delay(1000);
```
microseconds
```
delayMicroseconds(1000);
```

#### Sample Program
```
#include <wiringRTk.h>

#define PIN 8

int main() {

    wiringPiSetup();
    pinMode(PIN, OUTPUT);

    while(1) {
        digitalWrite(PIN, HIGH);
        delay(1000);
        digitalWrite(PIN, LOW);
        delay(1000);
    }

    return 0;
}
```

### RTk interface
You can use the alternate wiringRTk interface. Useful if you want to use
raspberry pi GPIO pins and RTk.GPIO together.  

#### Setup
```
RTk_wiringRTkSetup();
RTk_wiringRTkSetupGpi();
RTk_wiringRTkSetupPhys();
```
All other functions are same as wiringPi but with `RTk_` prefix
#### Sample Program
```
#include <RTkGPIO.h>

#define PIN 8

int main() {

    RTk_wiringRTkSetup();
    RTk_pinMode(PIN, OUTPUT);

    while(1) {
        RTk_digitalWrite(PIN, HIGH);
        RTk_delay(1000);
        RTk_digitalWrite(PIN, LOW);
        RTk_delay(1000);
    }

    return 0;
}
```

## Compilation
---
Link with `-lwiringRTk`

```
gcc wiringRTk_sample.c -lwiringRTk -o wiringRTk_sample
```
or for wiringRTk interface use `-lRTkGPIO`






[RTk.GPIO]: https://uk.pi-supply.com/products/ryanteck-rtk-gpio-pc-gpio-interface
[WiringPi]: http://wiringpi.com/
[WiringPiPins]: http://wiringpi.com/pins/