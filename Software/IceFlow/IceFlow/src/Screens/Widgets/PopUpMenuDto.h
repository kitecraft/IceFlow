#pragma once
#include <Arduino.h>
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class PopUpMenuDto {
public:
    DMCoordinates coordinates;
    int alignment = TL_DATUM;
    DMTheme theme;
    const GFXfont* font = nullptr;
    int fontHeight = 0;

    PopUpMenuDto() {}
    //alignment should be one of:  TR_DATUM, TL_DATUM, BR_DATUM, BL_DATUM
    PopUpMenuDto(DMCoordinates coordinates_, int alignment_, DMTheme theme_, const GFXfont* font_, int fontHeight_) {
        coordinates = coordinates_;
        alignment = alignment_;
        theme = theme_;
        font = font_;
        fontHeight = fontHeight_;
    }
    PopUpMenuDto(const PopUpMenuDto& orig) {
        coordinates = orig.coordinates;
        alignment = orig.alignment;
        theme = orig.theme;
        font = orig.font;
        fontHeight = orig.fontHeight;
    }
};