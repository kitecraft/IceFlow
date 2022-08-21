#include "BaseScreen.h"
#include "Utilities/ScreenNames.h"
#include "Utilities/DMTheme.h"
#include "Widgets/Box.h"
#include "../Utilities/PreferencesManager.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"


BaseScreen::BaseScreen(TFT_eSPI* tft)
{
	_tft = tft;

	_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
	_temperatureStreamStarted = false;
	_primaryTemperature = 20;
	_secondaryTemperature = 20;
	_tertiaryTemperature = 20;

	_graphPanel = new GraphPanel(_tft,
		DMCoordinates(
			0,
			0,
			BASE_GRAPHPANEL_W,
			BASE_GRAPHPANEL_H,
			BASE_GRAPHPANEL_X,
			BASE_GRAPHPANEL_Y),
		false,
		true
	);

	if (!ProfileManager.GetSavedProfile(&_currentProfile)) {
		Serial.println("Failed to load Profile");
	}

	DMTheme temperatureTheme = GlobalTheme;
	temperatureTheme.textColor = PRIMARY_TEMPERATURE_COLOR;

	_primaryTemperatureTB = new TextBox(TextBoxDto(
		DMCoordinates(
			BASE_PRIMARY_TEMPERATURE_TB_X,
			BASE_HEADER_TB_Y,
			BASE_TEMPERATURE_TB_W,
			MEDIUM_FONT_TEXT_BOX_H,
			BASE_FOOTER_X + BASE_PRIMARY_TEMPERATURE_TB_X,
			BASE_FOOTER_Y + BASE_HEADER_TB_Y
		),
		temperatureTheme,
		MEDIUM_FONT,
		MR_DATUM,
		false,
		true,
		GlobalTheme.panelLightColor),
		_tft);

	temperatureTheme.textColor = SEC_TEMPERATURE_COLOR;
	_secondaryTemperatureTB = new TextBox(TextBoxDto(
		DMCoordinates(
			BASE_SEC_TEMPERATURE_TB_X,
			BASE_HEADER_TB_Y,
			BASE_TEMPERATURE_TB_W,
			MEDIUM_FONT_TEXT_BOX_H,
			BASE_FOOTER_X + BASE_SEC_TEMPERATURE_TB_X,
			BASE_FOOTER_Y + BASE_HEADER_TB_Y
		),
		temperatureTheme,
		MEDIUM_FONT,
		MR_DATUM,
		false,
		true,
		GlobalTheme.panelLightColor),
		_tft);
}

BaseScreen::~BaseScreen()
{
	delete _graphPanel;
	delete(_primaryTemperatureTB);
	delete(_secondaryTemperatureTB);

}

bool BaseScreen::UpdateScreen(SCREEN_UPDATE_KEYS key, char* value)
{
	switch (key) {
	case suk_PrimaryTemperature:
		UpdatePrimaryTemp(value);
		break;
	case suk_SecondaryTemperature:
		UpdateSecondaryTemp(value);
		break;
	case suk_TertiaryTemperature:
		UpdateTertiaryTemp(value);
		break;
	case suk_TemperatureStreamStarted:
		_graphPanel->ReDraw();
		_temperatureStreamStarted = true;
		_nextGraphUpdate = millis() + UPDATE_GRAPH_RATE;
		break;
	case suk_Oven_Heaters_On:
		DrawHeatersIcon(true);
		break;
	case suk_Oven_Heaters_Off:
		DrawHeatersIcon(false);
		break;
	default:
		return false;
		break;
	}
	return true;
}

bool BaseScreen::HandleTouch(int x, int y)
{
	if (_primaryTemperatureTB->Touched(x, y)) {
		_graphPanel->ToggleTertiaryVisible();
		return true;
	}
	if (_secondaryTemperatureTB->Touched(x, y)) {
		_graphPanel->ToggleSecondaryVisible();
		return true;
	}

	return false;
}

void BaseScreen::DrawScreen()
{
	_tft->fillScreen(TFT_BLACK);

	DrawHeader();
	DrawFooter();
	DrawHeatersIcon(false);
	_graphPanel->Draw();
}

void BaseScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BASE_HEADER_W, BASE_HEADER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, BASE_HEADER_W, BASE_HEADER_H, 0, 0), BASE_HEADER_H / 2, GlobalTheme);

	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	sprite.drawString(__DEVICE_NAME__, BASE_DEVICE_NAME_X, BASE_DEVICE_NAME_Y);

	int profileLableX = BASE_DEVICE_NAME_X + sprite.textWidth(__DEVICE_NAME__) + BASE_PROFILE_LABEL_X_OFFSET;

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(ML_DATUM);
	sprite.drawString(BASE_PROFILE_LABEL, profileLableX, (BASE_HEADER_H / 2) - 1);

	int profileX = profileLableX + sprite.textWidth(BASE_PROFILE_LABEL);
	TextBox::DrawTextBox(&sprite,
		TextBoxDto(
			DMCoordinates(
				profileX,
				BASE_HEADER_TB_Y,
				-1,
				MEDIUM_FONT_TEXT_BOX_H,
				BASE_DEVICE_NAME_X + BASE_HEADER_X,
				BASE_HEADER_TB_Y + BASE_HEADER_Y),
			GlobalTheme,
			MEDIUM_FONT,
			MC_DATUM,
			false,
			true,
			GlobalTheme.panelLightColor),
		_currentProfile.name.c_str());

	_tft->pushImageDMA(BASE_HEADER_X, BASE_HEADER_Y, BASE_HEADER_W, BASE_HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void BaseScreen::DrawFooter()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BASE_FOOTER_W, BASE_FOOTER_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, BASE_FOOTER_W, BASE_FOOTER_H, 0, 0), BASE_FOOTER_H / 2, GlobalTheme);

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextDatum(ML_DATUM);

	sprite.setTextColor(PRIMARY_TEMPERATURE_COLOR, GlobalTheme.panelLightColor);
	sprite.drawString("Prim: ", BASE_PRIMARY_TEMPERATURE_LABEL_X, BASE_TEMPERATURE_LABEL_Y);

	sprite.setTextColor(SEC_TEMPERATURE_COLOR, GlobalTheme.panelLightColor);
	sprite.drawString("Sec: ", BASE_SEC_TEMPERATURE_LABEL_X, BASE_TEMPERATURE_LABEL_Y);

	_primaryTemperatureTB->Draw(&sprite, "888.88 C");
	_secondaryTemperatureTB->Draw(&sprite, "888.88 C");

	_tft->pushImageDMA(BASE_FOOTER_X, BASE_FOOTER_Y, BASE_FOOTER_W, BASE_FOOTER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void BaseScreen::UpdatePrimaryTemp(char* val)
{
	char value[9];
	snprintf(value, 9, "%s C", val);
	_primaryTemperatureTB->Update(value);
	_primaryTemperature = atof(val);
}

void BaseScreen::UpdateSecondaryTemp(char* val)
{
	char value[9];
	snprintf(value, 9, "%s C", val);
	_secondaryTemperatureTB->Update(value);
	_secondaryTemperature = atof(val);
}

void BaseScreen::UpdateTertiaryTemp(char* val)
{
	char value[9];
	snprintf(value, 9, "%s C", val);
	_tertiaryTemperature = atof(val);
}

void BaseScreen::DrawHeatersIcon(bool status)
{
	int color = GlobalTheme.panelDarkColor;
	if (status) {
		color = TFT_RED;
	}

	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(BASE_HEATER_ICON_W, BASE_HEATER_ICON_H);

	sprite.fillSprite(GlobalTheme.panelLightColor);
	sprite.drawRoundRect(0, 3, BASE_HEATER_ICON_W, BASE_HEATER_ICON_H - 6, 5, GlobalTheme.panelBorderColor);
	sprite.drawRoundRect(1, 4, BASE_HEATER_ICON_W - 2, BASE_HEATER_ICON_H - 8, 5, GlobalTheme.panelBorderColor);
	sprite.fillSmoothRoundRect(2, 5, BASE_HEATER_ICON_W - 4, BASE_HEATER_ICON_H - 10, 5, color, GlobalTheme.panelBorderColor);

	_tft->pushImageDMA(BASE_HEATER_ICON_X, BASE_HEATER_ICON_Y, BASE_HEATER_ICON_W, BASE_HEATER_ICON_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}
