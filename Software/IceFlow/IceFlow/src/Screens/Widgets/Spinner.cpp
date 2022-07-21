#include "Spinner.h"

Spinner::Spinner(TFT_eSPI* tft, int x, int y)
{
	_tft = tft;
	_x = x;
	_y = y;

	_col[0] = DFAULT_COLOR_1;
	_col[1] = DFAULT_COLOR_2;
	_col[2] = DFAULT_COLOR_3;
	_col[3] = DFAULT_COLOR_4;
	_col[4] = DFAULT_COLOR_5;
	_col[5] = DFAULT_COLOR_6;
	_col[6] = DFAULT_COLOR_7;
	_col[7] = DFAULT_COLOR_8;
}

void Spinner::SetColors(uint32_t colors[8])
{
	for (int i = 0; i < 8; i++) {
		_col[i] = colors[i];
	}
}

void Spinner::UpdateCircle()
{
	_tft->fillCircle(
		_x + 30 * (cos(-(_circleLoopCounter + _secondaryLoopCounter) * PI / 4)),
		_y + 30 * (sin(-(_circleLoopCounter + _secondaryLoopCounter) * PI / 4)),
		6,
		_col[_secondaryLoopCounter]);
	/*
	_sprite->fillCircle(6, 6, 6, _col[_secondaryLoopCounter]);

	_tft->pushImageDMA(
		_x + 30 * (cos(-(_circleLoopCounter + _secondaryLoopCounter) * PI / 4)),
		_y + 30 * (sin(-(_circleLoopCounter + _secondaryLoopCounter) * PI / 4)),
		12,
		12,
		_sprPtr);
	*/
	_secondaryLoopCounter++;
	if (_secondaryLoopCounter > 7)
	{
		_secondaryLoopCounter = 0;
		_circleLoopCounter--;
	}

	if (_circleLoopCounter <= 0)
	{
		_circleLoopCounter = 8;
	}

}

void Spinner::Draw()
{
	for (int i = 0; i < 8; i++) {
		UpdateCircle();
	}
}

void Spinner::Update()
{
	if (millis() < _nextUpdate)
	{
		return;
	}

	UpdateCircle();
	_nextUpdate = millis() + _updateScreenInterval;
}
