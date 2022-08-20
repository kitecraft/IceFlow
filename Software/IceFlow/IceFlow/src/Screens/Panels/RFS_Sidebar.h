#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"
#include "../Widgets/TextBox.h"

#define RFS_SIDEBAR_W 48
#define RFS_SIDEBAR_STAGE_H 178
#define RFS_SIDEBAR_H 240
#define RFS_SIDEBAR_X (320 - RFS_SIDEBAR_W)
#define RFS_SIDEBAR_Y 0

#define RFS_TARGET_BOX_X 0
#define RFS_TARGET_BOX_Y RFS_SIDEBAR_STAGE_H
#define RFS_TARGET_BOX_W RFS_SIDEBAR_W + 2
#define RFS_TARGET_BOX_H 35

#define RFS_TARGET_TEXT_BOX_X (RFS_TARGET_BOX_X)
#define RFS_TARGET_TEXT_BOX_Y (RFS_TARGET_BOX_Y + 17)
#define RFS_TARGET_TEXT_BOX_W RFS_TARGET_BOX_W
#define RFS_TARGET_TEXT_BOX_H SMALL_FONT_TEXT_BOX_H

class RFS_Sidebar
{
private:
	TFT_eSPI* _tft;
	Profile* _profile;
	TextBox* _targetTextBox = nullptr;
	void UpdateTime(int time, int y);

public:
	RFS_Sidebar();
	RFS_Sidebar(TFT_eSPI* tft, Profile* profile);
	~RFS_Sidebar();

	void UpdatePreHeatTime(int time);
	void EndPreHeatStage();

	void UpdateSoakTime(int time);
	void EndSoakStage();

	void UpdateRampTime(int time);
	void EndRampStage();

	void UpdateReflowTime(int time);
	void EndReflowStage();

	void UpdateCoolingTime(int time);
	void EndCoolingStage();

	void UpdateTarget(String text);

	void Draw();
};

