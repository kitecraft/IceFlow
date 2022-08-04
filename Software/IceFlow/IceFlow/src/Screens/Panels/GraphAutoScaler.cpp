#include "GraphAutoScaler.h"


GraphAutoScaler::GraphAutoScaler()
{
}

GraphAutoScaler::GraphAutoScaler(int numberOfValuesToTrack)
{
	_numberOfValuesToTrack = numberOfValuesToTrack;
}

GraphAutoScaler::~GraphAutoScaler()
{
	while (_itemListRoot != nullptr) {
		GraphItem* currItem = _itemListRoot;
		_itemListRoot = currItem->nextItem;
		free(currItem);
	}
}

void GraphAutoScaler::AddItem(float temperature)
{
	GraphItem* currentItem = (GraphItem*)ps_malloc(sizeof(GraphItem));
	currentItem->temperature = temperature;
	currentItem->nextItem = NULL;
	currentItem->prevItem = NULL;
	//Serial.print("ADDING ITEM: ");
	//Serial.println(currentItem->ToString());

	if (_itemListRoot == nullptr) {
		_maximum = currentItem->temperature;
		_minimum = currentItem->temperature;
		_currentSlotOfMaximum = 0;
		_currentSlotOfMinimum = 0;

		_itemListRoot = currentItem;
		_itemListEnd = currentItem;
		_numberOfItemsInList++;
		_itemListRoot->prevItem = NULL;
		return;
	}
	else {
		currentItem->prevItem = _itemListEnd;
		_itemListEnd->nextItem = currentItem;
		_itemListEnd = currentItem;
		_numberOfItemsInList++;

		if (currentItem->temperature > _maximum) {
			_maximum = currentItem->temperature;
			_currentSlotOfMaximum = 0;
		}
		else {
			_currentSlotOfMaximum++;
		}
		if (currentItem->temperature < _minimum) {
			_minimum = currentItem->temperature;
			_currentSlotOfMinimum = 0;
		}
		else {
			_currentSlotOfMinimum++;
		}
	}

	if (_numberOfItemsInList >= _numberOfValuesToTrack) {
		GraphItem* tmpItem = _itemListRoot;
		_itemListRoot = tmpItem->nextItem;
		_itemListRoot->prevItem = NULL;
		free(tmpItem);
		_numberOfItemsInList--;
	}
	
	if (_currentSlotOfMaximum >= _numberOfValuesToTrack ||
		_currentSlotOfMinimum >= _numberOfValuesToTrack) {
		RecalculateMaxMins();
	}
}

void GraphAutoScaler::RecalculateMaxMins()
{
	_maximum = -9999;
	_currentSlotOfMaximum = 0;
	_minimum = 9999;
	_currentSlotOfMinimum = 0;

	GraphItem* currItem = _itemListEnd;
	if (currItem == nullptr) {
		return;
	}

	_maximum = currItem->temperature;
	_minimum = currItem->temperature;


	currItem = currItem->prevItem;
	int counter = 0;
	while (currItem != NULL) {
		if (currItem->temperature > _maximum) {
			_maximum = currItem->temperature;
			_currentSlotOfMaximum = counter;
		}
		if (currItem->temperature < _minimum) {
			_minimum = currItem->temperature;
			_currentSlotOfMinimum = counter;
		}
		
		currItem = currItem->prevItem;
		counter++;
		//if (counter % 10 == 0) {
		//	vTaskDelay(1);
		//}
	}
	
	/*
	Serial.print("Maximum Temp found is: ");
	Serial.println(_maximum);
	Serial.print("Minimum Temp found is: ");
	Serial.println(_minimum);
	Serial.printf("Total count is: %i  and counter is %i\n",_numberOfValuesToTrack, counter);
	*/
}