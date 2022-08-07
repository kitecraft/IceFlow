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

	if (_config.useRounded) {
		_updateX = _config.coordinates.p_x + _config.coordinates.h/2;
		_updateY = _config.coordinates.p_y + 3;
		_updateW = _config.coordinates.w - _config.coordinates.h;
		_updateH = _config.coordinates.h - 6;
	}
	else {
		_updateX = _config.coordinates.p_x + 3;
		_updateY = _config.coordinates.p_y + 3;
		_updateW = _config.coordinates.w - 6;
		_updateH = _config.coordinates.h - 6;
	}


	switch (_config.textAlignment) {
	case ML_DATUM:
		if (_config.useRounded) {
			_textX = _config.coordinates.x + _config.coordinates.h/2;
			_updateTextX = 0;
		}
		else {
			_textX = _config.coordinates.x + 5;
			_updateTextX = 2;
		}
		_textY = round((double)_config.coordinates.y + (double)_config.coordinates.h / 2.0 - (double)TEXT_Y_OFFSET);
		_updateTextY = round((double)_updateH / 2.0 - (double)TEXT_Y_OFFSET);
		break;
	case MR_DATUM:
		if (_config.useRounded) {
			_textX = _config.coordinates.x + _config.coordinates.w - (_config.coordinates.h/2);
			_updateTextX = _updateW;
		}
		else {
			_textX = _config.coordinates.x + _config.coordinates.w - 5;
			_updateTextX = _updateW - 2;
		}
		_textY = round((double)_config.coordinates.y + (double)_config.coordinates.h / 2.0 - (double)TEXT_Y_OFFSET);
		_updateTextY = round((double)_updateH / 2.0 - (double)TEXT_Y_OFFSET);
		break;
	case MC_DATUM:  //Default sets to MC_DATUM anyways
	default:
		_config.textAlignment = MC_DATUM;
		_textX = _config.coordinates.x + _config.coordinates.w / 2;
		_textY = round((double)_config.coordinates.y + (double)_config.coordinates.h / 2.0 - (double)TEXT_Y_OFFSET);
		_updateTextX = _updateW / 2;
		_updateTextY = round((double)_updateH / 2.0 - (double)TEXT_Y_OFFSET);
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

bool TextBox::Touched(int x, int y)
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

void TextBox::Draw(TFT_eSprite* sprite)
{
	if (_config.useRounded) {
		DrawRoundedBox(sprite, _config.coordinates, _config.coordinates.h/2, _config.theme, _config.roundedBlendColor, _config.useDark);
	}
	else {
		DrawSquaredBox(sprite, _config.coordinates, _config.theme, _config.useDark);
	}
	
	sprite->setFreeFont(_config.font);
	sprite->setTextColor(_config.theme.textColor, _textBG);
	sprite->setTextDatum(_config.textAlignment);
}

void TextBox::Draw(TFT_eSprite* sprite, const char* text)
{
	Draw(sprite);
	sprite->drawString(text, _textX, _textY);
}

void TextBox::Draw(TFT_eSprite* sprite, const int number)
{
	Draw(sprite);
	sprite->drawNumber(number, _textX, _textY);
}

void TextBox::Update(const char* text)
{
	if (_tft == nullptr) {
		return;
	}
	_sprite->fillSprite(_textBG);
	_sprite->drawString(text, _updateTextX, _updateTextY);
	_tft->pushImageDMA(_updateX, _updateY, _updateW, _updateH, _sprPtr);
}

void TextBox::Update(const int number)
{
	if (_tft == nullptr) {
		return;
	}
	_sprite->fillSprite(_textBG);
	_sprite->drawNumber(number, _updateTextX, _updateTextY);
	_tft->pushImageDMA(_updateX, _updateY, _updateW, _updateH, _sprPtr);
}

int TextBox::DrawTextBox(TFT_eSprite* sprite, TextBoxDto configDto, const char* text)
{
	sprite->setFreeFont(configDto.font);
	sprite->setTextDatum(configDto.textAlignment);
	
	if (configDto.coordinates.w == -1) {
		if (configDto.useRounded) {
			configDto.coordinates.w = sprite->textWidth(text) + configDto.coordinates.h + 2;
		}
		else {
			configDto.coordinates.w = sprite->textWidth(text) + 10;
		}
	}

	int textX = 5;
	if (configDto.useRounded) {
		textX = configDto.coordinates.h / 2;
		DrawRoundedBox(sprite, configDto.coordinates, configDto.coordinates.h / 2, configDto.theme, configDto.roundedBlendColor, configDto.useDark);
	}
	else {
		DrawSquaredBox(sprite, configDto.coordinates, configDto.theme, configDto.useDark);
	}


	if (configDto.useDark) {
		sprite->setTextColor(configDto.theme.textColor, configDto.theme.panelDarkColor);
	}
	else {
		sprite->setTextColor(configDto.theme.textColor, configDto.theme.panelLightColor);
	}


	switch (configDto.textAlignment) {
	case ML_DATUM:
		sprite->drawString(text, configDto.coordinates.x + textX, configDto.coordinates.y + round(configDto.coordinates.h/2));
		break;
	case MR_DATUM:
		sprite->drawString(text, configDto.coordinates.w - textX, configDto.coordinates.y + round(configDto.coordinates.h / 2));
		break;
	case MC_DATUM:
		sprite->drawString(text, configDto.coordinates.x + round(configDto.coordinates.w / 2), configDto.coordinates.y + round(configDto.coordinates.h / 2));
		break;
	default:
		sprite->drawString(text, configDto.coordinates.x + textX, configDto.coordinates.y + 5);
		break;
	}
	return configDto.coordinates.w;
}