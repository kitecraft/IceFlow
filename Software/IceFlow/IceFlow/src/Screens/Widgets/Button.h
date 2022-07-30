#pragma once
#include "TFT_eSPI.h"
#include "ButtonDto.h"

class Button
{
private:
	TFT_eSPI* _tft;
	ButtonDto _config;
	String _label;
	TFT_eSprite* _sprite = nullptr;
	uint16_t* _sprPtr = nullptr;
	bool _visible = true;
public:
	Button();
	~Button();
	Button(ButtonDto config, String label, TFT_eSPI* tft);
	void Draw();
	bool Touched(int x, int y);
	void Visible(bool visible) { _visible = visible; }
};

