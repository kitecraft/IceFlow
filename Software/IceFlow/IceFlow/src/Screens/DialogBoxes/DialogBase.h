#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class DialogBase
{
protected:
    TFT_eSPI* _tft;
    DMCoordinates _coordinates;
    DMTheme _theme;

    bool _visible = false;

    TFT_eSprite* _sprite = nullptr;
    uint16_t* _sprPtr = nullptr;

    uint16_t* _screenReadBuffer = nullptr;
    void ClearBuffer();

public:
	DialogBase() {}
    ~DialogBase();
    DialogBase(TFT_eSPI* tft, DMCoordinates coordinates, DMTheme theme, String title);

    void Show();
    void Hide();
    bool Visible() { return _visible; }
};

