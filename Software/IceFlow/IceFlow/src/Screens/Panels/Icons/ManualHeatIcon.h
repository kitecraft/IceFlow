#pragma once
#include "IconBase.h"
class ManualHeatIcon :
    public IconBase
{
private:

public:
    ManualHeatIcon();
    ManualHeatIcon(IconBaseDto config, TFT_eSPI* tft);
    void Draw(TFT_eSprite* sprite);
};

