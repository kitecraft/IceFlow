#include "ProfileListItem.h"

ProfileListItem::ProfileListItem()
{

}

ProfileListItem::ProfileListItem(ProfileListItemDto config)
{
	_config = config;
}

void ProfileListItem::Draw(TFT_eSprite* sprite, bool selected)
{
	Serial.print("ProfileListItem draw coordinates: ");
	_config.coordinates.Print();
	uint16_t bgColor = _config.theme.panelLightColor;
	if (selected) {
		bgColor = _config.theme.panelDarkColor;
	}

	sprite->setFreeFont(SMALL_FONT);
	sprite->setTextColor(_config.theme.textColor, bgColor);
	sprite->setTextDatum(TL_DATUM);

	sprite->fillRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w, _config.coordinates.h, bgColor);

	sprite->drawString(_config.displayName, _config.coordinates.x + 1, _config.coordinates.y + 10);
	sprite->drawString(_config.fileName, _config.coordinates.x + 1, _config.coordinates.y + 30);
	sprite->drawFastHLine(_config.coordinates.x, _config.coordinates.y + _config.coordinates.h - 1, _config.coordinates.w, GlobalTheme.panelBorderColor);

	sprite->setTextColor(TFT_GREEN, bgColor);
	sprite->drawString("Name: ", _config.coordinates.x + 1, _config.coordinates.y + 1);
	sprite->drawString("File: ", _config.coordinates.x + 1, _config.coordinates.y + 20);
}

bool ProfileListItem::Touched(int x, int y)
{
	if (x >= _config.coordinates.p_x &&
		x <= _config.coordinates.p_x + _config.coordinates.w &&
		y >= _config.coordinates.p_y &&
		y <= _config.coordinates.p_y + _config.coordinates.h)
	{
		return true;
	}
	return false;
}