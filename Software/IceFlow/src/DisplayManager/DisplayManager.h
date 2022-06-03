#pragma once
#include "Arduino.h"
#include "../IceFlow_Config.h"
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Utilities/DisplayQueue.h"

#define CALIBRATION_FILENAME "/_calData_"
#define TOUCH_DEBOUNCE_TIME 500
extern DisplayQueue g_displayQueue;

class DisplayManager
{
private:
	void(*UpdateCurentScreen)(DISPLAY_UPDATE_KEYS inKey, String inPayload) = NULL;
	void(*UpdateCurentScreenOnInterval)() = NULL;
	void(*DestroyCurrentScreen)() = NULL;
	void(*HandleTouchPoint)(int x, int y) = NULL;

	TFT_eSPI tftDisplay;

	void DispatchCommand();
	void LoadNewScreen(DISPLAY_SCREEN_TYPES screenID);
	void UnloadOldDataFromDisplayQueue();

	void HandleTouch();
	bool CalibrationRequired();
	void CalibrateTouch();
	uint16_t calibrationData[5];
	unsigned long nextTouch = 0;
public:
	DisplayManager();
	~DisplayManager();
	void Init();
	void Run();

	void DeleteTouchCalibrationFile(){ SPIFFS.remove(CALIBRATION_FILENAME); }
};

