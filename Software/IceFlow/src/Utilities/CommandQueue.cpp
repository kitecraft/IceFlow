#include "CommandQueue.h"

CommandQueue::CommandQueue()
{
	ArduinoQueue<COMMAND_QUEUE_ITEM> commandQueue(5);
	commandQueueMutex = portMUX_INITIALIZER_UNLOCKED;
}

void CommandQueue::AddItemToQueue(STARSIDE_COMMANDS cmd, String value)
{
	COMMAND_QUEUE_ITEM qi;
	qi.command = cmd;
	qi.value = value;

	portENTER_CRITICAL(&commandQueueMutex);
	commandQueue.enqueue(qi);
	portEXIT_CRITICAL(&commandQueueMutex);
}

bool CommandQueue::IsQueueEmpty()
{
	return commandQueue.isEmpty();
}

COMMAND_QUEUE_ITEM CommandQueue::GetNextItem()
{
	COMMAND_QUEUE_ITEM ret;

	portENTER_CRITICAL(&commandQueueMutex);
	ret = commandQueue.dequeue();
	portEXIT_CRITICAL(&commandQueueMutex);

	return ret;
}