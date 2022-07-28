#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

class ProfileListItemDto {
public:
    DMCoordinates coordinates;
    DMTheme theme;
    String displayName;
    String fileName;

    ProfileListItemDto() {}

    ProfileListItemDto(DMCoordinates coordinates_, DMTheme theme_, String displayName_, String fileName_) {
        coordinates = coordinates_;
        theme = theme_;
        displayName = displayName_;
        fileName = fileName_;
    }

    ProfileListItemDto(const ProfileListItemDto& orig) {
        coordinates = orig.coordinates;
        theme = orig.theme;
        displayName = orig.displayName;
        fileName = orig.fileName;
    }
};
