#pragma once
#include <Arduino.h>
#include "PanelBase.h"
#include "../../../ProfileManager/Profile.h"

#define CP_NORMAL_RADIUS 10
#define CP_NORMAL_BORDER 2
#define BG_MAXTEMP_ADD 10

#define GRID_H_MARGIN 30
#define GRID_V_MARGIN 2

#define GRID_SPRITE_X (CP_NORMAL_X + GRID_H_MARGIN)
#define GRID_SPRITE_Y (CP_NORMAL_Y + 2)
#define GRID_SPRITE_WIDTH (CP_NORMAL_WIDTH - (GRID_H_MARGIN*2))
#define GRID_SPRITE_HEIGHT (CP_NORMAL_HEIGHT - (GRID_V_MARGIN*2))


class CenterPanel_Normal :
    public PanelBase
{
private:
    TFT_eSprite* _gridSprite = nullptr;
    Profile _profile;

    uint16_t _maxTemp;
    uint16_t _minTemp;
    

    void configure();
    void drawBackground();
public:
    void Init(TFT_eSPI* newTFT, Profile currentProfile);
    void Draw();
    void ChangeProfile(Profile newProfile);
    ~CenterPanel_Normal();
    
};

