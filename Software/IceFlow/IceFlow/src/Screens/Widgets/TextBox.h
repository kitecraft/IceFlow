#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "TextBoxDto.h"


class TextBox
{
private:
    TextBoxDto _config;
    int _textX = 0;
    int _textY = 0;
    uint16_t _textBG = 0;
    String _text = "";

    int _updateX = 0;
    int _updateY = 0;
    int _updateW = 0;
    int _updateH = 0;
    int _updateTextX = 0;
    int _updateTextY = 0;

    TFT_eSPI* _tft = nullptr;
    TFT_eSprite* _sprite = nullptr;
    uint16_t* _sprPtr = nullptr;
    void Draw(TFT_eSprite* sprite, bool useDark);
public:
    TextBox(TextBoxDto configDto, TFT_eSPI* tft = nullptr);
    ~TextBox();

    bool Touched(int x, int y);
    void Draw(TFT_eSprite* sprite, String text, bool useDark = false);
    void Draw(TFT_eSprite* sprite, const char* text, bool useDark = false);
    void Draw(TFT_eSprite* sprite, const int number, bool useDark = false);
    void Update(const char* text, bool useDark = false);
    void Update(const int number, bool useDark = false);
    void Update(String text, bool useDark = false);
    static int DrawTextBox(TFT_eSprite* sprite, TextBoxDto configDto, const char* text);
    String GetText() { return _text; }
};