#pragma once
#include <Arduino.h>
#include "../../DisplayManager_Config.h"


class DisplayQueueItem {
public:
	//the name of the event
	int displayUpdateKey = -1;

	//the value to be used in the event
	char payload[PAYLOAD_MAX_LENGTH];
	
	
	DisplayQueueItem() {}
	
	//Used to store a screenchange event
	DisplayQueueItem(int key) {
		displayUpdateKey = key;
	}

	//Used to store a screen update event
	//key:  used by the screen class to determine what widget to update
	//data:  the value to update the widge with
	//len:  the length of data
	DisplayQueueItem(int key, const char* data) {
		strlcpy(payload, data, PAYLOAD_MAX_LENGTH);
		displayUpdateKey = key;
	}

	//Destructor will free the payload if necessary
	~DisplayQueueItem() {
	}
};
