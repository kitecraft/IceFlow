#pragma once
#include <Arduino.h>
#include "../../DisplayManager_Config.h"

class CommandQueueItem {
public:
	//the name of the event
	int command = -1;

	//the value to be used in the event
	char payload[PAYLOAD_MAX_LENGTH];

	CommandQueueItem() {}

	CommandQueueItem(int command_) {
		command = command_;
	}
	CommandQueueItem(int command_, const char* data) {
		strlcpy(payload, data, PAYLOAD_MAX_LENGTH);
		command = command_;
	}

	//Destructor will free the payload if necessary
	~CommandQueueItem() {
	}
};