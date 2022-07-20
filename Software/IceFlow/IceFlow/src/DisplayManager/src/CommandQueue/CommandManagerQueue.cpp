#include "CommandManagerQueue.h"

void CommandManagerQueue::Init()
{
	_commandQueueMutex = portMUX_INITIALIZER_UNLOCKED;
	if (psramFound()) {
		cm_malloc = ps_malloc;
	}
	else {
		cm_malloc = malloc;
	}
}

void CommandManagerQueue::QueueCommand(int key)
{
	QueueItem(CommandQueueItem(key));
}

void CommandManagerQueue::QueueCommandAndValue(int key, const char* val)
{
	QueueItem(CommandQueueItem(key, val));
}

bool CommandManagerQueue::GetNextItem(int& key, char* val)
{
	if (IsEmpty())
	{
		return false;
	}

	portENTER_CRITICAL(&_commandQueueMutex);
	CommandQueueItem queueItem = _queueRoot->item;
	CommandManagerQueueItem* newRoot = _queueRoot->nextItem;
	free(_queueRoot);
	_queueRoot = newRoot;

	if (_queueRoot == nullptr) {
		_queueEnd = nullptr;
	}
	portEXIT_CRITICAL(&_commandQueueMutex);

	key = queueItem.command;
	strlcpy(val, queueItem.payload, PAYLOAD_MAX_LENGTH);
	return true;
}

void CommandManagerQueue::QueueItem(CommandQueueItem newQueueItem)
{
	CommandManagerQueueItem* newDMQueueItem = (CommandManagerQueueItem*)cm_malloc(sizeof(CommandManagerQueueItem));
	newDMQueueItem->item = newQueueItem;
	newDMQueueItem->nextItem = nullptr;

	portENTER_CRITICAL(&_commandQueueMutex);
	if (_queueRoot == nullptr) {
		_queueRoot = newDMQueueItem;
	}
	else {
		_queueEnd->nextItem = newDMQueueItem;
	}

	_queueEnd = newDMQueueItem;
	portEXIT_CRITICAL(&_commandQueueMutex);
}

bool CommandManagerQueue::IsEmpty()
{
	if (_queueRoot == nullptr) {
		return true;
	}
	return false;
}

CommandManagerQueue CommandQueue;