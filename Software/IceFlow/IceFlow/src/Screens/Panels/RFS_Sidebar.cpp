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
	sprite.setTextColor(TFT_DARKGREY, GlobalTheme.panelLightColor);
	sprite.setTextDatum(TR_DATUM);

	String message = String(_profile->pre_heat_target_temperature) + " C";
	sprite.drawString("Pre-Heat", x, 40);
	sprite.drawString(message, x, 52);
	sprite.drawFastHLine(3, 78, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	message = String(_profile->pre_heat_soak_time) + " sec";
	sprite.drawString("Soak", x, 80);
	sprite.drawString(message, x, 92);
	sprite.drawFastHLine(3, 118, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->reflow_target_temperature) + " C";
	sprite.drawString("Ramp", x, 120);
	sprite.drawString(message, x, 132);
	sprite.drawFastHLine(3, 158, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->reflow_soak_time) + " sec";
	sprite.drawString("ReFlow", x, 160);
	sprite.drawString(message, x, 172);
	sprite.drawFastHLine(3, 198, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);


	message = String(_profile->cooling_ramp_down_speed) + " C/sec";
	sprite.drawString("Cooling", x, 200);
	sprite.drawString(message, x, 212);

	_tft->pushImageDMA(RFS_SIDEBAR_X, RFS_SIDEBAR_Y, RFS_SIDEBAR_W, RFS_SIDEBAR_H, sprPtr);

	_tft->dmaWait();
}

void RFS_Sidebar::HandlePreHeat()
{

}

void RFS_Sidebar::HandleSoak()
{

}

void RFS_Sidebar::HandleRamp()
{

}

void RFS_Sidebar::HandleReflow()
{

}

void RFS_Sidebar::HandleCooling()
{

}
