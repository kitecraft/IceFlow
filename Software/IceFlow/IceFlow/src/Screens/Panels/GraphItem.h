#pragma once
#include <Arduino.h>

typedef struct GraphItem {
	float temperature = 0;
	GraphItem* nextItem = nullptr;
	GraphItem* prevItem = nullptr;

	GraphItem() {
		temperature = 0;
		nextItem = nullptr;
		prevItem = nullptr;
	}
	GraphItem(const GraphItem& orig) {
		temperature = orig.temperature;
		nextItem = orig.nextItem;
		prevItem = orig.prevItem;
	}
	String ToString() {
		String ret = "temperature_: " + String(temperature);
		return ret;
	}
}GraphItem;