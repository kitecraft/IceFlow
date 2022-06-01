#pragma once
#include "ScreenBase.h"
//#include "Panels/ConfigButton.h"
//#include "Panels/OTAButton.h"

#define SS_PANEL_HEIGHT 48
#define SS_PANEL_X 50
#define SS_PANEL_WIDTH 220
#define SS_PANEL_TEXT_WIDTH 190
#define SS_PANEL_TEXT_HEIGHT 18

#define SS_NET_PANEL_Y 0
#define SS_IP_PANEL_Y (SS_NET_PANEL_Y + SS_PANEL_HEIGHT)

#define SPINNER_X 230
#define SPINNER_Y 150

class SplashScreen : ScreenBase
{
private:
	unsigned long lastUpdate = 0;
	int updateScreenInterval = 15;

	void UpdateCircle();
	int circleLoopCounter;
	int secondaryLoopCounter;
	uint32_t col[8];

	void DrawPanel(int yOffset, String text);
	void UpdatePanel(int yOffset, String text, uint16_t circleColor);

	String networkSSID = "Connecting...";
	void Update_NetworkConnectionInfo(uint16_t circleColor);
	String iAddress;
	void Update_IPAddrInfo(uint16_t circleColor);

public:
	SplashScreen(TFT_eSPI *newTFT);
	~SplashScreen();
	void UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload);
	void UpdateScreenOnInterval();
};
