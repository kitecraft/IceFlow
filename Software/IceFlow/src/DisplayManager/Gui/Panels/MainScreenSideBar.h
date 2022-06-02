#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../../IceFlow_Config.h"
#include "../../../Utilities/PreferencesManager.h"
#include "../Icons/SettingsIcon.h"
#include "../Icons/StartIcon.h"
#include "../Icons/PreHeatIcon.h"
#include "../Icons/ProfilesIcon.h"
#include "../Icons/OpenDoorIcon.h"
#include "../Icons/IconPopUpMenu.h"
#include "../../Utilities/StarsideCoordinates.h"
#include "../../Utilities/StarsideTheme.h"
#include "../../../Utilities/CommandQueue.h"
#include "../../../ProfileManager/ProfileManager.h"

extern ProfileManager g_profileManager;

using namespace std;
#include <vector>

#define MAXIMUM_ICONS 5
//#define SIDEBAR_BACKGROUND_COLOR 0x9578

#define Y_OFFSET 1

#define SIDEBAR_WIDTH 69
#define SIDEBAR_X (TFT_DISPLAY_WIDTH - SIDEBAR_WIDTH)

#define SIDEBAR_Y 0
#define SIDEBAR_HEIGHT TFT_DISPLAY_HEIGHT

#define LEFT_MARGIN 2
#define RIGHT_MARGIN 3
#define VERTICAL_MARGIN 3

#define ICON_AREA_HEIGHT ((SIDEBAR_HEIGHT - (Y_OFFSET*2)) / 5)

#define ICON_WIDTH (SIDEBAR_WIDTH - RIGHT_MARGIN - LEFT_MARGIN)
#define ICON_HEIGHT (ICON_AREA_HEIGHT - (VERTICAL_MARGIN * 2))
#define ICON_RADIUS 8

#define ICON_X (SIDEBAR_X + LEFT_MARGIN)


#define START_ICON_Y				(VERTICAL_MARGIN + Y_OFFSET)
#define PRE_HEAT_ICON_Y				((ICON_AREA_HEIGHT * 1) + VERTICAL_MARGIN + Y_OFFSET)
#define PROFILE_SELECTION_ICON_Y	((ICON_AREA_HEIGHT * 2) + VERTICAL_MARGIN + Y_OFFSET)
#define OPEN_DOOR_VIEW_ICON_Y	((ICON_AREA_HEIGHT * 3) + VERTICAL_MARGIN + Y_OFFSET)
#define SETTINGS_ICON_Y				((ICON_AREA_HEIGHT * 4) + VERTICAL_MARGIN + Y_OFFSET)

extern CommandQueue g_commandQueue;

enum MSSB_RETURN_VALUES {
	MSSB_GENERIC_BADNESS = -1,
	MSSB_OK,
	MSSB_TOUCHED,
	MSSB_TOUCHED_AND_REFRESH_REQUIRED,
	MSSB_NOT_TOUCHED
};

class MainScreenSideBar
{
private:
	TFT_eSPI* TFT;
	StartIcon _startIcon;
	PreHeatIcon _preHeatIcon;
	SettingsIcon _settingsIcon;
	ProfilesIcon _profilesIcon;
	OpenDoorIcon _openDoorIcon;

	IconPopUpMenu _popUpMenu;

	void DrawSidebarBox(TFT_eSprite* sprite);
public:
	~MainScreenSideBar();
	void Init(TFT_eSPI* newTFT);
	void Draw();
	bool IsMenuOpen() { return _popUpMenu.IsMenuOpen(); }


	MSSB_RETURN_VALUES Touched(int x, int y);
};
