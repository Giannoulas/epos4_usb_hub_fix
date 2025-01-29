#include "value_assigner.hpp"


//creating the array of usb names, 
//the array will look like this: USB0, USB1, USB2, USB3, ... ,USBN, where N is the number of devices
char ** usb_names_create(int devices_num){

    char ** usb_names;
    const char usb_string[3] = {'U', 'S', 'B'};
    int i;

    //getting memory for the array of usbs
    usb_names = (char **) malloc(devices_num * sizeof(char *));
    if (usb_names == NULL){
        
        return NULL;
    }

    for (i = 0; i < devices_num; i++){
        
        //changing the size of the array depending on the number of devices
        //code works for up to 999 devices
        if (i < 10){
            *(usb_names + i) = (char *) malloc(5 * sizeof(char));
            if (*(usb_names + i) == NULL){
                return NULL;
            }
        }
        else if (i < 100){
            *(usb_names + i) = (char *) malloc(6 * sizeof(char));
            if (*(usb_names + i) == NULL){
                return NULL;
            }
        }
        else{
            *(usb_names + i) = (char *) malloc(7 * sizeof(char));
            if (*(usb_names + i) == NULL){
                return NULL;
            }
        }

        *(*(usb_names + i) + 0) = usb_string[0];
        *(*(usb_names + i) + 1) = usb_string[1];
        *(*(usb_names + i) + 2) = usb_string[2];

        sprintf((*(usb_names + i)+3), "%d", i); 

    }

    return usb_names;


}

//checking if a specific id is assigned to a specific usb port
int check_successful_assign(char *usb_name, unsigned short current_id, default_data default_values){

 
    char port_name[256];
	void* local_keyHandle;
	int lResult = FAILED;

	int return_value = 0;

	unsigned int pErrorCode;
    strcpy(port_name, usb_name);
	//we open the device on a certain port without knowing its id
    local_keyHandle = VCS_OpenDevice(default_values.deviceName, default_values.protocolStackName, default_values.interfaceName, port_name, &pErrorCode);

	if (local_keyHandle == 0 || pErrorCode !=0){
        std::cout << "VCS_OpenDevice failed with error code: " << pErrorCode << std::endl;
		return 2;
	}

	int oIsFault = 0;
	//we will try to check if it is in fault state. If the function is successful, then we can assing an id
    //if the function is not successful, then the id does not match the port that gave us the keyHandle
    ///////////////////////////////////////
    //if the VCS_GetFaultState fails for all ids given to the check_successful_assign function
    //then either the id doesnt exist, or there is some problem with the controller which causes the GetFaultState to fail
    ///////////////////////////////////////
    if(VCS_GetFaultState(local_keyHandle, current_id, &oIsFault, &pErrorCode)== 0 ) 
    {
		//failed
        return_value = 1;
	}
	else {
        
        //success
		return_value = 0;
	}

	if (VCS_CloseDevice(local_keyHandle, &pErrorCode) ==0){
        std::cout << "VCS_CloseDevice failed with error code: " << pErrorCode << std::endl;
		return 2;

	}

	return return_value;



}

//the function the ids to the USB ports
int value_assigner(int number_of_devices, unsigned short *available_ids, usb_and_id *usb_and_id_array){
 
	int i, k;

    char **usb_names;
	int check_value;

    default_data default_values;

    if (number_of_devices > MAX_DEVICES){
        std::cout << "\nnumber of devices is too high\n";
        return 1;
    }

    //create the array of usb names
    usb_names = usb_names_create(number_of_devices);
    if (usb_names == NULL){
        std::cout << "\nError in malloc\n";
        return 1;
    }

    //copying the usb names to the struct in ascending order
    for (i = 0; i < number_of_devices; i++){
        (usb_and_id_array+i)->usb_name = *(usb_names + i);
    }
	

	for (k = 0; k < number_of_devices; k++){
		for (i = 0; i < number_of_devices; i++){
			//if the usb has been used for some id, skip it
            if (**(usb_names +i) != 'U'){
                continue;
            }
            
            check_value = check_successful_assign(usb_names[i], *(available_ids+k), default_values);

			if(check_value == 2){   //critical error, some problem with the controller
                std::cout << "\nCritical error, some device not opening or closing. Check if the number of devices is correct. USB name: " << usb_names[i] << "\n";
				return 1;
			}
			else if (check_value == 0){ //success
				(usb_and_id_array+i)->node_id = *(available_ids + k);   //update the return values
                **(usb_names +i) = 'X'; //mark the usb as used
				break;
			}
			else if (i == number_of_devices - 1){
				//no usb ports matched for the node id
                //could be the node id does not exist, or there is some issue with the controller or connection
                std::cout << "\nno usbs matched for node id " << *(available_ids + k) << "\n";
				return 1;
			}

		}
	}

    //free the memory
    for (i = 0; i < number_of_devices; i++){
        free(*(usb_names + i));
    }
    free(usb_names);

    return 0;
}


