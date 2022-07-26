#pragma once
#include "TFT_eSPI.h"
#include "ButtonDto.h"


class Button
{
private:
	ButtonDto _config;
	String _label;
public:
	Button();
	~Button();
	Button(ButtonDto config, String label);
	void Draw(TFT_eSPI* tft);
	bool Touched(int x, int y);
};

