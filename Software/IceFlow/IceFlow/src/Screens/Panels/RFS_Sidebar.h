#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"

#define RFS_SIDEBAR_W 48
#define RFS_SIDEBAR_H 240
#define RFS_SIDEBAR_X (320 - RFS_SIDEBAR_W)
#define RFS_SIDEBAR_Y 0

class RFS_Sidebar
{
private:
	TFT_eSPI* _tft;


public:
	RFS_Sidebar();
	RFS_Sidebar(TFT_eSPI* tft);
	~RFS_Sidebar();

	void Draw();
};

