#include "TemperaturePanel.h"

void TemperaturePanel::DrawPanel()
{
	TFT_eSprite sprite = TFT_eSprite(TFT);
	sprite.setColorDepth(16);
	sprite.createSprite(TEMPERATURE_PANEL_WIDTH, TEMPERATURE_PANEL_HEIGHT);
	sprite.loadFont(AA_FONT_20PT);
	sprite.setTextDatum(ML_DATUM);

	sprite.fillSprite(TFT_TRANSPARENT);

	StarsideTheme theme = g_GlobalTheme;
	theme.panelHeaderColor = theme.panelBGColor;
	DrawRoundedBox(
		&sprite,
		StarsideCoordinates(0, 0, TEMPERATURE_PANEL_WIDTH, TEMPERATURE_PANEL_HEIGHT),
		theme,
		TEMP_PILLBOX_RADIUS,
		TEMP_PILLBOX_BORDER);

	sprite.setTextColor(PRIMARY_TEMP_TEXT_COLOR);
	sprite.drawString("Primary:", PRIMARY_LABEL_X, TEMP_LABEL_Y);
	sprite.setTextColor(SECONDARY_TEMP_TEXT_COLOR);
	sprite.drawString("Secondary:", SECONDARY_LABEL_X, TEMP_LABEL_Y);

	DrawRoundedBox(
		&sprite,
		StarsideCoordinates(PRIMARY_TEMP_PILLBOX_X, TEMP_PILLBOX_Y, TEMP_PILLBOX_WIDTH, TEMP_PILLBOX_HEIGHT),
		g_GlobalTheme,
		TEMP_PILLBOX_RADIUS,
		TEMP_PILLBOX_BORDER);

	DrawRoundedBox(
		&sprite,
		StarsideCoordinates(SECONDARY_TEMP_PILLBOX_X, TEMP_PILLBOX_Y, TEMP_PILLBOX_WIDTH, TEMP_PILLBOX_HEIGHT),
		g_GlobalTheme,
		TEMP_PILLBOX_RADIUS,
		TEMP_PILLBOX_BORDER);
	
	sprite.pushSprite(TEMPERATURE_PANEL_X, TEMPERATURE_PANEL_Y, TFT_TRANSPARENT);
	sprite.deleteSprite();
}


void TemperaturePanel::UpdateTemperatureValue(StarsideCoordinates coords, double value, uint16_t color)
{
	TFT_eSprite sprite = TFT_eSprite(TFT);

	sprite.setColorDepth(16);
	sprite.createSprite(coords.w, coords.h);
	sprite.fillSprite(g_GlobalTheme.panelHeaderColor);
	sprite.loadFont(AA_FONT_20PT);
	sprite.setTextColor(color);
	sprite.setTextDatum(MR_DATUM);
	String temp = String(value) + " C";
	sprite.drawString(temp, coords.w, 2 + (coords.h/2));
	sprite.pushSprite(coords.x, coords.y, TFT_TRANSPARENT);
	sprite.unloadFont();
	sprite.deleteSprite();
}


void TemperaturePanel::UpdatePrimaryTemperature(double value)
{
	StarsideCoordinates coords(PRIMARY_TEMP_VALUE_X, TEMP_VALUE_Y, TEMP_VALUE_WIDTH, TEMP_VAlUE_HEIGHT);
	UpdateTemperatureValue(coords, value, PRIMARY_TEMP_TEXT_COLOR);
}

void TemperaturePanel::UpdateSecondaryTemperature(double value)
{
	StarsideCoordinates coords(SECONDARY_TEMP_VALUE_X, TEMP_VALUE_Y, TEMP_VALUE_WIDTH, TEMP_VAlUE_HEIGHT);
	UpdateTemperatureValue(coords, value, SECONDARY_TEMP_TEXT_COLOR);
}