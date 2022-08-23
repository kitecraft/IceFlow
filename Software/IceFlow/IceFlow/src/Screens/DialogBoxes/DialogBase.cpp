#include "DialogBase.h"
#include "../Widgets/Box.h"
#include "../Widgets/TextBox.h"

DialogBase::DialogBase(TFT_eSPI* tft, DMCoordinates coordinates, DMTheme theme, String title)
{
	_tft = tft;
	_coordinates = coordinates;
	_theme = theme;
	_screenReadBuffer = nullptr;
	_visible = false;
	_title = title;
	_sprite = new TFT_eSprite(_tft);
	_sprPtr = (uint16_t*)_sprite->createSprite(_coordinates.w, _coordinates.h);
	Draw();
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
	if (_visible) {
		return;
	}
	ClearBuffer();
	_screenReadBuffer = (uint16_t*)ps_calloc(_coordinates.w * _coordinates.h, sizeof(uint16_t));
	_tft->readRect(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);

	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _sprPtr);
	_visible = true;
}

void DialogBase::Hide()
{
	if (!_visible) {
		return;
	}
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);
	_tft->dmaWait();
	_visible = false;
	ClearBuffer();
}

void DialogBase::Draw()
{
	_sprite->fillSprite(_theme.panelLightColor);

	DrawSquaredBox(_sprite, _coordinates, _theme, true);
	TextBox::DrawTextBox(_sprite,
		TextBoxDto(
			DMCoordinates(0, 0, _coordinates.w, LARGE_FONT_TEXT_BOX_H, 0, 0),
			GlobalTheme,
			LARGE_FONT,
			ML_DATUM,
			true),
		_title.c_str());
}

void DialogBase::ReShow()
{
	if (!_visible) {
		return;
	}
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _sprPtr);
}