#pragma once
#include <Arduino.h>
#include "GraphItem.h"

class GraphAutoScaler
{
private:
	GraphItem* _itemListRoot = nullptr;
	GraphItem* _itemListEnd = nullptr;
	int _numberOfItemsInList = 0;
	int _numberOfValuesToTrack;

	float _maximum = 0;
	int _currentSlotOfMaximum = 0;
	float _minimum = 0;
	int _currentSlotOfMinimum = 0;


	void RecalculateMaxMins();

public:
	GraphAutoScaler();
	GraphAutoScaler(int numberOfValuesToTrack);
	~GraphAutoScaler();
	
	void AddItem(float temperature);
	float GetMax() { return _maximum; }
	float GetMin() { return _minimum; }
};

