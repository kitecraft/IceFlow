#include "DisplayQueue.h"
DisplayQueue::DisplayQueue()
{
	ArduinoQueue<DISPLAY_QUEUE_ITEM> displayQueue(20);

	displayQueueMutex = portMUX_INITIALIZER_UNLOCKED;
}

void DisplayQueue::AddItemToQueue(DISPLAY_COMMANDS inCommand, DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	DISPLAY_QUEUE_ITEM qi;
	qi.command = inCommand;
	qi.displayUpdateKey = inKey;
	qi.payload = inPayload;

	portENTER_CRITICAL(&displayQueueMutex);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMutex);
}

void DisplayQueue::AddScreenChangeToQueue(DISPLAY_SCREEN_TYPES inScreenType)
{

	DISPLAY_QUEUE_ITEM qi;
	qi.command = DISPLAY_COMMAND_CHANGE_SCREEN;
	qi.loadScreenType = inScreenType;

	portENTER_CRITICAL(&displayQueueMutex);
	displayQueue.enqueue(qi);
	portEXIT_CRITICAL(&displayQueueMutex);
}

bool DisplayQueue::IsQueueEmpty()
{
	return displayQueue.isEmpty();
}

void DisplayQueue::ClearQueue()
{
	portENTER_CRITICAL(&displayQueueMutex);
	while (!displayQueue.isEmpty())
	{
		displayQueue.dequeue();
	}
	portEXIT_CRITICAL(&displayQueueMutex);
}

DISPLAY_QUEUE_ITEM DisplayQueue::GetNextItem()
{
	DISPLAY_QUEUE_ITEM ret;

	portENTER_CRITICAL(&displayQueueMutex);
	ret = displayQueue.dequeue();
	portEXIT_CRITICAL(&displayQueueMutex);

	return ret;
}