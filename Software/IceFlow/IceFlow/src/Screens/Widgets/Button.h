#pragma once
#include "TFT_eSPI.h"
#include "ButtonDto.h"

class Button
{
private:
	TFT_eSPI* _tft;
	ButtonDto _config;
	String _label;
	bool _visible = true;
	uint16_t _buttonColor = TFT_BLACK;
	TFT_eSprite* _sprite = nullptr;
	uint16_t* _sprPtr = nullptr;

public:
	Button();
	~Button();
	Button(ButtonDto config, String label, TFT_eSPI* tft, bool useSprite = true);
	void Draw();
	void Draw(TFT_eSprite* sprite);
	void Draw(TFT_eSprite* sprite, String label);
	bool Touched(int x, int y);
	void Visible(bool visible) { _visible = visible; }
	bool IsVisible() { return _visible; }
	void SetButtonColor(uint16_t buttonColor) { _buttonColor = buttonColor; }
};

