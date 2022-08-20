#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"

#define RFS_SIDEBAR_W 48
#define RFS_SIDEBAR_H 178
#define RFS_SIDEBAR_X (320 - RFS_SIDEBAR_W)
#define RFS_SIDEBAR_Y 0

class RFS_Sidebar
{
private:
	TFT_eSPI* _tft;
	Profile* _profile;

	void UpdateTime(int time, int y);

public:
	RFS_Sidebar();
	RFS_Sidebar(TFT_eSPI* tft, Profile* profile);
	~RFS_Sidebar();

	void UpdatePreHeatTime(int time);
	void EndPreHeat();

	void UpdateSoakTime(int time);
	void EndSoak();

	void UpdateRampTime(int time);
	void EndRamp();

	void UpdateReflowTime(int time);
	void EndReflow();

	void UpdateCoolingTime(int time);
	void EndCooling();

	void Draw();
};

