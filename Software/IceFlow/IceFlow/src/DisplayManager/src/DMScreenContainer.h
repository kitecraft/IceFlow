#pragma once
#include <Arduino.h>
#include "../Utilities/ScreenContainer.h"

struct DMScreenContainer {
	int key = -1;
	ScreenContainer screenContainer;
	DMScreenContainer() {}
	DMScreenContainer(int newKey, ScreenContainer newScreenContainer) {
		key = newKey;
		screenContainer = newScreenContainer;
	}
	DMScreenContainer(const DMScreenContainer &orig) {
		key = orig.key;
		screenContainer = orig.screenContainer;
	}
};