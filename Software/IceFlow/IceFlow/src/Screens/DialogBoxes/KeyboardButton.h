#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"
#include "DialogButton.h"
#include "../Widgets/Button.h"
#include "KeyboardButtonType.h"


class KeyboardButton
{
private:
	TFT_eSPI* _tft;
	DMCoordinates _coordinates;
	DMTheme _theme;
	KeyboardButtonType  _type = KBT_NO_BUTTON;
	Button* _button = nullptr;
	bool _shifted = false;
	
	char _text[3];
	char _upperText[3];

	void SetKeyText();
	void CreateKey();
public:
	KeyboardButton();
	KeyboardButton(TFT_eSPI* tft, DMCoordinates coordinates, KeyboardButtonType  type);
	~KeyboardButton();

	KeyboardButtonType GetType() { return _type; }
	void Draw(TFT_eSprite* sprite, bool shift = false);
	KeyboardButtonType Touched(int x, int y);
	String GetDisplayedValue();
};

