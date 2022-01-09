#include <time.h>

#define MICROS_IN_SEC 1000000
#define NANOS_IN_SEC 1000000000

//return difference between time in args and current time
unsigned long timeDifference(unsigned long secs, unsigned int micros) {
	struct timespec ts;
	unsigned int timeElapsed;
	clock_gettime(CLOCK_REALTIME, &ts);
	timeElapsed = (ts.tv_sec - secs) * MICROS_IN_SEC;
	timeElapsed += ts.tv_nsec - (micros * 1000);
	return timeElapsed;
}

//get current time in seconds and microseconds
void setTimeStamp(unsigned long *secs, unsigned int *micros) {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	*secs = ts.tv_sec;
	*micros = ts.tv_nsec * 1000;
}

//wait using system time loop in microseconds
void delayMicroSystemTime(unsigned int micros) {
	long nanosecs, nanomod;
	struct timespec currentTime, endTime;
	clock_gettime(CLOCK_REALTIME, &currentTime);
	endTime.tv_sec = currentTime.tv_sec + (micros / MICROS_IN_SEC);
	nanosecs = currentTime.tv_nsec + ((micros % MICROS_IN_SEC) * 1000);
	nanomod = nanosecs % NANOS_IN_SEC;
	if(nanomod > 0) {
		endTime.tv_sec += nanosecs/ NANOS_IN_SEC;
		endTime.tv_nsec = nanomod;
	} else {
		endTime.tv_nsec = nanosecs;
	}
	while((currentTime.tv_sec < endTime.tv_sec) && (currentTime.tv_nsec < endTime.tv_nsec)) {
		clock_gettime(CLOCK_REALTIME, &currentTime);
	}
}

//wait using nanosleep time loop in microseconds
void delayMicroSystemSleep(unsigned long micros) {
	struct timespec ts;
	ts.tv_sec = micros / MICROS_IN_SEC;
	ts.tv_nsec = (micros % MICROS_IN_SEC) * 1000;
	nanosleep(&ts, NULL);
}
