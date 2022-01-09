//write byte to serial return number of bytes successfully written.
int rtkWrite(int serialDevice, char byte);
//reads size number of bytes into buffer returns number of bytes read.
int rtkRead(int serialDevice, void* buffer, int size);
//finds serial device and returns handle.
//serialPath must be PATH_MAX + 1 returns path to serial device
int getRTk(char* serialPath);
//close serial handle.
void closeRTk(int serialDevice);
