//return difference between time in args and current time
unsigned long timeDifference(unsigned long secs, unsigned int micros);
//get current time in seconds and microseconds
void setTimeStamp(unsigned long *secs, unsigned int *micros);
//wait using system time loop in microseconds
void delayMicroSystemTime (unsigned int micros);
//wait using nanosleep time loop in microseconds
void delayMicroSystemSleep(unsigned long micros);

