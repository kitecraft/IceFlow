#include "ScrollButtons.h"

ScrollButtons::ScrollButtons()
{

}

ScrollButtons::ScrollButtons(DMCoordinates coordinates)
{
	_downButtonCoordinates.x = coordinates.x;
	_downButtonCoordinates.y = coordinates.y;
	_downButtonCoordinates.w = coordinates.w / 2;
	_downButtonCoordinates.h = coordinates.h;
	_downButtonCoordinates.p_x = coordinates.p_x;
	_downButtonCoordinates.p_y = coordinates.p_y;

	_upButtonCoordinates.w = coordinates.w / 2;
	_upButtonCoordinates.x = coordinates.x + _downButtonCoordinates.w;
	_upButtonCoordinates.y = coordinates.y;
	_upButtonCoordinates.h = coordinates.h;
	_upButtonCoordinates.p_x = coordinates.p_x + _downButtonCoordinates.w;
	_upButtonCoordinates.p_y = coordinates.p_y;
}
ScrollButtons::~ScrollButtons()
{
}


void ScrollButtons::Draw(TFT_eSprite *sprite, bool downEnabled, bool upEnabled)
{
	sprite->drawRect(_downButtonCoordinates.x, _downButtonCoordinates.y, _downButtonCoordinates.w, _downButtonCoordinates.h, GlobalTheme.panelBorderColor);
	sprite->drawRect(_upButtonCoordinates.x, _upButtonCoordinates.y, _upButtonCoordinates.w, _upButtonCoordinates.h, GlobalTheme.panelBorderColor);

	sprite->drawRect(_downButtonCoordinates.x + 1, _downButtonCoordinates.y + 1, _downButtonCoordinates.w - 2, _downButtonCoordinates.h - 2, TFT_LIGHTGREY);

	sprite->drawRect(_upButtonCoordinates.x + 1, _upButtonCoordinates.y + 1, _upButtonCoordinates.w - 2, _upButtonCoordinates.h - 2, TFT_LIGHTGREY);


	uint16_t color = GlobalTheme.panelBorderColor;
	if (downEnabled) {
		color = GlobalTheme.panelDarkColor;
	}
	sprite->fillTriangle(
		_downButtonCoordinates.x + 3, _downButtonCoordinates.y + 3,
		_downButtonCoordinates.x + (_downButtonCoordinates.w - 6), _downButtonCoordinates.y + 3,
		_downButtonCoordinates.x + (_downButtonCoordinates.w / 2), _downButtonCoordinates.y + (_downButtonCoordinates.h - 3),
		color
	);

	color = GlobalTheme.panelBorderColor;
	if (upEnabled) {
		color = GlobalTheme.panelDarkColor;
	}
	sprite->fillTriangle(
		_upButtonCoordinates.x + (_downButtonCoordinates.w / 2), _upButtonCoordinates.y + 3,
		_upButtonCoordinates.x + 3, _downButtonCoordinates.y + (_upButtonCoordinates.h - 3),
		_upButtonCoordinates.x + (_downButtonCoordinates.w - 6), _upButtonCoordinates.y + (_upButtonCoordinates.h - 3),
		color
	);
}

Scrolldirection ScrollButtons::Touched(int x, int y)
{

	return SCROLL_NONE;
}
