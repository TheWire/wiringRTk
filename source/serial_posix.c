#include <string.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

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
	cfsetispeed(&serialSettings, RTK_BAUD);
	cfsetospeed(&serialSettings, RTK_BAUD);
	//int iFlags = TIOCM_DTR;
	//ioctl(serialDevice, TIOCMBIC, &iFlags);//off
	//ioctl(serialDevice, TIOCMBIS, &iFlags);//on
	//iFlags = 0;
	//ioctl(serialDevice, TIOCMGET, &iFlags);
	//iFlags |= TIOCM_RTS;
	//iFlags &= ~TIOCM_RTS;
	//ioctl(serialDevice, TIOCMSET, &iFlags);
	serialSettings.c_cflag |= (CLOCAL | CREAD);
	serialSettings.c_cflag |= CS8;
	//serialSettings.c_cflag &= ~CSIZE;
	serialSettings.c_cflag &= ~CSTOPB;
	serialSettings.c_cflag &= ~(PARENB | PARODD);
	serialSettings.c_cflag &= ~CRTSCTS;
	serialSettings.c_iflag &= ~(IXON | IXOFF | IXANY);
	//serialSettings.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG | ECHOK | ECHONL | IEXTEN);
	//serialSettings.c_oflag &= ~(OPOST | ONLCR | OCRNL);
	//serialSettings.c_iflag &= ~(INLCR | IGNCR | ICRNL | IGNBRK);
	//serialSettings.c_iflag &= ~(INPCK | ISTRIP);
	///serialSettings.c_iflag &= ~IUCLC;
	//serialSettings.c_iflag &= ~PARMRK;
	serialSettings.c_cc[VMIN] = 0;
	serialSettings.c_cc[VTIME] = 10;
	tcflush(serialDevice, TCIFLUSH);
	if(tcsetattr(serialDevice, TCSANOW, &serialSettings) != 0) {
		return -2;
	}
	return 0;
}
