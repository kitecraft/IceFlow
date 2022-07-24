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

    int _updateX = 0;
    int _updateY = 0;
    int _updateW = 0;
    int _updateH = 0;
    int _updateTextX = 0;
    int _updateTextY = 0;

    TFT_eSPI* _tft = nullptr;
    TFT_eSprite* _sprite = nullptr;
    uint16_t* _sprPtr = nullptr;
    void DeleteSprite();
public:
    TextBox(TextBoxDto configDto, TFT_eSPI* tft = nullptr);
    ~TextBox();

    void Configure(TextBoxDto configDto, TFT_eSPI* tft = nullptr);
    void Draw(TFT_eSprite* sprite, const char* text);
    void Update(const char* text);

    static int DrawTextBox(TFT_eSprite* sprite, TextBoxDto configDto, const char* text);
};