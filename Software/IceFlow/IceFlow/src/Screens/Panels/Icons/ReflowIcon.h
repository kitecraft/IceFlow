#pragma once
#include "IconBase.h"

class ReflowIcon :
    public IconBase
{
private:

public:
    ReflowIcon();
    ReflowIcon(IconBaseDto config, TFT_eSPI* tft);
    void Draw(TFT_eSprite* sprite);
};

