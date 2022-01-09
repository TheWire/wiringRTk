#include <string.h>
#include <CoreFoundation/CoreFoundation.h>
 
#include <IOKit/IOKitLib.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/IOBSD.h>


#include <sys/syslimits.h>
#include "_find_serial.h"

#define SERIAL_DEVICE_TYPE "IOSerialBSDClient"
#define SERIAL_DEVICE_PATH_PROPERTY "IOCalloutDevice"
#define USB_DEVICE_TYPE "IOUSBDevice"
#define PARENT_SERVICE_TYPE "IOService"


//get serial device path in /dev from io registry
//serialPath must be of size PATH_MAX + 1
int getSerialPath(char* serialPath, io_object_t serialDevice) {
	CFStringRef cfstr = CFStringCreateWithCString(kCFAllocatorDefault, SERIAL_DEVICE_PATH_PROPERTY, kCFStringEncodingASCII);
	CFTypeRef cf  = IORegistryEntryCreateCFProperty(serialDevice, cfstr, kCFAllocatorDefault, 0);
	memset(serialPath, 0, PATH_MAX + 1);
	if(cf != NULL) {
		strcpy(serialPath, CFStringGetCStringPtr(cf, 0));
	} else {
		CFRelease(cfstr);
		CFRelease(cf);
		return -1;
	}
	CFRelease(cfstr);
	CFRelease(cf);
	return 0;
}

//get io registry name of device
//buffer size should be MAX_PATH + 1
char* registryEntryGetName(char* buffer, io_object_t device) {
	memset(buffer, 0, PATH_MAX + 1);
	IOObjectGetClass(device, buffer);
	return buffer;
}

//get usb parent of serial device
io_object_t getUSBParent(io_object_t device) {
	kern_return_t ret;
	io_object_t parent;
	char* str =  malloc(PATH_MAX + 1);
	if(str == NULL) {printf("malloc failed\n"); exit(1);}
	while(strcmp(str = registryEntryGetName(str, device), USB_DEVICE_TYPE) != 0) {
		parent = 0;
		ret = IORegistryEntryGetParentEntry(device, PARENT_SERVICE_TYPE,(void*) &parent);
		if(ret != 0) {
			free(str);
			return 0;
		}
		device = parent;
	}
	free(str);
	return device;
} 

//get int property from io object
int getIntProperty(io_object_t obj, char* property) {
	int ret = 0;
	CFStringRef cfstr = CFStringCreateWithCString(kCFAllocatorDefault, property, kCFStringEncodingASCII);
	CFTypeRef cf  = IORegistryEntryCreateCFProperty(obj, cfstr, kCFAllocatorDefault, 0);
	if(cf != NULL) {
		CFNumberGetValue(cf, kCFNumberSInt16Type, &ret);
		CFRelease(cf);
	} else {
		return -1;
	}
	return ret;
}

//checks if serial device matchs RTk.GPIO vender and product id
//return 1 if RTk found, 0 if not
int identifyRTk(io_object_t usbDevice) {
	if(getIntProperty(usbDevice, VENDOR_ID) == RTK_VENDOR_ID) {
		if(getIntProperty(usbDevice, PRODUCT_ID) == RTK_PRODUCT_ID) {
			return 1;
		}
	}
	return 0;
} 

//list serial devices and identify RTk.GPIO by vendor/ product id
//path should be size PATH_MAX + 1
int getSerial(char* path) {
	kern_return_t ret;
	io_iterator_t iter;
	io_object_t serialDevice, usbDevice;
	ret =  IOServiceGetMatchingServices(kIOMasterPortDefault, IOServiceMatching(SERIAL_DEVICE_TYPE), &iter);
	if(ret != KERN_SUCCESS) {
		return -2;
	}
	while((IOIteratorIsValid(iter) != false) && ((serialDevice = IOIteratorNext(iter)) != 0)) {
		if(getSerialPath(path, serialDevice) == 0) {
			usbDevice = getUSBParent(serialDevice);
			if(usbDevice != 0) {
				if(identifyRTk(usbDevice) == 1) {
					return 0;
				}  
			}
		}
	}
	IOObjectRelease(iter);
	memset(path, 0, PATH_MAX + 1);
	return -1;
}