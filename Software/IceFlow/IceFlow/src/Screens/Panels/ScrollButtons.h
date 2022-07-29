#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

enum Scrolldirection {
	SCROLL_NONE = -1,
	SCROLL_UP,
	SCROLL_DOWN
};

class ScrollButtons
{
private:
	bool _scrollUpEnabled = false;
	bool _scrollDownEnabled = false;
	DMCoordinates _downButtonCoordinates;
	DMCoordinates _upButtonCoordinates;

public:
	ScrollButtons();
	ScrollButtons(DMCoordinates coordinates);
	~ScrollButtons();
	void Draw(TFT_eSprite* sprite, bool downEnabled, bool upEnabled);
	Scrolldirection Touched(int x, int y);
};

