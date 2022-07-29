#include "DisplayManager.h"
#include "Utilities/DisplayQueue.h"
#include "Utilities/CommandQueue.h"
#include "../Utilities/MemUseage.h"

DisplayManager::DisplayManager()
{
}

DisplayManager::~DisplayManager()
{
}

//You should call this as early in setup() as possible.
//And only once.
//Use DisplayManager_Config.h to configure the Display
void DisplayManager::Init()
{
	DisplayQueue.Init();
	CommandQueue.Init();

	_tft.init();
#ifdef DM_USE_DMA
	_tft.initDMA();
#endif

	_tft.setRotation(DM_SCREEN_ROTATION);

#ifdef  DM_TOUCH_ENABLED
	if (CalibrationRequired()) {
		Serial.println("Calibrating touch....");
		CalibrateTouch();
	}
#endif

	xTaskCreatePinnedToCore(
		DisplayManagerTask,
		"DisplayManager",
		DM_TASK_STACK_SIZE,
		nullptr,
		DM_PRIORITY,
		&_taskHandle,
		DM_CORE
	);

	_tft.setCursor(10, 10);
	_tft.setTextSize(10);
	_tft.print("Started");
}

#ifdef  DM_TOUCH_ENABLED
void DisplayManager::HandleTouch()
{
	if (_currentScreen.HandleTouchPoint == nullptr) {
		return;
	}

	uint16_t x, y;
	if (_tft.getTouch(&x, &y)) {
		if (_nextTouch < millis()) {
			_currentScreen.HandleTouchPoint(x, y);
			_nextTouch = millis() + TOUCH_DEBOUNCE_TIME;
		}
	}
}

bool DisplayManager::CalibrationRequired()
{
	if (!SPIFFS.begin()) {
		Serial.println("DisplayManager::CalibrationRequired(): SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}


	String fileName = CALIBRATION_FILENAME;
	if (!SPIFFS.exists(fileName.c_str())) {
		return true;
	}

	File f = SPIFFS.open(fileName.c_str(), "r");
	if (!f || !(f.readBytes((char*)_calibrationData, 14) == 14)) {
		return true;
	}
	f.close();

	_tft.setTouch(_calibrationData);
	return false;
}

void DisplayManager::CalibrateTouch()
{
	if (!SPIFFS.begin()) {
		Serial.println("DisplayManager::CalibrateTouch(): SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}

	_tft.fillScreen(TFT_BLACK);
	_tft.setTextColor(TFT_WHITE, TFT_BLACK);
	_tft.setCursor(20, 0);
	_tft.setTextFont(2);
	_tft.setTextSize(1);
	_tft.println("Touch corners as indicated");
	_tft.setTextFont(1);
	_tft.println();

	_tft.calibrateTouch(_calibrationData, TFT_MAGENTA, TFT_BLACK, 15);
	_tft.setTouch(_calibrationData);

	String fileName = CALIBRATION_FILENAME;
	File f = SPIFFS.open(fileName.c_str(), "w");
	if (f) {
		f.write((const unsigned char*)_calibrationData, 14);
		f.close();
		_tft.fillScreen(TFT_GREEN);
		_tft.setTextColor(TFT_BLACK, TFT_GREEN);
		_tft.println("Calibration complete!");
	}
	else {
		Serial.print("Failed to write touch calibration file:  '");
		Serial.print(f.getWriteError());
		Serial.println("'");

		_tft.fillScreen(TFT_RED);
		_tft.setTextColor(TFT_RED, TFT_GREEN);
		_tft.println("Calibration failed to write file.");
	}
}
#endif // DM_TOUCH_ENABLED

void DisplayManager::LoadNewScreen(int screenID)
{
	//Serial.print("\nLoading new screen: ");
	//Serial.println(screenID);
	//PrintMemUseage();
	ScreenContainer newScreen;
	for (int i = 0; i < _dmScreenContainers.size(); i++) {
		if (_dmScreenContainers.at(i).key == screenID) {
			newScreen = _dmScreenContainers.at(i).screenContainer;
			break;
		}
	}

	//Return if fail to find a matching screen ID
	//or if 1 of the 2 required callbacks is missing
	if (newScreen.CreateScreen == nullptr || newScreen.DestroyScreen == nullptr) {
		return;
	}
	if (_currentScreen.DestroyScreen != nullptr) {
		_currentScreen.DestroyScreen();
		//Serial.println("\nScreen destroyed");
		//PrintMemUseage();
	}
	_currentScreen = newScreen;
	_currentScreen.CreateScreen(&_tft);
	//Serial.println("\nScreen created");
	//PrintMemUseage();
}

void DisplayManager::HandleDisplayQueue()
{
	DisplayQueueItem queueItem;
	DISPLAY_COMMANDS command;
	while (DisplayQueue.GetNextItem(command, queueItem)) {
		/*
		Serial.print("Got queue item: ");
		Serial.print(command);
		Serial.print(" | ");
		Serial.println(queueItem.displayUpdateKey);
		*/
		switch (command) {
		case DISPLAY_COMMAND_CHANGE_SCREEN:
			LoadNewScreen(queueItem.displayUpdateKey);
			break;
		case DISPLAY_COMMAND_UPDATE_VALUE:
			if (_currentScreen.UpdateScreen != nullptr) {
				_currentScreen.UpdateScreen(queueItem.displayUpdateKey, queueItem.payload);
			}
			break;
		default:
			Serial.println(F("DisplayManager::DispatchCommand - Something has gone horribly wrong"));
			break;
		}
		
		vTaskDelay(1);
	}


	vTaskDelay(1);
}

//DO NOT CAll THIS DIRECTLY
//This is the main 'thread' for the
//DisplayManager.  It is started from Init()
void DisplayManager::Run()
{
	while (true) {
#ifdef  DM_TOUCH_ENABLED
		HandleTouch();
#endif // DM_TOUCH_ENABLED

		HandleDisplayQueue();
		if (_currentScreen.UpdateScreenOnInterval != nullptr) {
			_currentScreen.UpdateScreenOnInterval();
		}
	}
}


//Add a new screen by defineing a key and supplying
//a ScreenContainer object with the callbacks defined in your 
//Screen classes header file. (See examples)
void DisplayManager::AddScreen(int16_t key, ScreenContainer screen)
{
	_dmScreenContainers.push_back(DMScreenContainer(key, screen));
}


DisplayManager Display;