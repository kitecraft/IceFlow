#include "RFS_Sidebar.h"
#include "../Widgets/Box.h"


RFS_Sidebar::RFS_Sidebar()
{
}

RFS_Sidebar::RFS_Sidebar(TFT_eSPI* tft)
{
	_tft = tft;
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
	sprite.setTextColor(GlobalTheme.textColor, TFT_DARKGREY);

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
	sprite.drawString("Pre-Heat", x, 40);
	sprite.drawFastHLine(3, 78, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.setTextColor(GlobalTheme.panelBorderColor, GlobalTheme.panelLightColor);

	sprite.drawString("Soak", x, 80);
	sprite.drawFastHLine(3, 118, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.drawString("Ramp", x, 120);
	sprite.drawFastHLine(3, 158, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.drawString("ReFlow", x, 160);
	sprite.drawFastHLine(3, 198, RFS_SIDEBAR_W - 6, GlobalTheme.panelBorderColor);

	sprite.drawString("Cooling", x, 200);

	_tft->pushImageDMA(RFS_SIDEBAR_X, RFS_SIDEBAR_Y, RFS_SIDEBAR_W, RFS_SIDEBAR_H, sprPtr);

	_tft->dmaWait();
}