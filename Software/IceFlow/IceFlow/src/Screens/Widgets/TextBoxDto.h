#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class TextBoxDto {
public:
    DMCoordinates coordinates;
    DMTheme theme;
    const GFXfont* font = nullptr;
    int textAlignment = MC_DATUM;
    bool useDark = false;
    bool useRounded = false;
    uint16_t roundedBlendColor = TFT_BLACK;

    TextBoxDto() {}

    TextBoxDto(DMCoordinates coordinates_, DMTheme theme_, const GFXfont* font_, int textAlignment_ = MC_DATUM, bool useDark_ = false, bool useRounded_ = false, uint16_t roundedBlendColor_ = TFT_BLACK) {
        coordinates = coordinates_;
        theme = theme_;
        font = font_;
        textAlignment = textAlignment_;
        useDark = useDark_;
        useRounded = useRounded_;
        roundedBlendColor = roundedBlendColor_;
    }

    TextBoxDto(const TextBoxDto& orig) {
        coordinates = orig.coordinates;
        theme = orig.theme;
        font = orig.font;
        textAlignment = orig.textAlignment;
        useDark = orig.useDark;
        useRounded = orig.useRounded;
        roundedBlendColor = orig.roundedBlendColor;
    }
};