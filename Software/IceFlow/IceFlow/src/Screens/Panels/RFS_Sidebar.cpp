#include "RFS_Sidebar.h"
#include "../Widgets/Box.h"


RFS_Sidebar::RFS_Sidebar()
{
}

RFS_Sidebar::RFS_Sidebar(TFT_eSPI* tft, Profile* profile)
{
	_tft = tft;
	_profile = profile;
}

RFS_Sidebar::~RFS_Sidebar()
{

}

void RFS_Sidebar::Draw()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W, RFS_SIDEBAR_H);
	sprite.fillSprite(TFT_BLACK);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, RFS_SIDEBAR_W, RFS_SIDEBAR_H, 0, 0), 12, GlobalTheme);

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

	int x = RFS_SIDEBAR_W - 4;
	sprite.setTextDatum(TR_DATUM);

	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	String message = String(_profile->pre_heat_target_temperature) + " C";
	sprite.drawString("Pre-Heat", x, 40);
	sprite.drawString(message, x, 52);
	sprite.drawFastHLine(3, 64, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.setTextColor(TFT_DARKGREY, GlobalTheme.panelLightColor);
	message = String(_profile->pre_heat_soak_time) + " sec";
	sprite.drawString("Soak", x, 67);
	sprite.drawString(message, x, 79);
	sprite.drawFastHLine(3, 91, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->reflow_target_temperature) + " C";
	sprite.drawString("Ramp", x, 94);
	sprite.drawString(message, x, 106);
	sprite.drawFastHLine(3, 118, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->reflow_soak_time) + " sec";
	sprite.drawString("ReFlow", x, 121);
	sprite.drawString(message, x, 133);
	sprite.drawFastHLine(3, 145, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->cooling_ramp_down_speed) + " C/sec";
	sprite.drawString("Cooling", x, 148);
	sprite.drawString(message, x, 160);

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

	String message = String(String(time) + " secs");
	sprite.drawString(message, RFS_SIDEBAR_W - 7, 0);
	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, yValue, RFS_SIDEBAR_W - 6, SMALL_FONT_TEXT_H, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::UpdatePreHeatTime(int time)
{
	UpdateTime(time, 52);
}

void RFS_Sidebar::EndPreHeatStage()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W - 6, 23);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextDatum(TR_DATUM);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	String message = String(_profile->pre_heat_soak_time) + " secs";
	sprite.drawString("Soak", RFS_SIDEBAR_W - 6, 0);
	sprite.drawString(message, RFS_SIDEBAR_W - 6, 12);

	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, 67, RFS_SIDEBAR_W - 6, 23, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::UpdateSoakTime(int time)
{
	UpdateTime(time, 79);
}

void RFS_Sidebar::EndSoakStage()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W - 6, 23);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextDatum(TR_DATUM);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	String message = String("0 secs");
	sprite.drawString("Ramp", RFS_SIDEBAR_W - 6, 0);
	sprite.drawString(message, RFS_SIDEBAR_W - 6, 12);

	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, 94, RFS_SIDEBAR_W - 6, 23, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::UpdateRampTime(int time)
{
	UpdateTime(time, 106);
}

void RFS_Sidebar::EndRampStage()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W - 6, 23);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextDatum(TR_DATUM);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	String message = String(_profile->reflow_soak_time) + " secs";
	sprite.drawString("Reflow", RFS_SIDEBAR_W - 6, 0);
	sprite.drawString(message, RFS_SIDEBAR_W - 6, 12);

	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, 121, RFS_SIDEBAR_W - 6, 23, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::UpdateReflowTime(int time)
{
	UpdateTime(time, 133);
}

void RFS_Sidebar::EndReflowStage()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(RFS_SIDEBAR_W - 6, 23);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextDatum(TR_DATUM);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);

	String message = "0 secs";
	sprite.drawString("Cooling", RFS_SIDEBAR_W - 6, 0);
	sprite.drawString(message, RFS_SIDEBAR_W - 6, 12);

	_tft->pushImageDMA(320 - RFS_SIDEBAR_W + 3, 148, RFS_SIDEBAR_W - 6, 23, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void RFS_Sidebar::UpdateCoolingTime(int time)
{
	UpdateTime(time, 160);
}

void RFS_Sidebar::EndCoolingStage()
{

}