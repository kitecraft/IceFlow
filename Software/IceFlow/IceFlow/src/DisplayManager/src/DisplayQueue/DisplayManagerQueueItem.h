#pragma once
#include <Arduino.h>
#include "DisplayQueueCommands.h"
#include "DisplayQueueItem.h"

typedef struct DisplayManagerQueueItem {
	DISPLAY_COMMANDS command = DISPLAY_COMMAND_NO_COMMAND;
	DisplayQueueItem item;
	DisplayManagerQueueItem *nextItem = nullptr;
}DisplayManagerQueueItem;