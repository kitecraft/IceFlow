#pragma once
#include "DialogButtonDto.h"
#include "../Widgets/Button.h"

#define DIALOG_BUTTON_WIDTH 65
#define DIALOG_BUTTON_HEIGHT MEDIUM_FONT_TEXT_BOX_H

class DialogButton
{
private:
	DialogButtonDto _config;
	Button* _button = nullptr;

public:
	DialogButton() {}
	~DialogButton();
	DialogButton(DialogButtonDto config, TFT_eSPI* tft);
	void Draw(TFT_eSprite* sprite);
	bool Touched(int x, int y);
};

