#include "TemperaturePanel.h"

void TemperaturePanel::DrawPanel(TFT_eSprite* parentSprite)
{
	parentSprite->loadFont(AA_FONT_20PT);
	parentSprite->setTextDatum(ML_DATUM);

	StarsideTheme theme = g_GlobalTheme;
	theme.panelHeaderColor = theme.panelBGColor;
	DrawRoundedBox(
		parentSprite,
		StarsideCoordinates(TEMPERATURE_PANEL_X, TEMPERATURE_PANEL_Y, TEMPERATURE_PANEL_WIDTH, TEMPERATURE_PANEL_HEIGHT),
		theme,
		TEMP_PILLBOX_RADIUS,
		TEMP_PILLBOX_BORDER);

	parentSprite->setTextColor(PRIMARY_TEMP_TEXT_COLOR);
	parentSprite->drawString("Primary:", PRIMARY_LABEL_X, TEMP_LABEL_Y);
	parentSprite->setTextColor(SECONDARY_TEMP_TEXT_COLOR);
	parentSprite->drawString("Secondary:", SECONDARY_LABEL_X, TEMP_LABEL_Y);

	DrawRoundedBox(
		parentSprite,
		StarsideCoordinates(PRIMARY_TEMP_PILLBOX_X, TEMP_PILLBOX_Y, TEMP_PILLBOX_WIDTH, TEMP_PILLBOX_HEIGHT),
		g_GlobalTheme,
		TEMP_PILLBOX_RADIUS,
		TEMP_PILLBOX_BORDER);

	DrawRoundedBox(
		parentSprite,
		StarsideCoordinates(SECONDARY_TEMP_PILLBOX_X, TEMP_PILLBOX_Y, TEMP_PILLBOX_WIDTH, TEMP_PILLBOX_HEIGHT),
		g_GlobalTheme,
		TEMP_PILLBOX_RADIUS,
		TEMP_PILLBOX_BORDER);
}


void TemperaturePanel::UpdateTemperatureValue(StarsideCoordinates coords, double value, uint16_t color)
{
	TFT_eSprite sprite = TFT_eSprite(TFT);

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
	StarsideCoordinates coords(PRIMARY_TEMP_VALUE_X, TEMP_VALUE_Y_UPDATE, TEMP_VALUE_WIDTH, TEMP_VAlUE_HEIGHT);
	UpdateTemperatureValue(coords, value, PRIMARY_TEMP_TEXT_COLOR);
}

void TemperaturePanel::UpdateSecondaryTemperature(double value)
{
	StarsideCoordinates coords(SECONDARY_TEMP_VALUE_X, TEMP_VALUE_Y_UPDATE, TEMP_VALUE_WIDTH, TEMP_VAlUE_HEIGHT);
	UpdateTemperatureValue(coords, value, SECONDARY_TEMP_TEXT_COLOR);
}