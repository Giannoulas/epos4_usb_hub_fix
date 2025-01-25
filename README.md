# EPOS4 USB Hub Fix

The port names and node IDs of EPOS4 controllers can be problematic when using USB hubs. A solution found while working on Hermes Team, https://www.hermesteam.eu/. 

## Tested Devices

This solution was tested on the following Maxon EPOS 4 Controllers using a Linux Machine:
- Maxon EPOS 4 50/15: https://www.maxongroup.com/maxon/view/product/control/Positionierung/EPOS-4/520886
- Maxon EPOS 4 50/5: https://www.maxongroup.com/maxon/view/product/control/Positionierung/EPOS-4/541718

## Problem Description

When multiple EPOS4 controllers are connected to a Linux device via USB hubs, the controller IDs and their corresponding USB port assignments may change at every startup, even if the physical connections to the hardware ports remain unchanged.

Maxon recommends avoiding USB hubs where possible. Refer to these links for more information:
- https://support.maxongroup.com/hc/en-us/articles/360014218020-EPOS4-IDX-Action-steps-in-case-of-failing-USB-connection
- https://support.maxongroup.com/hc/en-us/articles/360017773999-Sporadic-USB-communication-errors

## Our Solution

In our setup, avoiding USB hubs was not an option as we needed to connect many controllers via USB. To address the issue, we devised a trial-and-error method to match USB ports to IDs.

### Approach

We use the VCS_GetFaultState function to validate the correctness of USB port-to-ID assignments. The process involves:
1. Checking all combinations by opening and closing devices on different ports.
2. Skipping USB ports that have already been assigned to an ID.

The current implementation supports up to 999 devices, though this limit can be easily extended.

### Code Overview

Key Files:
- value_assigner.cpp: Contains the main functions for the port-to-ID matching logic.
- value_assigner.hpp: Defines two structs and a function template. Also includes instructions for the function arguments.
- Definitions.h, that is the file that one can download from the maxon website as part of the linux library zip

Main Function:
int value_assigner(int number_of_devices, unsigned short *available_ids, usb_and_id *usb_and_id_array)

This function performs the USB port-to-ID assignment. Use the usb_and_id_array to retrieve the matched USB ports and IDs. Refer to value_assigner.hpp for detailed instructions on the arguments.

## Future Considerations

- Optimization: The current method checks all ports for a given ID. It would be faster to check all IDs for a port instead as it would not require constant opening and closing. This optimization was not implemented during initial development.
- Multithreading: Introducing threads could significantly speed up the process.

However, all in all, the current implementation is good enough.

## Acknowledgments

This work was developed by the Hermes Team, https://www.hermesteam.eu/, with Maxon as a sponsor.
