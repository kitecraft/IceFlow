#include "PanelBase.h"

void PanelBase::Init(TFT_eSPI* newTFT)
{
	TFT = newTFT;
}

void PanelBase::DrawRoundedBox(TFT_eSprite* sprite, StarsideCoordinates coordinates, StarsideTheme theme, int radius, int border)
{
	sprite->fillSmoothRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, theme.panelBorderColor, TFT_TRANSPARENT);
	sprite->fillSmoothRoundRect(
		coordinates.x + border,
		coordinates.y + border,
		coordinates.w - (border * 2),
		coordinates.h - (border * 2),
		radius,
		theme.panelHeaderColor,
		TFT_TRANSPARENT);
}