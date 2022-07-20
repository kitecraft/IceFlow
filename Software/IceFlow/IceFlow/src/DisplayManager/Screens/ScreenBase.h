#pragma once
#include <Arduino.h>
#include "Screens_Config.h"
#include <TFT_eSPI.h>
#include "../DisplayManager/Utilities/DisplayQueue.h"

class ScreenBase
{
private:

protected:
	TFT_eSPI* _tft;

public:
	ScreenBase(TFT_eSPI* tft);
};

