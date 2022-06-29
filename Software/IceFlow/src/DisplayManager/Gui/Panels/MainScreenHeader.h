#pragma once
#include <Arduino.h>
#include "PanelBase.h"
#include "PanelLayout.h"

#define MSH_PANEL_RADIUS 10
#define MSH_PANEL_BORDER 2

#define ICEFLOW_LABEL_PILLBOX_WIDTH 82
#define ICEFLOW_LABEL_PILLBOX_HEIGHT 30
#define ICEFLOW_LABEL_PILLBOX_X 6
#define ICEFLOW_LABEL_PILLBOX_Y 6
#define ICEFLOW_LABEL_COLOR TFT_CYAN
#define ICEFLOW_LABEL_X (ICEFLOW_LABEL_PILLBOX_X + MSH_PANEL_RADIUS)
#define ICEFLOW_LABEL_Y (1 + ICEFLOW_LABEL_PILLBOX_Y + (ICEFLOW_LABEL_PILLBOX_HEIGHT/2))

#define PROFILE_LABEL_X ((MSH_PANEL_WIDTH/2) - 50)
#define PROFILE_TEXT_COLOR_NONE TFT_RED
#define PROFILE_TEXT_COLOR	TFT_GREEN
#define PROFILE_LABEL_PILLBOX_X (PROFILE_LABEL_X + 64)
#define PROFILE_NAME_X (PROFILE_LABEL_PILLBOX_X + MSH_PANEL_RADIUS)

class MainScreenHeader : 
	public PanelBase
{
private:
	String _profileName;
public:
	~MainScreenHeader() {}
	void DrawPanel(String newProfileName);
};

