#pragma once
#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include "../Display_Defs.h"
#include "../Utilities/StarsideCoordinates.h"
#include "../Utilities/StarsideTheme.h"
#include "../Utilities/Display_Enums.h"
#include "../../Utilities/StarsideError.h"

class ScreenBase
{
private:
	StarsideError localError;

public:
	ScreenBase(TFT_eSPI *newTFT);
	~ScreenBase();
	bool SPIFFS_Load_Fonts();
	
	TFT_eSPI *TFT;

	void DrawBoxWithBorderAndDropShadow(TFT_eSprite *sprite, StarsideCoordinates coords, StarsideTheme theme);
	void DrawBoxWithBorderAndDropShadow(StarsideCoordinates coords, StarsideTheme theme);
	void DrawBorderandDropShadow(StarsideCoordinates coords, StarsideTheme theme);
	void DrawBorderandDropShadow(TFT_eSprite *sprite, StarsideCoordinates coords, StarsideTheme theme);

	bool CheckForErrorOnLast();
	StarsideError GetLastError();

	void DisplayFreeHeap();
};

