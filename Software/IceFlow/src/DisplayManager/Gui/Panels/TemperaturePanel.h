#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../../IceFlow_Config.h"
#include "../../Utilities/StarsideCoordinates.h"
#include "../../Utilities/StarsideTheme.h"

#define TEMPERATURE_PANEL_WIDTH (TFT_DISPLAY_WIDTH - 72)
#define TEMPERATURE_PANEL_HEIGHT 42
#define TEMPERATURE_PANEL_X 1
#define TEMPERATURE_PANEL_Y (TFT_DISPLAY_HEIGHT - TEMPERATURE_PANEL_HEIGHT)

#define PRIMARY_TEMP_TEXT_COLOR TFT_GREEN
#define SECONDARY_TEMP_TEXT_COLOR TFT_CYAN

#define PRIMARY_LABEL_X 8
#define SECONDARY_LABEL_X ((TEMPERATURE_PANEL_WIDTH/2) - 8)
#define TEMP_LABEL_Y (2 + TEMPERATURE_PANEL_HEIGHT/2)
#define TEMP_PILLBOX_WIDTH 104
#define TEMP_PILLBOX_HEIGHT 26
#define TEMP_PILLBOX_RADIUS (TEMP_PILLBOX_HEIGHT/2)
#define TEMP_PILLBOX_BORDER 2

#define PRIMARY_TEMP_PILLBOX_X (PRIMARY_LABEL_X + 76)
#define SECONDARY_TEMP_PILLBOX_X (SECONDARY_LABEL_X + 102)
#define TEMP_PILLBOX_Y (((TEMPERATURE_PANEL_HEIGHT/2) - (TEMP_PILLBOX_HEIGHT/2)) + 1) 

#define PRIMARY_TEMP_VALUE_X (PRIMARY_TEMP_PILLBOX_X + TEMP_PILLBOX_RADIUS)
#define SECONDARY_TEMP_VALUE_X (SECONDARY_TEMP_PILLBOX_X + TEMP_PILLBOX_RADIUS)
#define TEMP_VALUE_Y (TEMPERATURE_PANEL_Y + TEMP_PILLBOX_Y + 5)
#define TEMP_VALUE_WIDTH (TEMP_PILLBOX_WIDTH - (TEMP_PILLBOX_RADIUS*2))
#define TEMP_VAlUE_HEIGHT (TEMP_PILLBOX_HEIGHT - 10)


class TemperaturePanel
{
private:
	TFT_eSPI* TFT;
	void DrawRoundedBox(TFT_eSprite* sprite, StarsideCoordinates coordinates, StarsideTheme theme, int radius, int border);
	void UpdateTemperatureValue(StarsideCoordinates coords, double value, uint16_t color);
public:
	~TemperaturePanel() {}
	void Init(TFT_eSPI* newTFT);
	void DrawPanel();
	void UpdatePrimaryTemperature(double value);
	void UpdateSecondaryTemperature(double value);
};

