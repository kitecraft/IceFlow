#include "DisplayManager.h"
#include "Gui/SplashScreen_Interface.h"
#include "Gui/OTAScreen_Interface.h"
#include "Gui/MainScreen_Interface.h"

DisplayManager::DisplayManager()
{
}

DisplayManager::~DisplayManager()
{
}

void DisplayManager::Init()
{
	tftDisplay.init();
	tftDisplay.setRotation(3);
	tftDisplay.fillScreen(TFT_BLACK);


	if (CalibrationRequired()){
		Serial.println("Calibrating touch....");
		CalibrateTouch();
	}

	LoadNewScreen(DISPLAY_SCREEN_TYPE_SPLASH_SCREEN);
}


void DisplayManager::Run()
{
	while (true)
	{
		HandleTouch();
		DispatchCommand();

		if (UpdateCurentScreenOnInterval != NULL)		{
			UpdateCurentScreenOnInterval();
		}


		//yield();
		delay(1);
	}
}

void DisplayManager::HandleTouch()
{
	uint16_t x, y;
	if (tftDisplay.getTouch(&x, &y))	{
		if (nextTouch < millis() && HandleTouchPoint != NULL)		{
			HandleTouchPoint(x, y);
			nextTouch = millis() + TOUCH_DEBOUNCE_TIME;
		}
	}
}

void DisplayManager::LoadNewScreen(DISPLAY_SCREEN_TYPES newScreenType)
{	
	if (DestroyCurrentScreen != NULL) {
		DestroyCurrentScreen();
		DestroyCurrentScreen = NULL;
	}
	UpdateCurentScreen = NULL;
	UpdateCurentScreenOnInterval = NULL;
	HandleTouchPoint = NULL;

	switch (newScreenType) {
	case DISPLAY_SCREEN_TYPE_OTA_SCREEN:
		Serial.println("DM:  DISPLAY_SCREEN_TYPE_OTA_SCREEN");
		DestroyCurrentScreen = Destroy_OTAScreen;
		UpdateCurentScreen = Update_OTAScreen;
		UpdateCurentScreenOnInterval = Update_OTAScreen_OnInterval;
		HandleTouchPoint = Handle_OTAScreen_Touch;
		Create_OTAScreen(&tftDisplay);
		break;
	case DISPLAY_SCREEN_TYPE_SPLASH_SCREEN:
		Serial.println("DM:  DISPLAY_SCREEN_TYPE_SPLASH_SCREEN");
		DestroyCurrentScreen = Destroy_SplashScreen;
		UpdateCurentScreen = Update_SplashScreen;
		UpdateCurentScreenOnInterval = Update_SplashScreen_OnInterval;
		Create_SplashScreen(&tftDisplay);
		break;
	case DISPLAY_SCREEN_TYPE_MAIN:
		Serial.println("DM:  DISPLAY_SCREEN_TYPE_MAIN");
		DestroyCurrentScreen = Destroy_MainScreen;
		UpdateCurentScreen = Update_MainScreen;
		UpdateCurentScreenOnInterval = Update_MainScreen_OnInterval;
		HandleTouchPoint = Handle_MainScreen_Touch;
		Create_MainScreen(&tftDisplay);
		break;
		/*
	case DISPLAY_SCREEN_TYPE_HOME_SCREEN:
		//DestroyCurrentScreen = Destroy_HomeScreen;
		//UpdateCurentScreen = Update_HomeScreen;
		//UpdateCurentScreenOnInterval = Update_HomeScreen_OnInterval;
		//Create_HomeScreen(&tftDisplay, screenPositions[screenPositionID].cs_pin, allQueues);
		break;
		*/
	default:
		break;
	}
}

void DisplayManager::DispatchCommand()
{
	while (!g_displayQueue.IsQueueEmpty())	{
		DISPLAY_QUEUE_ITEM currItem = g_displayQueue.GetNextItem();

		switch (currItem.command) {
		case DISPLAY_COMMANDS::DISPLAY_COMMAND_CHANGE_SCREEN:
			LoadNewScreen(currItem.loadScreenType);
			break;
		case DISPLAY_COMMANDS::DISPLAY_COMMAND_UPDATE_VALUE:
			if (UpdateCurentScreen != NULL)
			{
				UpdateCurentScreen(currItem.displayUpdateKey, currItem.payload);
			}
			break;
		default:
			break;
		}
		vTaskDelay(1);
	}
}

void DisplayManager::UnloadOldDataFromDisplayQueue()
{
	while (!g_displayQueue.IsQueueEmpty())	{
		g_displayQueue.GetNextItem();
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
	if (!f || !(f.readBytes((char*)calibrationData, 14) == 14)) {
		return true;
	}
	f.close();

	tftDisplay.setTouch(calibrationData);
	return false;
}

void DisplayManager::CalibrateTouch()
{
	if (!SPIFFS.begin()) {
		Serial.println("DisplayManager::CalibrateTouch(): SPIFFS initialisation failed!");
		while (1) yield(); // Stay here twiddling thumbs waiting
	}

	tftDisplay.fillScreen(TFT_BLACK);
	tftDisplay.setTextColor(TFT_WHITE, TFT_BLACK);
	tftDisplay.setCursor(20, 0);
	tftDisplay.setTextFont(2);
	tftDisplay.setTextSize(1);
	tftDisplay.println("Touch corners as indicated");
	tftDisplay.setTextFont(1);
	tftDisplay.println();

	tftDisplay.calibrateTouch(calibrationData, TFT_MAGENTA, TFT_BLACK, 15);
	tftDisplay.setTouch(calibrationData);

	String fileName = CALIBRATION_FILENAME;
	File f = SPIFFS.open(fileName.c_str(), "w");
	if (f) {
		f.write((const unsigned char*)calibrationData, 14);
		f.close();
		tftDisplay.fillScreen(TFT_GREEN);
		tftDisplay.setTextColor(TFT_BLACK, TFT_GREEN);
		tftDisplay.println("Calibration complete!");
	}
	else {
		Serial.print("Failed to write file:  '");
		Serial.print(f.getWriteError());
		Serial.println("'");

		tftDisplay.fillScreen(TFT_RED);
		tftDisplay.setTextColor(TFT_RED, TFT_GREEN);
		tftDisplay.println("Calibration failed to write file.");
	}
}