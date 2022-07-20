#pragma once
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class ProgressBarDto {
public:
    DMCoordinates coordinates;
    DMTheme theme;
    const GFXfont* font = nullptr;
    uint16_t barBackGroundColor = 0x0000;
    uint16_t barFillColor = 0xFFFF;

    ProgressBarDto() {}

    ProgressBarDto(DMCoordinates coordinates_, DMTheme theme_, const GFXfont* font_, uint16_t bgColor, uint16_t fillColor) {
        coordinates = coordinates_;
        theme = theme_;
        font = font_;
        barBackGroundColor = bgColor;
        barFillColor = fillColor;
    }

    ProgressBarDto(const ProgressBarDto& orig) {
        coordinates = orig.coordinates;
        theme = orig.theme;
        font = orig.font;
        barBackGroundColor = orig.barBackGroundColor;
        barFillColor = orig.barFillColor;
    }
};