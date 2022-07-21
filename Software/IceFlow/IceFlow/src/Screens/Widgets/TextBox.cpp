#include "TextBox.h"
#include "Box.h"

TextBox::TextBox(TextBoxDto configDto, TFT_eSPI* tft)
{
	Configure(configDto, tft);
}

TextBox::~TextBox()
{
	DeleteSprite();
}

void TextBox::DeleteSprite()
{
	if (_sprite != nullptr)
	{
		_sprite->deleteSprite();
		delete(_sprite);
		_sprite = nullptr;
	}
}

void TextBox::Configure(TextBoxDto configDto, TFT_eSPI* tft)
{
	_tft = tft;
	_config = configDto;
	DeleteSprite();

	_updateX = _config.coordinates.p_x + 3;
	_updateY = _config.coordinates.p_y + 3;
	_updateW = _config.coordinates.w - 6;
	_updateH = _config.coordinates.h - 6;

	switch (_config.textAlignment) {
	case ML_DATUM:
		_textX = _config.coordinates.x + 5;
		_textY = _config.coordinates.y + _config.coordinates.h / 2 - TEXT_Y_OFFSET;
		_updateTextX = 2;
		_updateTextY = _updateH / 2 - TEXT_Y_OFFSET;
		break;
	case MR_DATUM:
		_textX = _config.coordinates.x + _config.coordinates.w - 5;
		_textY = _config.coordinates.y + _config.coordinates.h / 2 - TEXT_Y_OFFSET;
		_updateTextX = _updateW - 2;
		_updateTextY = _updateH / 2 - TEXT_Y_OFFSET;
		break;
	case MC_DATUM:  //Default sets to MC_DATUM anyways
	default:
		_config.textAlignment = MC_DATUM;
		_textX = _config.coordinates.x + _config.coordinates.w / 2;
		_textY = _config.coordinates.y + _config.coordinates.h / 2 - TEXT_Y_OFFSET;
		_updateTextX = _updateW / 2;
		_updateTextY = _updateH / 2 - TEXT_Y_OFFSET;
		break;
	}

	if (_config.useDark) {
		_textBG = _config.theme.panelDarkColor;
	}
	else {
		_textBG = _config.theme.panelLightColor;
	}

	if (_tft != nullptr) {
		_sprite = new TFT_eSprite(_tft);
		_sprPtr = (uint16_t*)_sprite->createSprite(_updateW, _updateH);
		_sprite->setFreeFont(_config.font);
		_sprite->setTextColor(_config.theme.textColor, _textBG);
		_sprite->setTextDatum(_config.textAlignment);
	}
}

void TextBox::Draw(TFT_eSprite* sprite, const char* text)
{
	DrawSquaredBox(sprite, _config.coordinates, _config.theme, _config.useDark);
	sprite->setFreeFont(_config.font);
	sprite->setTextColor(_config.theme.textColor, _textBG);
	sprite->setTextDatum(_config.textAlignment);
	sprite->drawString(text, _textX, _textY);
}

void TextBox::Update(const char* text)
{
	if (_tft == nullptr) {
		return;
	}
	_sprite->fillSprite(_textBG);
	_sprite->drawString(text, _updateTextX, _updateTextY);
	_tft->dmaWait();
	_tft->pushImageDMA(_updateX, _updateY, _updateW, _updateH, _sprPtr);
}

void TextBox::DrawTextBox(TFT_eSprite* sprite, TextBoxDto configDto, const char* text)
{
	DrawSquaredBox(sprite, configDto.coordinates, configDto.theme, configDto.useDark);

	sprite->setFreeFont(configDto.font);
	sprite->setTextDatum(configDto.textAlignment);

	if (configDto.useDark) {
		sprite->setTextColor(configDto.theme.textColor, configDto.theme.panelDarkColor);
	}
	else {
		sprite->setTextColor(configDto.theme.textColor, configDto.theme.panelLightColor);
	}

	switch (configDto.textAlignment) {
	case ML_DATUM:
		sprite->drawString(text, 5, round(configDto.coordinates.h/2));
		break;
	case MR_DATUM:
		sprite->drawString(text, configDto.coordinates.w - 5, round(configDto.coordinates.h / 2));
		break;
	case MC_DATUM:
		sprite->drawString(text, round(configDto.coordinates.w / 2), round(configDto.coordinates.h / 2));
		break;
	default:
		sprite->drawString(text, 5, 5);
		break;
	}
}