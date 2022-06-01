#pragma once
#include "ScreenBase.h"
//#include "Buttons/ExitButton.h"

#define TEXT_COLOR 0xCE59
#define PANEL_HCOLOR 0x5AEB
#define PANEL_BGCOLOR 0x738E
#define BOX_BORDER_COLOR 0x9CD3
#define BOX_DROP_SHADOW 0x4208

#define OTA_PANEL_X 40
#define OTA_PANEL_Y 45
#define OTA_PANEL_WIDTH 240
#define OTA_PANEL_HEIGHT 130

class OTAScreen :
	public ScreenBase
{
private:
	//ExitButton exitButton;

	String networkName = "";

	unsigned long lastUpdate = 0;
	int updateScreenInterval = 100;
	int circleLoopCounter;	

	void DrawEntireScreen();

	void Draw_OTA_Information_Panel();
	void RedrawNetworkName();
	void RedrawIPAddress(String ipAddr);

	bool OTA_Update_In_Progress;
	void Starting_OTA_Update();
	void Draw_Waiting_Animation();
	void Update_In_Progress(int percentage);
	void Ending_OTA_Update();

public:
	OTAScreen(TFT_eSPI *newTFT);
	~OTAScreen();
	void UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload);
	void UpdateScreenOnInterval();

	void HandleTouch(int x, int y);
};

