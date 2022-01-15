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
SOURCE = source/
BUILD = build/
BIN = bin/
INSTALL_PATH = /usr/local/
INSTALL_LIB = lib/
INSTALL_INCLUDE = include/

all:libwiringRTk.so libRTkGPIO.so libwiringRTk.a libRTkGPIO.a

libwiringRTk.so: checkbin wiringRTk.o RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(CC) $(CFLAGS) $(SHAREDLIB) $(BUILD)wiringRTk.o $(BUILD)RTkGPIO.o $(BUILD)serial.o $(BUILD)serial_posix.o $(BUILD)find_serial.o $(BUILD)delay.o $(LIBS) -o $(BIN)shared/libwiringRTk.so

libRTkGPIO.so: checkbin RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(CC) $(CFLAGS) $(SHAREDLIB) $(BUILD)RTkGPIO.o $(BUILD)serial.o $(BUILD)serial_posix.o $(BUILD)find_serial.o $(BUILD)delay.o $(LIBS) -o $(BIN)shared/libRTkGPIO.so

libwiringRTk.a: checkbin wiringRTk.o RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(ARCHIVER) $(ARCHIVER_ARGS) $(BIN)static/libwiringRTk.a $(BUILD)wiringRTk.o $(BUILD)RTkGPIO.o $(BUILD)serial.o $(BUILD)serial_posix.o $(BUILD)find_serial.o $(BUILD)delay.o $(LIBS)

libRTkGPIO.a: checkbin RTkGPIO.o serial.o serial_posix.o find_serial.o delay.o
		$(ARCHIVER) $(ARCHIVER_ARGS) $(BIN)static/libRTkGPIO.a $(BUILD)RTkGPIO.o $(BUILD)serial.o $(BUILD)serial_posix.o $(BUILD)find_serial.o $(BUILD)delay.o $(LIBS)  

wiringRTk.o: checkbuild $(SOURCE)wiringRTk.c
		$(CC) $(CFLAGS) $(FPIC) -c $(SOURCE)wiringRTk.c $(INCLUDE) -o $(BUILD)wiringRTk.o

RTkGPIO.o: checkbuild $(SOURCE)RTkGPIO.c
		$(CC) $(CFLAGS) $(FPIC) -c $(SOURCE)RTkGPIO.c $(INCLUDE) -o $(BUILD)RTkGPIO.o

serial.o: $(SOURCE)serial.c
		$(CC) $(CFLAGS) $(FPIC) -c $(SOURCE)serial.c -o $(BUILD)serial.o

serial_posix.o: $(SOURCE)serial_posix.c
		$(CC) $(CFLAGS) $(FPIC) -c $(SOURCE)serial_posix.c -o $(BUILD)serial_posix.o

find_serial.o: $(SOURCE)$(FIND_SERIAL)
		$(CC) $(CFLAGS) $(FPIC) -c $(SOURCE)$(FIND_SERIAL) -o $(BUILD)find_serial.o

delay.o: $(SOURCE)$(DELAY)
		$(CC) $(CFLAGS) $(FPIC) -c $(SOURCE)$(DELAY) -o $(BUILD)delay.o

clean:
	@rm -rf $(BUILD)*
	@rm -rf $(BIN)*

checkbuild:
	@mkdir -p $(BUILD)

checkbin:
	@mkdir -p $(BIN)static
	@mkdir -p $(BIN)shared

install:
	$(INSTALL) $(BIN)shared/libRTkGPIO.so $(INSTALL_PATH)$(INSTALL_LIB)
	$(INSTALL) $(BIN)shared/libwiringRTk.so $(INSTALL_PATH)$(INSTALL_LIB)
	$(INSTALL) $(SOURCE)RTkGPIO.h $(INSTALL_PATH)$(INSTALL_INCLUDE)
	$(INSTALL) $(SOURCE)wiringRTk.h $(INSTALL_PATH)$(INSTALL_INCLUDE)
	ldconfig

uninstall:
	rm $(INSTALL_PATH)$(INSTALL_LIB)libRTkGPIO.so
	rm $(INSTALL_PATH)$(INSTALL_LIB)libwiringRTk.so
	rm $(INSTALL_PATH)$(INSTALL_INCLUDE)RTkGPIO.h
	rm $(INSTALL_PATH)$(INSTALL_INCLUDE)wiringRTk.h
	
