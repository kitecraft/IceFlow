#pragma once
#include <Arduino.h>
#include <ArduinoQueue.h>
#include "Command_Enums.h"

class COMMAND_QUEUE_ITEM {
public:
	STARSIDE_COMMANDS command = STARSIDE_CMD_NO_COMMAND;
	String value = "";
	COMMAND_QUEUE_ITEM() {};
};

class CommandQueue
{
private:
	ArduinoQueue<COMMAND_QUEUE_ITEM> commandQueue;
	portMUX_TYPE commandQueueMutex;

public:
	CommandQueue();
	void AddItemToQueue(STARSIDE_COMMANDS cmd, String value = "");
	bool IsQueueEmpty();
	COMMAND_QUEUE_ITEM GetNextItem();
};

extern CommandQueue g_commandQueue;
