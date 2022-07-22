#pragma once

class IconBaseDto {
public:
    DMCoordinates coordinates;
    DMTheme theme;
    bool useDark = false;

    IconBaseDto() {}
    IconBaseDto(DMCoordinates coordinates_, DMTheme theme_, bool useDark_ = false) {
        coordinates = coordinates_;
        theme = theme_;
        useDark = useDark_;
    }
    IconBaseDto(const IconBaseDto& orig) {
        coordinates = orig.coordinates;
        theme = orig.theme;
        useDark = orig.useDark;
    }
};