#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../../IceFlow_Config.h"
#include "../../Utilities/StarsideCoordinates.h"
#include "../../Utilities/StarsideTheme.h"

class PanelBase
{
private:

public:
	TFT_eSPI* TFT;
	void Init(TFT_eSPI* newTFT);
	void DrawRoundedBox(TFT_eSprite* sprite, StarsideCoordinates coordinates, StarsideTheme theme, int radius, int border);
};

