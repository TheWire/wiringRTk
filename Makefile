ifeq ($(OS),Windows_NT)
	FIND_SERIAL=
	FIND_SERIAL_LIBS=
else
	UNAME_S := $(shell uname -s)
endif

ifeq ($(UNAME_S),Linux)
	DELAY= delay_linux.c
	FIND_SERIAL= find_serial_linux.c
	FIND_SERIAL_LIBS=
endif    
ifeq ($(UNAME_S),Darwin)
	DELAY= delay_osx.c
	FIND_SERIAL= find_serial_osx.c
	FIND_SERIAL_LIBS= -framework IOKit -framework CoreFoundation
endif

CC=gcc
CFLAGS=
LIBS= $(FIND_SERIAL_LIBS)
INCLUDE=
SHAREDLIB= -shared
FPIC = -fPIC
ARCHIVER = ar
ARCHIVER_ARGS = rcs
INSTALL = install -c
INSTALL_PATH = /usr/local/lib/

all:libwiringRTk.so libRTkGPIO.so libwiringRTk.a libRTkGPIO.a

libwiringRTk.so: checkbin wiringRTk.o RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(CC) $(CFLAGS) $(SHAREDLIB) build/wiringRTk.o build/RTkGPIO.o build/serial.o build/serial_posix.o build/find_serial.o build/delay.o $(LIBS) -o bin/shared/libwiringRTk.so

libRTkGPIO.so: checkbin RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(CC) $(CFLAGS) $(SHAREDLIB) build/RTkGPIO.o build/serial.o build/serial_posix.o build/find_serial.o build/delay.o $(LIBS) -o bin/shared/libRTkGPIO.so

libwiringRTk.a: checkbin wiringRTk.o RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(ARCHIVER) $(ARCHIVER_ARGS) bin/static/libwiringRTk.a build/wiringRTk.o build/RTkGPIO.o build/serial.o build/serial_posix.o build/find_serial.o build/delay.o $(LIBS)

libRTkGPIO.a: checkbin RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(ARCHIVER) $(ARCHIVER_ARGS) bin/static/libRTkGPIO.a build/RTkGPIO.o build/serial.o build/serial_posix.o build/find_serial.o build/delay.o $(LIBS)  

wiringRTk.o: checkbuild source/wiringRTk.c
		$(CC) $(CFLAGS) $(FPIC) -c source/wiringRTk.c $(INCLUDE) -o build/wiringRTk.o

RTkGPIO.o: checkbuild source/RTkGPIO.c
		$(CC) $(CFLAGS) $(FPIC) -c source/RTkGPIO.c $(INCLUDE) -o build/RTkGPIO.o

serial.o: source/serial.c
		$(CC) $(CFLAGS) $(FPIC) -c source/serial.c -o build/serial.o

serial_posix.o: source/serial_posix.c
		$(CC) $(CFLAGS) $(FPIC) -c source/serial_posix.c -o build/serial_posix.o

find_serial.o: source/$(FIND_SERIAL)
		$(CC) $(CFLAGS) $(FPIC) -c source/$(FIND_SERIAL) -o build/find_serial.o

delay.o: source/$(DELAY)
		$(CC) $(CFLAGS) $(FPIC) -c source/$(DELAY) -o build/delay.o

clean:
	@rm -rf build/*
	@rm -rf bin/*

checkbuild:
	@mkdir -p build/

checkbin:
	@mkdir -p bin/static
	@mkdir -p bin/shared

install:
	$(INSTALL) ./bin/shared/libRTkGPIO.so $(INSTALL_PATH)
	$(INSTALL) ./bin/shared/libwiringRTk.so $(INSTALL_PATH)

uninstall:
	rm $(INSTALL_PATH)libRTkGPIO.so
	rm $(INSTALL_PATH)libwiringRTk.so
