#pragma once
#include "esp32-hal-psram.h"
#include "esp32/spiram.h"


static void PrintMemUseage() {

	Serial.println("========================================");
	Serial.printf("PSRAM first 4MB size : %u \n", ESP.getPsramSize());
	Serial.printf("PSRAM first 4MB free : %u \n", ESP.getMaxAllocPsram());
	Serial.println("========================================");
	Serial.printf("Internal RAM  size   : %u \n", ESP.getHeapSize());
	Serial.printf("Internal RAM  free   : %u \n", ESP.getFreeHeap());
	Serial.println("========================================");
}