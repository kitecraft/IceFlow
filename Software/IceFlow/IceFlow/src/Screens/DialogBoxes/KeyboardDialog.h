#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"
#include "DialogButton.h"

class KeyboardDialog
{
private:
    TFT_eSPI* _tft;
    DMCoordinates _coordinates;
    DMTheme _theme;

    bool _visible = false;

    TFT_eSprite* _sprite = nullptr;
    uint16_t* _sprPtr = nullptr;

    uint16_t* _screenReadBuffer = nullptr;
    void ClearBuffer();

public:
    KeyboardDialog() {}
    ~KeyboardDialog();
    KeyboardDialog(TFT_eSPI* tft, DMTheme theme, String title);

    void Show();
    void Hide();
    bool IsVisible() { return _visible; }
    DialogButtonType Touched(int x, int y);
};

