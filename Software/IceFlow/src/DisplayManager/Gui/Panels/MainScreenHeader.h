#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../../../IceFlow_Config.h"
#include "../../Utilities/StarsideCoordinates.h"
#include "../../Utilities/StarsideTheme.h"

#define MSH_PANEL_WIDTH (TFT_DISPLAY_WIDTH - 72)
#define MSH_PANEL_HEIGHT 40
#define MSH_PANEL_X 1
#define MSH_PANEL_Y 0
#define MSH_PANEL_RADIUS 10
#define MSH_PANEL_BORDER 2

#define ICEFLOW_LABEL_PILLBOX_WIDTH 82
#define ICEFLOW_LABEL_PILLBOX_HEIGHT 30
#define ICEFLOW_LABEL_PILLBOX_X 6
#define ICEFLOW_LABEL_PILLBOX_Y 6
#define ICEFLOW_LABEL_COLOR TFT_CYAN
#define ICEFLOW_LABEL_X (ICEFLOW_LABEL_PILLBOX_X + MSH_PANEL_RADIUS)
#define ICEFLOW_LABEL_Y (1 + ICEFLOW_LABEL_PILLBOX_Y + (ICEFLOW_LABEL_PILLBOX_HEIGHT/2))

#define PROFILE_LABEL_X ((MSH_PANEL_WIDTH/2) - 30)
#define PROFILE_TEXT_COLOR_NONE TFT_RED
#define PROFILE_TEXT_COLOR	TFT_GREEN
#define PROFILE_LABEL_PILLBOX_X (PROFILE_LABEL_X + 60)
#define PROFILE_NAME_X (PROFILE_LABEL_PILLBOX_X + MSH_PANEL_RADIUS)

class MainScreenHeader
{
private:
	TFT_eSPI* TFT;
	String _profileName;
	void DrawRoundedBox(TFT_eSprite* sprite, StarsideCoordinates coordinates, StarsideTheme theme, int radius, int border);
public:
	~MainScreenHeader() {}
	void Init(TFT_eSPI* newTFT);
	void DrawPanel(String newProfileName);
	void UpdateProfileName(String newProfileName);
};

