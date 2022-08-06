#pragma once
#include <Arduino.h>
#include "GraphItem.h"

#define GAS_NO_VALUE -9999

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

	GraphItem* _userAccessPointer = nullptr;

	void RecalculateMaxMins();
	float GetUserAccessValue();

public:
	GraphAutoScaler();
	GraphAutoScaler(int numberOfValuesToTrack);
	~GraphAutoScaler();
	
	//returns true if caused a MinMax recalc
	bool AddItem(float temperature);
	float GetMax() { return _maximum; }
	float GetMin() { return _minimum; }

	float GetNewest();
	float GetNext();

	int GetSize() { return _numberOfItemsInList; }
	void Clear();
};

