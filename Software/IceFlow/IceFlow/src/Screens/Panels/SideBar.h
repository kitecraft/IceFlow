#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Widgets/PopUpMenu.h"
#include "Icons/SettingsIcon.h"
#include "Icons/ProfileIcon.h"
#include "Icons/ManualHeatIcon.h"
#include "Icons/ReflowIcon.h"

#define SIDEBAR_NUMBER_ICONS 5

#define SIDEBAR_W 48
#define SIDEBAR_H (240 - 12) //228

#define ICON_H_PADDING 2
#define ICON_WIDTH (SIDEBAR_W - ICON_H_PADDING)
#define ICON_X ICON_H_PADDING
#define ICON_HEIGHT (SIDEBAR_H / SIDEBAR_NUMBER_ICONS)

#define ICON_V_PADDING ((SIDEBAR_H - (ICON_HEIGHT*SIDEBAR_NUMBER_ICONS)) / (SIDEBAR_NUMBER_ICONS-1))  //u figure it out

//1st
#define SETTINGS_ICON_Y 0 

//2nd
#define PROFILE_ICON_Y ((ICON_HEIGHT + ICON_V_PADDING)*1)

//3rd
#define MANUAL_HEAT_ICON_Y ((ICON_HEIGHT + ICON_V_PADDING)*2)

//4th
#define REFLOW_ICON_Y ((ICON_HEIGHT + ICON_V_PADDING)*3)
//#define SETTINGS_ICON_Y ((ICON_HEIGHT + ICON_V_PADDING)*3)

//5th

enum SB_TOUCHED_RETURN {
	SB_TOUCHED = 0,
	SB_NOT_TOUCHED,
	SB_MENU_CLOSED,
	SB_START_MANUAL_HEAT,
	SB_START_AUTO_TUNE,
	SB_START_REFLOW,
};

enum ICONID {
	NO_ICONID = -1,
	SettingsIconID = 0,
	ManualHeatIconID,
};

class SideBar
{
private:
	TFT_eSPI* _tft;
	DMCoordinates _coordinates;
	SettingsIcon* _settingsIcon = nullptr;
	ProfileIcon* _profileIcon = nullptr;
	ManualHeatIcon* _manualHeatIcon = nullptr;
	ReflowIcon* _reflowIcon = nullptr;

	PopUpMenu _popUpMenu;
	ICONID _activeIcon = NO_ICONID;
public:
	SideBar();
	SideBar(TFT_eSPI* tft, DMCoordinates coordinates);
	~SideBar();

	void Draw();
	SB_TOUCHED_RETURN Touched(int x, int y);
	bool IsPopUpOpen() { return _popUpMenu.isOpen(); }
	void ReDrawPopUP() { _popUpMenu.Redraw(); }
	void ManualHeatIconEnabled(bool enabled);
	void ReflowIconEnabled(bool enabled);
};

