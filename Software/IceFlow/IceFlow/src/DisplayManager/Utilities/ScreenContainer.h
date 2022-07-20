#pragma once
#include "../DisplayManager_Config.h"

typedef struct ScreenContainer {
public:
	void(*CreateScreen)(TFT_eSPI* TFT) = nullptr;
	void(*DestroyScreen)() = nullptr;
	void(*UpdateScreen)(int inKey, char *value) = nullptr;
	void(*UpdateScreenOnInterval)() = nullptr;
#ifdef DM_TOUCH_ENABLED
	void(*HandleTouchPoint)(int x, int y) = nullptr;
#endif
	ScreenContainer() {}
	ScreenContainer(
		void(*CreateScreenCallBack)(TFT_eSPI* TFT),
		void(*DestroyScreenCallBack)(),
		void(*UpdateScreenCallBack)(int inKey, char* value),
		void(*UpdateScreenOnIntervalCallBack)()
#ifdef DM_TOUCH_ENABLED
		,void(*HandleTouchPointCallBack)(int x, int y)
#endif
	) {
		CreateScreen = CreateScreenCallBack;
		DestroyScreen = DestroyScreenCallBack;
		UpdateScreen = UpdateScreenCallBack;
		UpdateScreenOnInterval = UpdateScreenOnIntervalCallBack;
#ifdef DM_TOUCH_ENABLED
		HandleTouchPoint = HandleTouchPointCallBack;
#endif
	}

	ScreenContainer(const ScreenContainer&source) {
		CreateScreen = source.CreateScreen;
		DestroyScreen = source.DestroyScreen;
		UpdateScreen = source.UpdateScreen;
		UpdateScreenOnInterval = source.UpdateScreenOnInterval;
#ifdef DM_TOUCH_ENABLED
		HandleTouchPoint = source.HandleTouchPoint;
#endif
	}
}ScreenContainer;