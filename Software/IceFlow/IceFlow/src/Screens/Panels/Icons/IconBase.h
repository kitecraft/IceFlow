#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../Utilities/Screens_Config.h"
#include "../../Utilities/DMCoordinates.h"
#include "../../Utilities/DMTheme.h"
#include "IconBaseDto.h"

class IconBase
{
protected:
	TFT_eSPI* _tft;
    IconBaseDto _config;
    uint16_t _bgColor = TFT_BLACK;

public:
    IconBase();
    IconBase(IconBaseDto config, TFT_eSPI* tft);
    bool Touched(int x, int y);
    void Draw(TFT_eSprite* sprite);
    int GetPX() { return _config.coordinates.p_x; }
    int GetPY() { return _config.coordinates.p_y; }
};

