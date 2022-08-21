#pragma once
#include "IconBase.h"

class StopIcon :
    public IconBase
{
private:

public:
    StopIcon();
    StopIcon(IconBaseDto config, TFT_eSPI* tft);
    void Draw(TFT_eSprite* sprite);
};

