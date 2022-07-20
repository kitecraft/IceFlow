#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "ProgressBarDto.h"

class ProgressBar
{
private:
    ProgressBarDto _config;
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
    ProgressBar();
    ~ProgressBar();
    ProgressBar(ProgressBarDto configDto, TFT_eSPI* tft);

    void Configure(ProgressBarDto configDto, TFT_eSPI* tft);
    void Draw(TFT_eSprite* sprite);
    void Update(int progress);
};

