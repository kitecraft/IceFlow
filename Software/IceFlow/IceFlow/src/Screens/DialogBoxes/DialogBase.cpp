#include "DialogBase.h"
#include "../Widgets/Box.h"
#include "../Widgets/TextBox.h"

DialogBase::DialogBase(TFT_eSPI* tft, DMCoordinates coordinates, DMTheme theme, String title)
{
	_tft = tft;
	_coordinates = coordinates;
	_theme = theme;
	_screenReadBuffer = nullptr;

	_sprite = new TFT_eSprite(_tft);
	_sprPtr = (uint16_t*)_sprite->createSprite(_coordinates.w, _coordinates.h);


	//_sprite->fillSprite(_theme.panelDarkColor);
	_sprite->fillSprite(_theme.panelLightColor);

	DrawSquaredBox(_sprite, coordinates, theme, true);
	TextBox::DrawTextBox(_sprite,
		TextBoxDto(
			DMCoordinates(0, 0, _coordinates.w, LARGE_FONT_TEXT_BOX_H, 0, 0),
			GlobalTheme,
			LARGE_FONT,
			ML_DATUM,
			true),
		title.c_str());
}

DialogBase::~DialogBase()
{
	_sprite->deleteSprite();
	delete _sprite;
	
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

	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _sprPtr);
	_visible = true;
}

void DialogBase::Hide()
{
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);
	_tft->dmaWait();
	_visible = false;
	ClearBuffer();
}