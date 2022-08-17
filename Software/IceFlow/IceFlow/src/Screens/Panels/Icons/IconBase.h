#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../Utilities/Screens_Config.h"
#include "../../Utilities/DMCoordinates.h"
#include "../../Utilities/DMTheme.h"
#include "IconBaseDto.h"

#define DISABLED_BLEND_RATIO 90
#define DISABLED_BLEND_COLOR TFT_DARKGREY
class IconBase
{
protected:
	TFT_eSPI* _tft;
    IconBaseDto _config;
    uint16_t _bgColor = TFT_BLACK;
    bool _enabled = true;

    void UpdateAsDisabled(TFT_eSprite *sprite);

public:
    IconBase();
    IconBase(IconBaseDto config, TFT_eSPI* tft);
    bool Touched(int x, int y);
    void Draw(TFT_eSprite* sprite);
    int GetPX() { return _config.coordinates.p_x; }
    int GetPY() { return _config.coordinates.p_y; }
    void SetEnabled(bool enabled) { _enabled = enabled; }
    bool IsEnabled() { return _enabled; }
};

