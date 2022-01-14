#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

//RTk.GPIO baud rate
#define RTK_BAUD B230400


//takes path to serial device returns handle to serial device.
int openSerial(char* serialDevice) {
	int serial;
	serial = open(serialDevice, O_RDWR | O_NOCTTY | O_NONBLOCK | O_NDELAY);
	if(serial == -1) {
		return -1;
	}
	return serial;
	
}

//sets up open serial device returns 0 on sucess < 0 on failure.
int setupSerial(int serialDevice) {
	struct termios serialSettings;
	memset(&serialSettings, 0, sizeof(struct termios));
	if(tcgetattr(serialDevice, &serialSettings) != 0) {
		return -1;
	}
	/*
	if(ioctl(serialDevice, TIOCEXCL) == -1) {
        	printf("Could not lock serial port\n");
        	return -1;
    }
    */
	cfsetospeed(&serialSettings, RTK_BAUD);
	serialSettings.c_cflag |= (CLOCAL | CREAD);
	serialSettings.c_cflag &= ~CSIZE;
	serialSettings.c_cflag |= CS8;
	serialSettings.c_cflag &= ~CSTOPB;
	serialSettings.c_cflag &= ~(PARENB | PARODD | CMSPAR);
	if(tcsetattr(serialDevice, TCSANOW, &serialSettings) != 0) {
		return -2;
	}
	return 0;
}
