#pragma once

#include "FS.h"
#include "SPIFFS.h"
#include <SPI.h>

static bool LoadFonts() {
	if (!SPIFFS.begin()) {
		Serial.println("LoadFonts() - Failed to start SPIFFS");
		return false;
	}
	bool font_is_missing = false;
	if (SPIFFS.exists(SMALL_FONT_FILE) == false) font_is_missing = true;
	if (SPIFFS.exists(MEDIUM_FONT_FILE) == false) font_is_missing = true;
	if (SPIFFS.exists(LARGE_FONT_FILE) == false) font_is_missing = true;

	if (font_is_missing)
	{
		Serial.println("LoadFonts() - Failed to find all fonts");
		return false;
	}

	return true;
}