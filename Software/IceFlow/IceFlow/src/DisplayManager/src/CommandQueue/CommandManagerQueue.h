#pragma once
#include <Arduino.h>
#include "CommandManagerQueueItem.h"

class CommandManagerQueue
{
private:
	CommandManagerQueueItem* _queueRoot = nullptr;
	CommandManagerQueueItem* _queueEnd = nullptr;
	portMUX_TYPE _commandQueueMutex;

	void* (*cm_malloc)(size_t size) = nullptr;
	void QueueItem(CommandQueueItem newQueueItem);

public:
	~CommandManagerQueue() {}
	void Init();
	bool IsEmpty();

	void QueueCommand(int key);
	void QueueCommandAndValue(int key, const char* val);
	bool GetNextItem(int &key, char *val);
};

