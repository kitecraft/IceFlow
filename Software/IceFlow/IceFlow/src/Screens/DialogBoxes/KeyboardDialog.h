#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"
#include "../Widgets/TextBox.h"
#include "KeyboardButton.h"

#define KBD_WIDTH 320
#define KBD_HEADER_H 55

#define KBD_VALUE_TB_X 0
#define KBD_VALUE_TB_Y 26
#define KBD_VALUE_TB_W 320
#define KBD_VALUE_TB_H LARGE_FONT_TEXT_BOX_H

#define KBD_BUTTON_ROW_START_Y 55
#define KBD_BUTTON_HEIGHT 45
#define KBD_BUTTON_WIDTH 29
#define KBD_NUM_ROWS 4
#define KDB_NUM_COLUMS 11

#define KBG_MAX_LENGTH_BG_COLOR 0x935b

class KeyboardDialog
{
private:
    TFT_eSPI* _tft;
    DMCoordinates _coordinates;
    DMTheme _theme;
    String _title;
    bool _visible = false;

    String _value = "";
    TextBox* _valueTB = nullptr;
    bool _selected = false;
    int _maxLength = 16;

    bool _shiftSelected = false;

    KeyboardButton* _kbButtons[NUMBER_OF_KEYBOARD_BUTTONS] = { nullptr };
    void CreateButtons();

    void DrawHeader();
    void DrawButtons();

    void UpdateValueTextbox(String newChar);
    void BackSpaceValue();
public:
    KeyboardDialog() {}
    ~KeyboardDialog();
    KeyboardDialog(TFT_eSPI* tft, String title);

    DialogButtonType Touched(int x, int y);

    void Show();
    void Hide();
    bool IsVisible() { return _visible; }

    void SetValue(String newValue);
    String GetValue() { return _value; }
    void SetMaxLength(int newMaxLength) { _maxLength = newMaxLength; }
    int GetMaxLength() { return _maxLength; }
};

