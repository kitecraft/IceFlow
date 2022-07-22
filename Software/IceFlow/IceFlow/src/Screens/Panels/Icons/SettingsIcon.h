#pragma once
#include "IconBase.h"

#define LINE_SHORT 10
#define LINE_SHORT_X_OFFSET 4
#define LINE_LONG 22
#define LINE_LONG_X_OFFSET (LINE_SHORT_X_OFFSET + LINE_SHORT + 4)
#define LINE_RADIUS 5
#define LINE_HEIGHT (LINE_RADIUS*2)
#define MID_Y_OFFSET 2

class SettingsIcon :
    public IconBase
{
private:

public:
    SettingsIcon();
    SettingsIcon(IconBaseDto config, TFT_eSPI* tft);
    void Draw(TFT_eSprite* sprite);
};

