#pragma once
#include "IconBase.h"
class ProfileIcon :
    public IconBase
{
private:

public:
    ProfileIcon();
    ProfileIcon(IconBaseDto config, TFT_eSPI* tft);
    void Draw(TFT_eSprite* sprite);
};

