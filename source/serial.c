#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "find_serial.h"
#include "serial_posix.h"

//write byte to serial return number of bytes successfully written.
int rtkWrite(int serialDevice, char byte) {
	printf("%d\n", byte);
	return write(serialDevice, &byte, sizeof(char));
}

//reads size number of bytes into buffer returns number of bytes read.
int rtkRead(int serialDevice, void* buffer, int size) {
	return read(serialDevice, buffer, size);
}

//close serial handle.
void closeRTk(int serialDevice) {
	close(serialDevice);
}

//finds serial device and returns handle.
//serialPath must be PATH_MAX + 1 returns path to serial device
int getRTk(char* serialPath) {
	int serial;
	if(getSerial(serialPath) != 0) {return -1;}
	serial = openSerial(serialPath);
	if(serial == -1) {return -2;}
	if(setupSerial(serial < 0)) {close(serial); return -3;}
	return serial;
}
