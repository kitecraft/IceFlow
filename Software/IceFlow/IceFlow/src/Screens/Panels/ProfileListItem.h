#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "ProfileListItemDto.h"
#include "../Utilities/Screens_Config.h"
class ProfileListItem
{
private:
	ProfileListItemDto _config;

public:
	ProfileListItem();
	ProfileListItem(ProfileListItemDto config);
	void Draw(TFT_eSprite* sprite, bool selected);
	String GetFileName() { return _config.fileName; }
	bool Touched(int x, int y);
};

