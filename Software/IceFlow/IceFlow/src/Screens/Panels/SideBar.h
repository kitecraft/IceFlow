#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Widgets/PopUpMenu.h"
#include "Icons/SettingsIcon.h"

#define SIDEBAR_NUMBER_ICONS 5

#define SIDEBAR_W 48
#define SIDEBAR_H (240 - 12) //228

#define ICON_H_PADDING 2
#define ICON_WIDTH (SIDEBAR_W - ICON_H_PADDING)
#define ICON_X ICON_H_PADDING
#define ICON_HEIGHT (SIDEBAR_H / SIDEBAR_NUMBER_ICONS) //43
#define ICON_V_PADDING ((SIDEBAR_H - (ICON_HEIGHT*SIDEBAR_NUMBER_ICONS)) / (SIDEBAR_NUMBER_ICONS-1))  //u figure it out

//1st
#define SETTINGS_ICON_Y 0 

//5th
//#define SETTINGS_ICON_Y ((ICON_HEIGHT + ICON_V_PADDING)*4)

enum ICONID {
	NO_ICONID = -1,
	SettingsIconID = 0,
};

class SideBar
{
private:
	TFT_eSPI* _tft;
	DMCoordinates _coordinates;
	SettingsIcon* _settingsIcon = nullptr;
	PopUpMenu _popUpMenu;
	ICONID _activeIcon = NO_ICONID;
public:
	SideBar();
	SideBar(TFT_eSPI* tft, DMCoordinates coordinates);
	~SideBar();

	void Draw();
	bool Touched(int x, int y);
};

