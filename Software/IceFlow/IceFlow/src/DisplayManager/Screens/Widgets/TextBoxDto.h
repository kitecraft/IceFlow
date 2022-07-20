#pragma once
#include <Arduino.h>
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class TextBoxDto {
public:
    DMCoordinates coordinates;
    DMTheme theme;
    const GFXfont* font = nullptr;
    int textAlignment = MC_DATUM;
    bool useDark = false;

    TextBoxDto() {}

    TextBoxDto(DMCoordinates coordinates_, DMTheme theme_, const GFXfont* font_, int textAlignment_ = MC_DATUM, bool useDark_ = false) {
        coordinates = coordinates_;
        theme = theme_;
        font = font_;
        textAlignment = textAlignment_;
        useDark = useDark_;
    }

    TextBoxDto(const TextBoxDto& orig) {
        coordinates = orig.coordinates;
        theme = orig.theme;
        font = orig.font;
        textAlignment = orig.textAlignment;
        useDark = orig.useDark;
    }
};