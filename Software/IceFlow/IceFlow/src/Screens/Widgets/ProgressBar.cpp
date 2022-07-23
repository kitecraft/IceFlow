#include "ProgressBar.h"
#include "Box.h"

ProgressBar::ProgressBar()
{

}

ProgressBar::~ProgressBar()
{
	DeleteSprite();
}

void ProgressBar::DeleteSprite()
{
	if (_sprite != nullptr)
	{
		_sprite->deleteSprite();
		delete(_sprite);
		_sprite = nullptr;
	}
}

ProgressBar::ProgressBar(ProgressBarDto configDto, TFT_eSPI* tft)
{
	Configure(configDto, tft);
}

void ProgressBar::Configure(ProgressBarDto configDto, TFT_eSPI* tft)
{
	_tft = tft;
	_config = configDto;
	DeleteSprite();

	_updateX = _config.coordinates.p_x + 3;
	_updateY = _config.coordinates.p_y + 3;
	_updateW = _config.coordinates.w - 6;
	_updateH = _config.coordinates.h - 6;


	if (_tft != nullptr) {
		_sprite = new TFT_eSprite(_tft);
		_sprPtr = (uint16_t*)_sprite->createSprite(_updateW, _updateH);
		_sprite->setFreeFont(_config.font);
		_sprite->setTextColor(_config.theme.textColor, TFT_TRANSPARENT);
		_sprite->setTextDatum(MC_DATUM);
	}
}

void ProgressBar::Draw(TFT_eSprite* sprite)
{
	DrawSquaredBox(sprite, _config.coordinates, _config.theme);
	sprite->fillRect(_config.coordinates.x + 3,
		_config.coordinates.y + 3,
		_config.coordinates.w - 6,
		_config.coordinates.h - 6,
		_config.barBackGroundColor);
}

void ProgressBar::Update(int progress)
{
	if (_tft == nullptr) {
		return;
	}
	_sprite->fillSprite(_config.barBackGroundColor);

	if (progress < 50) {
		_sprite->setTextColor(_config.theme.textColor, _config.barBackGroundColor);
	}
	else {
		_sprite->setTextColor(_config.theme.textColor, _config.barFillColor);
	}
	
	int w = round(((double)progress / 100.0) * (double)_updateW);
	_sprite->fillRect(0, 0, w, _updateH, _config.barFillColor);
	String msg = String(progress) + " %";
	_sprite->drawString(msg, _updateW / 2, _updateH / 2 - TEXT_Y_OFFSET);
	
	_tft->pushImageDMA(_updateX, _updateY, _updateW, _updateH, _sprPtr);
}