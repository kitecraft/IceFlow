#include "MainScreenSideBar.h"

void MainScreenSideBar::Init(TFT_eSPI* newTFT)
{
	TFT = newTFT;
	_startIcon.Init(TFT, StarsideCoordinates(ICON_X, START_ICON_Y, ICON_WIDTH, ICON_HEIGHT));
	_preHeatIcon.Init(TFT, StarsideCoordinates(ICON_X, PRE_HEAT_ICON_Y, ICON_WIDTH, ICON_HEIGHT));
	_profilesIcon.Init(TFT, StarsideCoordinates(ICON_X, PROFILE_SELECTION_ICON_Y, ICON_WIDTH, ICON_HEIGHT));
	_openDoorIcon.Init(TFT, StarsideCoordinates(ICON_X, OPEN_DOOR_VIEW_ICON_Y, ICON_WIDTH, ICON_HEIGHT));
	_settingsIcon.Init(TFT, StarsideCoordinates(ICON_X, SETTINGS_ICON_Y, ICON_WIDTH, ICON_HEIGHT));
	_popUpMenu.Init(TFT);
}

void MainScreenSideBar::DrawPanel()
{
	TFT_eSprite sprite = TFT_eSprite(TFT);
	sprite.setColorDepth(16);
	sprite.createSprite(SIDEBAR_WIDTH, SIDEBAR_HEIGHT);
	sprite.fillSprite(TFT_TRANSPARENT);

	DrawSidebarBox(&sprite);

	sprite.pushSprite(SIDEBAR_X, SIDEBAR_Y, TFT_TRANSPARENT);
	sprite.deleteSprite();
}

void MainScreenSideBar::DrawSidebarBox(TFT_eSprite* sprite)
{
	sprite->fillSmoothRoundRect(0, 0, SIDEBAR_WIDTH, SIDEBAR_HEIGHT, 8, g_GlobalTheme.panelHeaderColor);

	sprite->fillSmoothRoundRect(0, 0, SIDEBAR_WIDTH, SIDEBAR_HEIGHT, 8, g_GlobalTheme.panelDropShadowColor);
	sprite->fillSmoothRoundRect(2, 2, SIDEBAR_WIDTH - 3, SIDEBAR_HEIGHT - 4, 8, g_GlobalTheme.panelDropShadowColor);

	sprite->fillSmoothRoundRect(0, 0, SIDEBAR_WIDTH - 1, SIDEBAR_HEIGHT - 1, 8, g_GlobalTheme.panelBorderColor);
	sprite->fillSmoothRoundRect(1, 1, SIDEBAR_WIDTH - 3, SIDEBAR_HEIGHT - 3, 8, g_GlobalTheme.panelBorderColor);

	_startIcon.Draw(sprite, _startIconStatus);
	_preHeatIcon.Draw(sprite);
	_profilesIcon.Draw(sprite);
	_openDoorIcon.Draw(sprite);
	_settingsIcon.Draw(sprite);
}

//Returns true if touched.
//Even if the touch was handled internally in some way
MSSB_RETURN_VALUES MainScreenSideBar::Touched(int x, int y)
{
	//if touch was on sidebar, but popUp is open
	// then close the popup and return NeedsRefresh
	if (x > (SIDEBAR_X + LEFT_MARGIN) && _popUpMenu.IsMenuOpen())
	{
		_popUpMenu.Close();
		return MSSB_TOUCHED_AND_REFRESH_REQUIRED;
	}

	//If touch was not on sidebar and popUp is not open
	//return NotTouche
	if (x < (SIDEBAR_X + LEFT_MARGIN) && !_popUpMenu.IsMenuOpen())
	{
		return MSSB_NOT_TOUCHED;
	}

	//If touch was not on sidebar and popUp is open
	//Check if popUp needs to handle the touch and 
	//If it did, return touched
	if (x < (SIDEBAR_X + LEFT_MARGIN) && _popUpMenu.IsMenuOpen())
	{
		//handle touching the open menu here
		String touchedOption = _popUpMenu.Touched(x, y);
		if (!touchedOption.isEmpty())
		{
			return MSSB_TOUCHED;
		}
		//Settings Icon
		/*
		if (touchedOption == "OTA")
		{
			//allQueues.AddItemToCommandQueue(STARSIDE_CMD_BEGIN_OTA, "");
			popUpMenu.Close();
			return true;
		}
		else if (touchedOption == "Configure")
		{
			allQueues.AddItemToCommandQueue(STARSIDE_CMD_BEGIN_WIFI_WEBSERVER, "");
			popUpMenu.Close();
			return true;
		}
		*/

		return MSSB_TOUCHED;
	}

	//Iterate each icon in sidebar to see if it was touched
	if (_startIcon.Touched(x, y)) {
		return MSSB_START_REFLOW;
	}

	if (_preHeatIcon.Touched(x, y)) {
		return MSSB_START_MANUAL_PREHEAT;
	}

	if(_profilesIcon.Touched(x, y)){
		vector<String> filelist = g_profileManager.GetListOfProfileFileNames();
		for (int i = 0; i < filelist.size(); i++) {
			Serial.println(filelist.at(i));
		}
		return MSSB_TOUCHED;
	}

	if (_settingsIcon.Touched(x, y))
	{
		String options[2] = { "Settings","Info" };
		_popUpMenu.Create(StarsideCoordinates(ICON_X - 2, _settingsIcon.coordinates.y, 0, 0), options, 2);
		_popUpMenu.Open();
		return MSSB_TOUCHED;
	}

	return MSSB_NOT_TOUCHED;
}