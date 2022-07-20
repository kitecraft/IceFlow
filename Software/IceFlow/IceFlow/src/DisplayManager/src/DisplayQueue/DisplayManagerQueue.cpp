#include "DisplayManagerQueue.h"

void DisplayManagerQueue::Init()
{
	_displayQueueMutex = portMUX_INITIALIZER_UNLOCKED;
	if (psramFound()) {
		dm_malloc = ps_malloc;
	}
	else {
		dm_malloc = malloc;
	}
}

void DisplayManagerQueue::QueueScreenChange(int newScreenID)
{
	QueueItem(DISPLAY_COMMAND_CHANGE_SCREEN, DisplayQueueItem(newScreenID));
}

void DisplayManagerQueue::QueueKey(int key)
{
	QueueItem(DISPLAY_COMMAND_UPDATE_VALUE, DisplayQueueItem(key));
}

void DisplayManagerQueue::QueueKeyAndValue(int key, const char* val)
{
	QueueItem(DISPLAY_COMMAND_UPDATE_VALUE, DisplayQueueItem(key, val));
}

bool DisplayManagerQueue::GetNextItem(DISPLAY_COMMANDS& command, DisplayQueueItem& queueItem)
{
	if (IsEmpty())
	{
		return false;
	}

	portENTER_CRITICAL(&_displayQueueMutex);
	queueItem = _queueRoot->item;
	command = _queueRoot->command;
	DisplayManagerQueueItem* newRoot = _queueRoot->nextItem;
	free(_queueRoot);
	_queueRoot = newRoot;

	if (_queueRoot == nullptr) {
		_queueEnd = nullptr;
	}
	portEXIT_CRITICAL(&_displayQueueMutex);

	return true;
}

void DisplayManagerQueue::QueueItem(DISPLAY_COMMANDS command, DisplayQueueItem newQueueItem)
{
	DisplayManagerQueueItem* newDMQueueItem = (DisplayManagerQueueItem*)dm_malloc(sizeof(DisplayManagerQueueItem));
	newDMQueueItem->command = command;
	newDMQueueItem->item = newQueueItem;
	newDMQueueItem->nextItem = nullptr;

	portENTER_CRITICAL(&_displayQueueMutex);
	if (_queueRoot == nullptr) {
		_queueRoot = newDMQueueItem;
	}
	else {
		_queueEnd->nextItem = newDMQueueItem;
	}

	_queueEnd = newDMQueueItem;
	portEXIT_CRITICAL(&_displayQueueMutex);
}

bool DisplayManagerQueue::IsEmpty()
{
	if (_queueRoot == nullptr) {
		return true;
	}
	return false;
}


DisplayManagerQueue DisplayQueue;