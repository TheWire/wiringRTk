#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "find_serial.h"
#include "serial_posix.h"

//write byte to serial return number of bytes successfully written.
int rtkWrite(int serialDevice, char byte) {
	// printf("%c\n", byte);
	return write(serialDevice, &byte, sizeof(char));
}

int rtkWriteMultiple(int serialDevice, char* buffer, int size) {
	// printf("%c%c\n", buffer[0], buffer[1]);
	return write(serialDevice, buffer, size);
}

//reads up to size number of bytes into buffer until term character 
//returns number of bytes read.
int rtkRead(int serialDevice, char* buffer, int size, char termChar) {
	int amountRead = 0;
	while(amountRead < size) {
		char data;
		if(read(serialDevice, &data, 1) > 0) {
			if(data == termChar) return amountRead;
			buffer[amountRead] = data;
			amountRead++;
		}
	}
	return amountRead;
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
	if(setupSerial(serial) < 0) {close(serial); return -3;}
	return serial;
}
