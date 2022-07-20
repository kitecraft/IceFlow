#pragma once
#include <Arduino.h>
#include "../../DisplayManager_Config.h"
#include "CommandQueueItem.h"

typedef struct CommandManagerQueueItem {
	CommandQueueItem item;
	CommandManagerQueueItem* nextItem = nullptr;
}CommandManagerQueueItem;