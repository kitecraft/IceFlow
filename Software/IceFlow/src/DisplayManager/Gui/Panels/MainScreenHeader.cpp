#include "MainScreenHeader.h"

void MainScreenHeader::DrawPanel(String profileName)
{
	TFT_eSprite sprite = TFT_eSprite(TFT);
	sprite.setColorDepth(16);
	sprite.createSprite(MSH_PANEL_WIDTH, MSH_PANEL_HEIGHT);
	sprite.loadFont(AA_FONT_20PT);
	sprite.setTextDatum(MC_DATUM);

	StarsideTheme theme = g_GlobalTheme;
	theme.panelHeaderColor = theme.panelBGColor;
	DrawRoundedBox(
		&sprite,
		StarsideCoordinates(0, 0, MSH_PANEL_WIDTH, MSH_PANEL_HEIGHT),
		theme,
		MSH_PANEL_RADIUS,
		MSH_PANEL_BORDER);

	DrawRoundedBox(
		&sprite,
		StarsideCoordinates(ICEFLOW_LABEL_PILLBOX_X, ICEFLOW_LABEL_PILLBOX_Y, ICEFLOW_LABEL_PILLBOX_WIDTH, ICEFLOW_LABEL_PILLBOX_HEIGHT),
		g_GlobalTheme,
		MSH_PANEL_RADIUS,
		MSH_PANEL_BORDER);

	sprite.setTextDatum(ML_DATUM);
	sprite.setTextColor(ICEFLOW_LABEL_COLOR);
	sprite.drawString(__DEVICE_NAME__, ICEFLOW_LABEL_X, ICEFLOW_LABEL_Y);

	if (profileName.isEmpty()) {
		sprite.setTextColor(PROFILE_TEXT_COLOR_NONE);
		sprite.drawString("No Profile Loaded", PROFILE_LABEL_X, ICEFLOW_LABEL_Y);
	}
	else if (profileName.equals(PROFILE_ERROR_NAME_TEXT)) {
		sprite.setTextColor(PROFILE_TEXT_COLOR_NONE);
		sprite.drawString("Error loading profile", PROFILE_LABEL_X, ICEFLOW_LABEL_Y);

	}
	else {
		sprite.setTextColor(PROFILE_TEXT_COLOR);
		sprite.drawString("Profile:", PROFILE_LABEL_X, ICEFLOW_LABEL_Y);

		int tWidth = sprite.textWidth(profileName) + (MSH_PANEL_RADIUS*2) + 2;
		DrawRoundedBox(
			&sprite,
			StarsideCoordinates(PROFILE_LABEL_PILLBOX_X, ICEFLOW_LABEL_PILLBOX_Y, tWidth, ICEFLOW_LABEL_PILLBOX_HEIGHT),
			g_GlobalTheme,
			MSH_PANEL_RADIUS,
			MSH_PANEL_BORDER);
		sprite.drawString(profileName, PROFILE_NAME_X, ICEFLOW_LABEL_Y);
	}

	//sprite.pushSprite(MSH_PANEL_X, MSH_PANEL_Y, TFT_TRANSPARENT);
	//sprite.deleteSprite();
}