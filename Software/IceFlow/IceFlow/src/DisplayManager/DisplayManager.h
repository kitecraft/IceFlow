#pragma once
#include "Arduino.h"
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "DisplayManager_Config.h"
#include "src/DMScreenContainer.h"

using namespace std;
#include <vector>

class DisplayManager
{
private:
	TFT_eSPI _tft;
	vector<DMScreenContainer> _dmScreenContainers;
	ScreenContainer _currentScreen;
	TaskHandle_t _taskHandle = nullptr;

	void HandleDisplayQueue();
	void LoadNewScreen(int screenID);

#ifdef  DM_TOUCH_ENABLED
	uint16_t _calibrationData[5];
	unsigned long _nextTouch = 0;
	void HandleTouch();
	bool CalibrationRequired();
	void CalibrateTouch();
#endif //  DM_TOUCH_ENABLED

public:
	DisplayManager();
	~DisplayManager();
	void Init();
	void Run();

	void AddScreen(int16_t key, ScreenContainer newScreen);

#ifdef  DM_TOUCH_ENABLED
	//Delete the touchscreen calibration file.
	//The next time your run Init(), a new calibration
	//routine will be run, and a new set of data points saved.
	void DeleteTouchCalibrationFile() { SPIFFS.remove(CALIBRATION_FILENAME); }
#endif //  DM_TOUCH_ENABLED
};

//The Display Manager object.
extern DisplayManager Display;

//The task which runs the Display Managers goodness engine.
void static IRAM_ATTR DisplayManagerTask(void*)
{
	Display.Run();
}