#include "MainScreenHeader.h"

void MainScreenHeader::DrawPanel(TFT_eSprite *parentSprite, String profileName)
{
	//TFT_eSprite sprite = TFT_eSprite(TFT);
	parentSprite->setColorDepth(16);
	parentSprite->createSprite(MSH_PANEL_WIDTH, MSH_PANEL_HEIGHT);
	parentSprite->loadFont(AA_FONT_20PT);
	parentSprite->setTextDatum(MC_DATUM);

	parentSprite->fillSprite(TFT_TRANSPARENT);

	StarsideTheme theme = g_GlobalTheme;
	theme.panelHeaderColor = theme.panelBGColor;
	DrawRoundedBox(
		parentSprite,
		StarsideCoordinates(0, 0, MSH_PANEL_WIDTH, MSH_PANEL_HEIGHT),
		theme,
		MSH_PANEL_RADIUS,
		MSH_PANEL_BORDER);

	DrawRoundedBox(
		parentSprite,
		StarsideCoordinates(ICEFLOW_LABEL_PILLBOX_X, ICEFLOW_LABEL_PILLBOX_Y, ICEFLOW_LABEL_PILLBOX_WIDTH, ICEFLOW_LABEL_PILLBOX_HEIGHT),
		g_GlobalTheme,
		MSH_PANEL_RADIUS,
		MSH_PANEL_BORDER);

	parentSprite->setTextDatum(ML_DATUM);
	parentSprite->setTextColor(ICEFLOW_LABEL_COLOR);
	parentSprite->drawString(__DEVICE_NAME__, ICEFLOW_LABEL_X, ICEFLOW_LABEL_Y);

	if (profileName.isEmpty()) {
		parentSprite->setTextColor(PROFILE_TEXT_COLOR_NONE);
		parentSprite->drawString("No Profile Loaded", PROFILE_LABEL_X, ICEFLOW_LABEL_Y);
	}
	else if (profileName.equals(PROFILE_ERROR_NAME_TEXT)) {
		parentSprite->setTextColor(PROFILE_TEXT_COLOR_NONE);
		parentSprite->drawString("Error loading profile", PROFILE_LABEL_X, ICEFLOW_LABEL_Y);

	}
	else {
		parentSprite->setTextColor(PROFILE_TEXT_COLOR);
		parentSprite->drawString("Profile:", PROFILE_LABEL_X, ICEFLOW_LABEL_Y);

		int tWidth = parentSprite->textWidth(profileName) + (MSH_PANEL_RADIUS*2) + 2;
		DrawRoundedBox(
			parentSprite,
			StarsideCoordinates(PROFILE_LABEL_PILLBOX_X, ICEFLOW_LABEL_PILLBOX_Y, tWidth, ICEFLOW_LABEL_PILLBOX_HEIGHT),
			g_GlobalTheme,
			MSH_PANEL_RADIUS,
			MSH_PANEL_BORDER);
		parentSprite->drawString(profileName, PROFILE_NAME_X, ICEFLOW_LABEL_Y);
	}

	//sprite.pushSprite(MSH_PANEL_X, MSH_PANEL_Y, TFT_TRANSPARENT);
	//sprite.deleteSprite();
}