#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "ProfileListItemDto.h"

class ProfileListItem
{
private:
	ProfileListItemDto _config;

public:
	ProfileListItem();
	ProfileListItem(ProfileListItemDto config);
	void Draw(TFT_eSprite* sprite, bool selected);
	String GetFileName() { return _config.fileName; }
};

