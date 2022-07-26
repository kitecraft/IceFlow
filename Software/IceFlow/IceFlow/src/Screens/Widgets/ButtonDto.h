#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class ButtonDto {
public:
    DMCoordinates coordinates;
    DMTheme theme;
    uint16_t buttonColor = TFT_WHITE;
    const GFXfont* font = nullptr;

    ButtonDto() {}
    ButtonDto(DMCoordinates coordinates_, DMTheme theme_, const GFXfont* font_, uint16_t buttonColor_) {
        coordinates = coordinates_;
        theme = theme_;
        font = font_;
        buttonColor = buttonColor_;
    }
    ButtonDto(const ButtonDto& orig) {
        coordinates = orig.coordinates;
        theme = orig.theme;
        font = orig.font;
        buttonColor = orig.buttonColor;
    }
};