#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Widgets/Button.h"
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

enum NUM_PAD_BUTTON {
	NPB_ZERO = 0,
	NPB_ONE,
	NPB_TWO,
	NPB_THREE,
	NPB_FOUR,
	NPB_FIVE,
	NPB_SIX,
	NPB_SEVEN,
	NPB_EIGHT,
	NPB_NINE,
	NPB_BSP,
	NPB_ENTER,
	NPB_DECIMAL,
	NPB_NO_BUTTON,
};

class NumPadButton
{
private:
	Button* _button = nullptr;
	NUM_PAD_BUTTON _type;
	String _label;

	void SetLabel(NUM_PAD_BUTTON type);

public:
	NumPadButton() {}
	NumPadButton(TFT_eSPI* tft, NUM_PAD_BUTTON type, DMCoordinates coordinates, DMTheme theme = GlobalTheme);
	~NumPadButton();
	
	void Draw(TFT_eSprite* sprite);
	NUM_PAD_BUTTON Touched(int x, int y);

	void Visible(bool visible) { _button->Visible(visible); }
};

