#include "DialogBase.h"
#include "../Widgets/Box.h"

DialogBase::DialogBase(TFT_eSPI* tft, DMCoordinates coordinates, DMTheme theme, const GFXfont* font)
{
	_tft = tft;
	_coordinates = coordinates;
	_theme = theme;
	_font = font;

	_sprite = new TFT_eSprite(_tft);
	_sprPtr = (uint16_t*)_sprite->createSprite(_coordinates.w, _coordinates.h);


	//_sprite->fillSprite(_theme.panelDarkColor);
	_sprite->fillSprite(_theme.panelLightColor);

	DrawSquaredBox(_sprite, coordinates, theme, true);
}

DialogBase::~DialogBase()
{
	if (_sprite != nullptr) {
		_sprite->deleteSprite();
		delete _sprite;
		_sprite = nullptr;
	}
	
	ClearBuffer();
}

void DialogBase::ClearBuffer()
{
	if (_screenReadBuffer != nullptr) {
		free(_screenReadBuffer);
		_screenReadBuffer = nullptr;
	}
}

void DialogBase::Show()
{
	ClearBuffer();
	_screenReadBuffer = (uint16_t*)ps_calloc(_coordinates.w * _coordinates.h, sizeof(uint16_t));
	_tft->readRect(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);

	_tft->startWrite();
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _sprPtr);
	_visible = true;
	_tft->dmaWait();
	_tft->endWrite();
}

void DialogBase::Hide()
{
	_tft->startWrite();
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);
	_visible = false;
	_tft->dmaWait();
	_tft->endWrite();
	ClearBuffer();
}