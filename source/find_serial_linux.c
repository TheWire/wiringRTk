#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include <glob.h>
#include "_find_serial.h"

//linux serial device 
#define USBSERIAL "/dev/ttyUSB*"
//paths to linux serial devices
#define PATH_TO_SERIAL_DEVICES "/sys/class/tty/"
#define DEVICE "/device"


//return USB id value from file path.
int getID(char* path, char* file) {
	unsigned int hwid = 0;
	FILE *idFile;
	char* buffer = calloc(PATH_MAX + 1, sizeof(char));
	if(buffer == NULL) {fprintf(stderr, "calloc failed\n"); exit(1);}
	strcat(buffer, path);
	strcat(buffer, "/");
	strcat(buffer, file);
	idFile = fopen(buffer, "r");
	if(idFile) {
		fscanf(idFile, "%x", &hwid);
	} else {
		printf("%s error id\n", path);
		fclose(idFile);
		free(buffer);
		return -1;
	}
	free(buffer);
	fclose(idFile);
	return hwid;
}

//checks if serial device matchs RTk.GPIO vender and product id
//return 1 if RTk found, 0 if not
int identifyRTk(char* device) {
	char* path, device_path, usb_device_path;
	int idVendor, idProduct;
	path = malloc(PATH_MAX + 1);
	if(path == NULL) {fprintf(stderr, "malloc failed\n"); exit(1);}
	strcpy(path, PATH_TO_SERIAL_DEVICES);
	device_path = malloc(PATH_MAX + 1);
	if(device_path == NULL) {fprintf(stderr, "malloc failed\n"); exit(1);}
	usb_device_path = malloc(PATH_MAX + 1);
	if(usb_device_path == NULL) {fprintf(stderr, "malloc failed\n"); exit(1);}
	strcat(path, device);
	strcat(path, DEVICE);
	if(realpath(path, device_path) == NULL) {
		printf("error reading serial devices\n");
		free(path);
		free(device_path);
		free(usb_device_path);
		return 0;
	}
	strcpy(usb_device_path, dirname(dirname(device_path)));
	if(getID(usb_device_path, VENDOR_ID) == RTK_VENDOR_ID) {
		if(getID(usb_device_path, PRODUCT_ID) == RTK_PRODUCT_ID) {
			return 1;
		}
	}
	free(path);
	free(device_path);
	free(usb_device_path);
	return 0;	
	
}

//list serial devices and identify RTk.GPIO by vendor/ product id
//path should be size PATH_MAX + 1
int getSerial(char* path) {
	int i = 0;
	glob_t serial_list;
	glob(USBSERIAL, 0, NULL, &serial_list);
	for(i = 0; i < serial_list.gl_pathc; i++) {
		if(identifyRTk(basename(serial_list.gl_pathv[i])) == 1) {
			strcpy(path, serial_list.gl_pathv[i]);
			globfree(&serial_list);
			return 0;
		}
	} 
	globfree(&serial_list);
	return -1;
}
