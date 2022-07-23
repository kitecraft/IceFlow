#include "SettingsIcon.h"

SettingsIcon::SettingsIcon()
	: IconBase()
{

}

SettingsIcon::SettingsIcon(IconBaseDto config, TFT_eSPI* tft)
	: IconBase(config, tft)
{

}

void SettingsIcon::Draw(TFT_eSprite* sprite)
{
	IconBase::Draw(sprite);
	int mid = _config.coordinates.y + (_config.coordinates.h / 2) - MID_Y_OFFSET;
	int offsetY = LINE_HEIGHT;
	int shortX = _config.coordinates.x + LINE_SHORT_X_OFFSET;
	int longX = _config.coordinates.x + LINE_LONG_X_OFFSET;
	sprite->fillSmoothRoundRect(shortX, mid - offsetY, LINE_SHORT, LINE_RADIUS, LINE_HEIGHT, _config.theme.panelDarkColor, _config.theme.panelLightColor);
	sprite->fillSmoothRoundRect(shortX, mid, LINE_SHORT, LINE_RADIUS, LINE_HEIGHT, _config.theme.panelDarkColor, _config.theme.panelLightColor);
	sprite->fillSmoothRoundRect(shortX, mid + offsetY, LINE_SHORT, LINE_RADIUS, LINE_HEIGHT, _config.theme.panelDarkColor, _config.theme.panelLightColor);

	sprite->fillSmoothRoundRect(longX, mid - offsetY, LINE_LONG, LINE_RADIUS, LINE_HEIGHT, _config.theme.panelDarkColor, _config.theme.panelLightColor);
	sprite->fillSmoothRoundRect(longX, mid, LINE_LONG, LINE_RADIUS, LINE_HEIGHT, _config.theme.panelDarkColor, _config.theme.panelLightColor);
	sprite->fillSmoothRoundRect(longX, mid + offsetY, LINE_LONG, LINE_RADIUS, LINE_HEIGHT, _config.theme.panelDarkColor, _config.theme.panelLightColor);
}