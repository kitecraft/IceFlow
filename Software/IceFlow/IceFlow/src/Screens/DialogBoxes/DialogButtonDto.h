#pragma once
#include <Arduino.h>
#include "TFT_eSPI.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"

enum DialogButtonType {
	DB_NONE = -1,
	DB_Cancel,
	DB_Save,
	DB_Continue,
	DB_OK,
};

class DialogButtonDto {
public:
	DMCoordinates coordinates;
	DMTheme theme;
	const GFXfont* font = nullptr;
	DialogButtonType type;

	DialogButtonDto() {}
	DialogButtonDto(DMCoordinates coordinates_, DMTheme theme_, const GFXfont* font_, DialogButtonType type_) {
		coordinates = coordinates_;
		theme = theme_;
		font = font_;
		type = type_;
	}
	DialogButtonDto(const DialogButtonDto& orig) {
		coordinates = orig.coordinates;
		theme = orig.theme;
		font = orig.font;
		type = orig.type;
	}
};