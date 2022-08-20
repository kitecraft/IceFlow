#include "RFS_Sidebar.h"
#include "../Widgets/Box.h"


RFS_Sidebar::RFS_Sidebar()
{
}

RFS_Sidebar::RFS_Sidebar(TFT_eSPI* tft, Profile* profile)
{
	_tft = tft;
	_profile = profile;
	_targetTextBox = new TextBox(TextBoxDto(
		DMCoordinates(
			RFS_TARGET_TEXT_BOX_X,
			RFS_TARGET_TEXT_BOX_Y,
			RFS_TARGET_TEXT_BOX_W,
			RFS_TARGET_TEXT_BOX_H,
			RFS_SIDEBAR_X + RFS_TARGET_TEXT_BOX_X,
			RFS_TARGET_TEXT_BOX_Y
		),
		GlobalTheme,
		SMALL_FONT,
		MR_DATUM,
		false,
		true,
		GlobalTheme.panelLightColor),
		_tft);

	_cancelButton = new Button(
		ButtonDto(
			DMCoordinates(
				RFS_CANCEL_BUTTON_X,
				RFS_CANCEL_BUTTON_Y,
				RFS_CANCEL_BUTTON_W,
				RFS_CANCEL_BUTTON_H,
				RFS_SIDEBAR_X + RFS_CANCEL_BUTTON_X,
				RFS_CANCEL_BUTTON_Y),
			GlobalTheme,
			SMALL_FONT,
			BUTTON_COLOR),
		"Cancel",
		_tft);

}

RFS_Sidebar::~RFS_Sidebar()
{
	delete _targetTextBox;
	delete _cancelButton;
}

void RFS_Sidebar::Draw()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W, RFS_SIDEBAR_H);
	sprite.fillSprite(TFT_BLACK);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, RFS_SIDEBAR_W, RFS_SIDEBAR_STAGE_H, 0, 0), 12, GlobalTheme);
	DrawRoundedBox(&sprite, DMCoordinates(RFS_TARGET_BOX_X, RFS_TARGET_BOX_Y, RFS_TARGET_BOX_W, RFS_TARGET_BOX_H, 0, 0), 12, GlobalTheme);
	_targetTextBox->Draw(&sprite, "      ");
	_cancelButton->Draw(&sprite);

	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	sprite.setTextDatum(TC_DATUM);
	sprite.drawString("Reflow", RFS_SIDEBAR_W/2, 5);
	sprite.drawString("Stage", RFS_SIDEBAR_W/2, 17);
	sprite.drawFastHLine(3, 29, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);
	sprite.drawFastHLine(3, 30, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);
	sprite.drawFastHLine(3, 33, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);
	sprite.drawFastHLine(3, 34, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);
	sprite.drawFastHLine(3, 37, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);
	sprite.drawFastHLine(3, 38, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.drawString("Target", RFS_SIDEBAR_W / 2, RFS_TARGET_BOX_Y + 5);

	int x = RFS_SIDEBAR_W - 4;
	sprite.setTextDatum(TR_DATUM);

	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	String message = String(_profile->pre_heat_target_temperature) + " C";
	sprite.drawString("Pre-Heat", x, RFS_LABEL_PREHEAT);
	sprite.drawString(message, x, RFS_LABEL_PREHEAT + RFS_MSG_OFFSET);
	sprite.drawFastHLine(3, 64, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.setTextColor(TFT_DARKGREY, GlobalTheme.panelLightColor);
	message = String(_profile->pre_heat_soak_time) + " sec";
	sprite.drawString("Soak", x, RFS_LABEL_SOAK);
	sprite.drawString(message, x, RFS_LABEL_SOAK + RFS_MSG_OFFSET);
	sprite.drawFastHLine(3, 91, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->reflow_target_temperature) + " C";
	sprite.drawString("Ramp", x,RFS_LABEL_RAMP);
	sprite.drawString(message, x, RFS_LABEL_RAMP + RFS_MSG_OFFSET);
	sprite.drawFastHLine(3, 118, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->reflow_soak_time) + " sec";
	sprite.drawString("ReFlow", x, RFS_LABEL_REFLOW);
	sprite.drawString(message, x, RFS_LABEL_REFLOW + RFS_MSG_OFFSET);
	sprite.drawFastHLine(3, 145, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->cooling_ramp_down_speed) + " C/sec";
	sprite.drawString("Cooling", x, RFS_LABEL_COOLING);
	sprite.drawString(message, x, RFS_LABEL_COOLING + RFS_MSG_OFFSET);

	_tft->pushImageDMA(RFS_SIDEBAR_X, RFS_SIDEBAR_Y, RFS_SIDEBAR_W, RFS_SIDEBAR_H, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::UpdateTime(int time, int yValue)
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W - 6, SMALL_FONT_TEXT_H);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextDatum(TR_DATUM);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	String message = String(String(time) + " sec");
	sprite.drawString(message, RFS_SIDEBAR_W - 7, 0);
	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, yValue, RFS_SIDEBAR_W - 6, SMALL_FONT_TEXT_H, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::StartNewStage(String label, String message, int yOffset)
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W - 6, 23);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextDatum(TR_DATUM);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	sprite.drawString(label, RFS_SIDEBAR_W - 6, 0);
	sprite.drawString(message, RFS_SIDEBAR_W - 6, 12);

	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, yOffset, RFS_SIDEBAR_W - 6, 23, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();

}

void RFS_Sidebar::UpdatePreHeatTime(int time)
{
	UpdateTime(time, RFS_LABEL_PREHEAT + RFS_MSG_OFFSET);
}

void RFS_Sidebar::EndPreHeatStage()
{
	String message = String(_profile->pre_heat_soak_time) + " sec";
	StartNewStage("Soak", message, RFS_LABEL_SOAK);
}

void RFS_Sidebar::UpdateSoakTime(int time)
{
	UpdateTime(time, RFS_LABEL_SOAK + RFS_MSG_OFFSET);
}

void RFS_Sidebar::EndSoakStage()
{
	String message = String("0 sec");
	StartNewStage("Ramp", message, RFS_LABEL_RAMP);
}

void RFS_Sidebar::UpdateRampTime(int time)
{
	UpdateTime(time, RFS_LABEL_RAMP + RFS_MSG_OFFSET);
}

void RFS_Sidebar::EndRampStage()
{
	String message = String(_profile->reflow_soak_time) + " sec";
	StartNewStage("Reflow", message, RFS_LABEL_REFLOW);
}

void RFS_Sidebar::UpdateReflowTime(int time)
{
	UpdateTime(time, RFS_LABEL_REFLOW + RFS_MSG_OFFSET);
}

void RFS_Sidebar::EndReflowStage()
{
	String message = "0 sec";
	StartNewStage("Cooling", message, RFS_LABEL_COOLING);
}

void RFS_Sidebar::UpdateCoolingTime(int time)
{
	UpdateTime(time, RFS_LABEL_COOLING + RFS_MSG_OFFSET);
}

void RFS_Sidebar::EndCoolingStage()
{

}

void RFS_Sidebar::UpdateTarget(String text)
{
	_targetTextBox->Update(text);
}