#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"
#include "../../OvenController/ReflowStage.h"

#define RFS_SIDEBAR_W 48
#define RFS_SIDEBAR_H 240
#define RFS_SIDEBAR_X (320 - RFS_SIDEBAR_W)
#define RFS_SIDEBAR_Y 0

class RFS_Sidebar
{
private:
	TFT_eSPI* _tft;
	Profile* _profile;
	REFLOW_STAGE _reflowStage = RS_NOT_ACTIVE;

	void HandlePreHeat();
	void HandleSoak();
	void HandleRamp();
	void HandleReflow();
	void HandleCooling();

public:
	RFS_Sidebar();
	RFS_Sidebar(TFT_eSPI* tft, Profile* profile);
	~RFS_Sidebar();

	void UpdateOnInterval();
	void Draw();
};

