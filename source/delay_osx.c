#include <mach/mach_time.h>
#include <time.h>


#define MICROS_IN_SEC 1000000
#define NANOS_IN_SEC 1000000000

//return difference between time in args and current time
unsigned long timeDifference(unsigned long secs, unsigned int micros) {
	uint64_t time = mach_absolute_time();
	return time - ((secs * NANOS_IN_SEC) + (micros * 1000));
}

//get current time in seconds and microseconds
void setTimeStamp(unsigned long *secs, unsigned int *micros) {
	uint64_t time = mach_absolute_time();
	*secs = time / NANOS_IN_SEC;
	*micros = (time % NANOS_IN_SEC) * 1000;
}

//wait using system time loop in microseconds
void delayMicroSystemTime (unsigned int micros) {
	uint64_t startTime = mach_absolute_time();
	uint64_t endTime = startTime + (micros * 1000);
	while(startTime < endTime){
		startTime = mach_absolute_time();
	}
}

//wait using nanosleep time loop in microseconds
void delayMicroSystemSleep(unsigned long micros) {
	struct timespec ts;
	ts.tv_sec = micros / MICROS_IN_SEC;
	ts.tv_nsec = (micros % MICROS_IN_SEC) * 1000;
	nanosleep(&ts, NULL);
}
