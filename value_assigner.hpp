#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "Definitions.h"
#include <string.h>

//this code supports up to 999 devices, 
//to add more devices, go to the function usb_names_create and make necessary changes
#define MAX_DEVICES 999 


#define FAILED 1
#define SUCCESS 0


//struct with the default controllers' info, change if necessary
typedef struct default_data{
	
	
	char deviceName[256] = "EPOS4";
	char protocolStackName[256] = "MAXON SERIAL V2";
	char interfaceName[256] = "USB";
	int baudrate =1000000;

}default_data;

//struct containing the matched USB port and the id
typedef struct usb_and_id{
    std::string usb_name;
    unsigned short node_id;

}usb_and_id;

//////////////////////
//the function that assigns the ids to the USB ports, returns 0 if successful, 1 if failed

//number_of_devices:            is the number of controllers
//available_ids:                is the node ids of the controllers

//ARRAY OF MATCHING
//usb_and_id_array:             the ids matched with the USB ports will be stored in the usb_and_id_array 
//the length of the array should be equal to the number_of_devices
////////////////////
int value_assigner(int number_of_devices, unsigned short *available_ids, usb_and_id *usb_and_id_array);