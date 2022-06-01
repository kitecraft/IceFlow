#pragma once
#include <Arduino.h>
#include <ArduinoQueue.h>
#include "Display_Enums.h"

class DISPLAY_QUEUE_ITEM {
public:
	DISPLAY_COMMANDS command = DISPLAY_COMMAND_NO_COMMAND;
	DISPLAY_UPDATE_KEYS displayUpdateKey = DISPLAY_UPDATE_KEY_NO_KEY;
	String payload = "";

	DISPLAY_SCREEN_TYPES loadScreenType = DISPLAY_SCREEN_TYPE_NO_SCREEN;
	DISPLAY_QUEUE_ITEM() {};
};

class DisplayQueue
{
private:
	ArduinoQueue<DISPLAY_QUEUE_ITEM> displayQueue;
	portMUX_TYPE displayQueueMutex;

public:
	DisplayQueue();
	void AddItemToQueue(DISPLAY_COMMANDS inCommand, DISPLAY_UPDATE_KEYS inKey, String inPayload = "");
	void AddScreenChangeToQueue(DISPLAY_SCREEN_TYPES inScreenType);
	bool IsQueueEmpty();
	void ClearQueue();
	DISPLAY_QUEUE_ITEM GetNextItem();
};
