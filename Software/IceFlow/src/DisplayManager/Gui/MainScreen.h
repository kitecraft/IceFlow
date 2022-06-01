#pragma once
#include "ScreenBase.h"
#include "../../ProfileManager/ProfileManager.h"
#include "../../ProfileManager/Profile.h"
#include "Panels/MainScreenSideBar.h"

#define PROBE_BOX_WIDTH 100
#define PROBE_BOX_HEIGHT 100
#define PROBE_BOX_HEADER_HEIGHT 24

#define PROBE_A_HEADER_X 3
#define PROBE_A_HEADER_Y 3

#define PROBE_B_HEADER_X 104
#define PROBE_B_HEADER_Y 3

extern ProfileManager g_profileManager;

class MainScreen :
    public ScreenBase
{
private:
	MainScreenSideBar _sideBar;
	String _currentProfileFileName = "";

	void DrawScreen();
	void DrawProfile();

public:
	MainScreen(TFT_eSPI* newTFT);
	~MainScreen();


	void UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload);
	void UpdateScreenOnInterval();
	void HandleTouch(int x, int y);
};

