#pragma once
#include <Arduino.h>
#include "PanelBase.h"


#define PRIMARY_TEMP_TEXT_COLOR TFT_GREEN
#define SECONDARY_TEMP_TEXT_COLOR TFT_CYAN

#define PRIMARY_LABEL_X 8
#define SECONDARY_LABEL_X ((TEMPERATURE_PANEL_WIDTH/2) - 8)
#define TEMP_LABEL_Y (TEMPERATURE_PANEL_Y + 2 + TEMPERATURE_PANEL_HEIGHT/2)
#define TEMP_PILLBOX_WIDTH 104
#define TEMP_PILLBOX_HEIGHT 26
#define TEMP_PILLBOX_RADIUS (TEMP_PILLBOX_HEIGHT/2)
#define TEMP_PILLBOX_BORDER 2

#define PRIMARY_TEMP_PILLBOX_X (PRIMARY_LABEL_X + 76)
#define SECONDARY_TEMP_PILLBOX_X (SECONDARY_LABEL_X + 102)
#define TEMP_PILLBOX_Y (TEMPERATURE_PANEL_Y + ((TEMPERATURE_PANEL_HEIGHT/2) - (TEMP_PILLBOX_HEIGHT/2)) + 1) 

#define PRIMARY_TEMP_VALUE_X (PRIMARY_TEMP_PILLBOX_X + TEMP_PILLBOX_RADIUS)
#define SECONDARY_TEMP_VALUE_X (SECONDARY_TEMP_PILLBOX_X + TEMP_PILLBOX_RADIUS)
#define TEMP_VALUE_Y (TEMPERATURE_PANEL_Y + TEMP_PILLBOX_Y + 5)
#define TEMP_VALUE_Y_UPDATE (TEMP_PILLBOX_Y + 5)
#define TEMP_VALUE_WIDTH (TEMP_PILLBOX_WIDTH - (TEMP_PILLBOX_RADIUS*2))
#define TEMP_VAlUE_HEIGHT (TEMP_PILLBOX_HEIGHT - 10)


class TemperaturePanel :
	public PanelBase
{
private:
	void UpdateTemperatureValue(StarsideCoordinates coords, double value, uint16_t color);
public:
	~TemperaturePanel() {}
	void DrawPanel(TFT_eSprite* parentSprite);
	void UpdatePrimaryTemperature(double value);
	void UpdateSecondaryTemperature(double value);
};

