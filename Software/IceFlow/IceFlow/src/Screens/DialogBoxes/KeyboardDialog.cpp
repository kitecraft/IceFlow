#include "KeyboardDialog.h"

KeyboardDialog::KeyboardDialog(TFT_eSPI* tft, DMTheme theme, String title)
{
	_tft = tft;
	_coordinates = DMCoordinates(0, 0, 320, 100, 0, 0);
	_theme = theme;

	_sprite = new TFT_eSprite(_tft);
	_sprPtr = (uint16_t*)_sprite->createSprite(_coordinates.w, _coordinates.h);


	//_sprite->fillSprite(_theme.panelDarkColor);
	_sprite->fillSprite(TFT_GREEN);

	/*
	DrawSquaredBox(_sprite, coordinates, theme, true);
	TextBox::DrawTextBox(_sprite,
		TextBoxDto(
			DMCoordinates(0, 0, _coordinates.w, LARGE_FONT_TEXT_BOX_H, 0, 0),
			GlobalTheme,
			LARGE_FONT,
			ML_DATUM,
			true),
		title.c_str());
		*/
}

KeyboardDialog::~KeyboardDialog()
{
	if (_sprite != nullptr) {
		_sprite->deleteSprite();
		delete _sprite;
		_sprite = nullptr;
	}

	ClearBuffer();
}

void KeyboardDialog::ClearBuffer()
{
	if (_screenReadBuffer != nullptr) {
		free(_screenReadBuffer);
		_screenReadBuffer = nullptr;
	}
}

void KeyboardDialog::Show()
{
	ClearBuffer();
	_screenReadBuffer = (uint16_t*)ps_calloc(_coordinates.w * _coordinates.h, sizeof(uint16_t));
	_tft->readRect(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);

	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _sprPtr);
	_visible = true;
}

void KeyboardDialog::Hide()
{
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, _coordinates.w, _coordinates.h, _screenReadBuffer);
	_visible = false;
	ClearBuffer();
}

DialogButtonType KeyboardDialog::Touched(int x, int y)
{
	if (!_visible) {
		return DB_NONE;
	}

	return DB_NONE;
}