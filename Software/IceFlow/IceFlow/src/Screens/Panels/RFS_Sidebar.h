#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"
#include "../Widgets/TextBox.h"
#include "../Widgets/Button.h"

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

#define RFS_CANCEL_BUTTON_X RFS_TARGET_BOX_X
#define RFS_CANCEL_BUTTON_Y (RFS_TARGET_TEXT_BOX_Y + RFS_TARGET_TEXT_BOX_H)
#define RFS_CANCEL_BUTTON_W RFS_SIDEBAR_W
#define RFS_CANCEL_BUTTON_H (240 - RFS_CANCEL_BUTTON_Y)

#define RFS_MSG_OFFSET 12
#define RFS_LABEL_PREHEAT 40
#define RFS_LABEL_SOAK 67
#define RFS_LABEL_RAMP 94
#define RFS_LABEL_REFLOW 121
#define RFS_LABEL_COOLING 148


class RFS_Sidebar
{
private:
	TFT_eSPI* _tft;
	Profile* _profile;
	TextBox* _targetTextBox = nullptr;
	Button* _cancelButton = nullptr;
	void UpdateTime(int time, int y);

	void StartNewStage(String label, String message, int yOffset);

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
	bool CancelTouched(int x, int y) { return _cancelButton->Touched(x, y); }
};

