#include <stdio.h>
#include <stdlib.h>
#include "serial.h"
#include "delay.h"
#include "RTkGPIO.h" //added this ages after everything else so check

#ifdef _WIN32

#elif __APPLE__
	#include <sys/syslimits.h>
#elif __linux__
	#include <linux/limits.h>
#else
#	error "Unknown platform"
#endif

//main wiringRTk handle 
typedef struct WiringRTkHandle {
	int isSetup;
	int pinLayout;
	char* serialDevicePath;
	int serialDevice;
	unsigned long seconds;
	unsigned int microseconds;
} WiringRTkHandle;

//RTk pinmap Physical, BCM, wiringPi 
const int PINMAP[NUMPINS][3] =
	{
		{3, 2, 8},
		{5, 3, 9},
		{7, 4, 7},
		{8, 14, 15},
		{10, 15, 16},
		{11, 17, 0},
		{12, 18, 1},
		{13, 27, 2},
		{15, 22, 3},
		{16, 23, 4},
		{18, 24, 5},
		{19, 10, 12},
		{21, 9, 13},
		{22, 25, 6},
		{23, 11, 14},
		{24, 8, 10},
		{26, 7, 11},
		{27, 0, 30},
		{28, 1, 31},
		{29, 5, 21},
		{31, 6, 22},
		{32, 12, 26},
		{33, 13, 23},
		{35, 19, 24},
		{36, 16, 27},
		{37, 26, 25},
		{38, 20, 28},
		{40, 21, 29}
	};

//gets wiringRTk handle stored in static pointer.
//creats handle on first call.
WiringRTkHandle *rtk_getRTkHandle() {
	static WiringRTkHandle *rtkHandle;
	if(rtkHandle == NULL) {
		rtkHandle = malloc(sizeof(WiringRTkHandle));
		if(rtkHandle == NULL) {fprintf(stderr, "malloc failed\n"); exit(1);}
	}
	return (WiringRTkHandle*) rtkHandle;
}

//close rtk serial device and cleanup wiringRTk
void RTk_closeWiringRTk() {
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	if(rtk->isSetup == 1) {
		closeRTk(rtk->serialDevice);
		free(rtk->serialDevicePath);
		free(rtk);
	}
}

//return pin RTk.GPIO api value from setup pinmap
int rtk_getChannel(int pin) {
	int i;
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	for(i = 0; i < NUMPINS; i++) {
		if(PINMAP[i][rtk->pinLayout] == pin) {
			return (PINMAP[i][BCM] + CHSTART);
		}
	}
	return -1;
}

//return output setup RTk.GPIO api value from wiringPi value
int rtk_getMode(int mode) {
	switch(mode) {
		case INPUT:
			return GPIO_MODE_INPUT;
		case OUTPUT:
			return GPIO_MODE_OUTPUT;
	}
	return -1;
}

//return pullup/down RTk.GPIO api value from wiringPi value
int rtk_getPUD(int mode) {
	switch(mode) {
		case PUD_OFF:
			return GPIO_PULL_NONE;
		case PUD_DOWN:
			return GPIO_PULL_DOWN;
		case PUD_UP:
			return GPIO_PULL_UP;
	}
	return -1;
}

//return input RTk.GPIO api value from wiringPi value
int rtk_getValueInput(int value) {
	if(value == GPIO_VALUE_LOW) {
		return LOW;
	} else if(value == GPIO_VALUE_HIGH) {
		return HIGH;
	}
	return -1;
} 

//return output RTk.GPIO api value from wiringPi value
int rtk_getValueOutput(int value) {
	if(value <= LOW) {
		return GPIO_VALUE_LOW;
	}
	return GPIO_VALUE_HIGH;
} 


//setup pin for INPUT or OUTPUT
void RTk_pinMode(int pin, int mode) {
	int WRITE_SIZE = 2;
	char *buffer = (char*) malloc(WRITE_SIZE);
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	buffer[0] = (char) rtk_getChannel(pin);
	buffer[1] = (char) rtk_getMode(mode);
	int ret = rtkWriteMultiple(rtk->serialDevice, buffer, WRITE_SIZE);
	if(ret < WRITE_SIZE) {fprintf(stderr, "RTk serial write failed");}
	free(buffer);
	if(mode == OUTPUT) {
		RTk_pullUpDnControl(pin, PUD_OFF);
	}
}

//return whether input pin is HIGH or LOW
int RTk_digitalRead(int pin) {
	int ret, channel;
	char buffer[DIGITAL_READ_SIZE];
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	channel = rtk_getChannel(pin);

	if(rtkWrite(rtk->serialDevice, channel) < 1) goto write_failed;
	if(rtkWrite(rtk->serialDevice, GPIO_READ) < 1) goto write_failed;

	ret = rtkRead(rtk->serialDevice, buffer, sizeof(buffer), '\n');
	// printf("read ret: %d\n", ret);
	// printf("%d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3]);
	if(buffer[0] == channel) {
		return rtk_getValueInput(buffer[1]);
	}
	return -1;
	write_failed:
		fprintf(stderr, "RTk serial write failed");
}

//set output pin to HIGH or LOW
void RTk_digitalWrite(int pin, int value) {
	char ret;
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	ret = rtk_getChannel(pin);
	ret = rtkWrite(rtk->serialDevice, ret);
	if(ret < 1) goto write_failed;
	ret = rtkWrite(rtk->serialDevice, rtk_getValueOutput(value));
	if(ret < 1) goto write_failed;
	return;
	write_failed:
		fprintf(stderr, "RTk serial write failed");
}

//set pull up, down or no resistor on pin
void RTk_pullUpDnControl(int pin, int pud) {
	int WRITE_SIZE = 2;
	char *buffer = (char*) malloc(WRITE_SIZE);
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	buffer[0] = (char) rtk_getChannel(pin);
	buffer[1] = rtk_getPUD(pud);
	int ret = rtkWriteMultiple(rtk->serialDevice, buffer, WRITE_SIZE);
	if(ret < WRITE_SIZE) {fprintf(stderr, "RTk serial write failed");}
	RTk_digitalWrite(pin, LOW);
	free(buffer);
}

//get time since any setup functions called in milliseconds
unsigned int RTk_millis (void) {
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	return timeDifference(rtk->seconds, rtk->microseconds) / 1000;
}

//get time since any setup functions called in microseconds
unsigned long RTk_micros (void) {
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	return timeDifference(rtk->seconds, rtk->microseconds);
}

//wait in milliseconds
void RTk_delay (unsigned int howLong) {
	delayMicroSystemSleep(howLong * 1000);
}

//wait in microseconds
//use system time loop for less than 100 micoseconds
//for greater than 100 use nanosleep
void RTk_delayMicroseconds (unsigned long howLong) {
	if(howLong < 100) {
		delayMicroSystemTime((unsigned int) howLong);
	} else {
		delayMicroSystemSleep(howLong);
	}
}

//impliments all setup functions
//returns 0 for success -1 for failure.
int rtk_setupGPIO(int pinLayout) {
	WiringRTkHandle *rtk = rtk_getRTkHandle();
	if(rtk->isSetup == 0) {
		rtk->pinLayout = pinLayout;
		setTimeStamp(&rtk->seconds, &rtk->microseconds);
		rtk->serialDevicePath = malloc(PATH_MAX + 1);
		if(rtk->serialDevicePath == NULL) {fprintf(stderr, "malloc failed\n"); exit(1);}
		rtk->serialDevice = getRTk(rtk->serialDevicePath);
		if(rtk->serialDevice < 0) {
			free(rtk->serialDevicePath);
			fprintf(stderr, "wiringRTkSetup failed code: %d\n", rtk->serialDevice);
			return -1;
		}
		rtk->isSetup = 1;
		fprintf(stderr, "RTk.GPIO found on %s\n", rtk->serialDevicePath);
		return 0;
	}
	fprintf(stderr, "wiringRTkSetup failed\n");
	return -1;
}

//setup wiringRTk using wiringPi pin layout
//returns 0 for success -1 for failure.
int RTk_wiringRTkSetup() {
	return rtk_setupGPIO(WPI);
}
//setup wiringRTk using broadcom pin layout
//returns 0 for success -1 for failure.
int RTk_wiringRTkSetupGpi() {
	return rtk_setupGPIO(BCM);
}
//setup wiringRTk using physical pin layout
//returns 0 for success -1 for failure.
int RTk_wiringRTkSetupPhys() {
	return rtk_setupGPIO(PHYS);
}
