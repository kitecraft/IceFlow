#pragma once
#include <Arduino.h>
#include "DisplayManagerQueueItem.h"

class DisplayManagerQueue
{
private:
	DisplayManagerQueueItem* _queueRoot = nullptr;
	DisplayManagerQueueItem* _queueEnd = nullptr;
	portMUX_TYPE _displayQueueMutex;

	void*(*dm_malloc)(size_t size) = nullptr;
	void QueueItem(DISPLAY_COMMANDS command, DisplayQueueItem newQueueItem);

public:
	~DisplayManagerQueue() {}
	void Init();
	bool IsEmpty();
	void QueueScreenChange(int newScreenID);
	void QueueKey(int key);
	void QueueKeyAndValue(int key, const char* val);
	bool GetNextItem(DISPLAY_COMMANDS &command, DisplayQueueItem &queueItem);
};

