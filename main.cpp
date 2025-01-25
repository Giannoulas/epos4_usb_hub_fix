#include "main.hpp"


int main(){
	
	//array with available ids
	unsigned short available_ids[3] = {1, 2, 3};

	//value assigner return struct 
	usb_and_id usb_and_id_array[3];

	//number of devices
	int number_of_devices = 3;

	//call the value assigner function, check value_assigner.cpp for more info
	if (value_assigner(number_of_devices, available_ids, usb_and_id_array)){
		std::cout << "Error in value assigner\n";
		return 1;
	}

	//print the results	
	for (int i = 0; i < number_of_devices; i++){
		std::cout << "USB name: " << (usb_and_id_array+i)->usb_name << " Node id: " << (usb_and_id_array+i)->node_id << std::endl;
	}
	return 0;
	
}


