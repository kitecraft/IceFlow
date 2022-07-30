#include "PS_ProfileGraphPanel.h"

PS_ProfileGraphPanel::PS_ProfileGraphPanel()
{

}

PS_ProfileGraphPanel::PS_ProfileGraphPanel(TFT_eSPI* tft)
{
	_tft = tft;
}

PS_ProfileGraphPanel::~PS_ProfileGraphPanel()
{
}

void PS_ProfileGraphPanel::CalculateMaxTimeAndTemp()
{
	_maxTime = _profile.pre_heat_ramp_time +
		_profile.pre_heat_soak_time +
		_profile.reflow_ramp_time +
		_profile.reflow_soak_time +
		//(_profile.reflow_max_temperature - _profile.pre_heat_target_temperature) / _profile.cooling_ramp_down_speed;  //time to pre heat temp
		(_profile.reflow_max_temperature - 20) / _profile.cooling_ramp_down_speed;  // time to 20 degrees

	_maxTemp = _profile.reflow_max_temperature;

	Serial.printf("maxTime: %i\nmaxTemp: %i\n", _maxTime, _maxTemp);
}

void PS_ProfileGraphPanel::Draw(String profileFileName)
{
	if (!ProfileManager.GetProfile(profileFileName, &_profile)) {
		Serial.println("Failed to load Profile");
		return;
	}

	CalculateMaxTimeAndTemp();
	ReDraw();
}

void PS_ProfileGraphPanel::ReDraw()
{
	TFT_eSprite sprite[2] = { TFT_eSprite(_tft), TFT_eSprite(_tft) };
	uint16_t* sprPtr[2] = { nullptr };

	sprPtr[TOP_SIDE] = (uint16_t*)sprite[TOP_SIDE].createSprite(PROFILE_GRAPH_W, PROFILE_GRAPH_SPRITE_TOP_H);
	sprPtr[BOTTOM_SIDE] = (uint16_t*)sprite[BOTTOM_SIDE].createSprite(PROFILE_GRAPH_W, PROFILE_GRAPH_SPRITE_H);

	// Move the sprite 1 coordinate datum upwards half the screen height
	// so from coordinate point of view it occupies the bottom of screen
	sprite[BOTTOM_SIDE].setViewport(0, -PROFILE_GRAPH_SPRITE_H, PROFILE_GRAPH_W, PROFILE_GRAPH_H);

	sprite[TOP_SIDE].setFreeFont(SMALL_FONT);
	sprite[TOP_SIDE].setTextColor(GlobalTheme.textColor, TFT_BLACK);

	sprite[BOTTOM_SIDE].setFreeFont(SMALL_FONT);
	sprite[BOTTOM_SIDE].setTextColor(GlobalTheme.textColor, TFT_BLACK);

	sprite[TOP_SIDE].fillSprite(TFT_BLACK);
	sprite[BOTTOM_SIDE].fillSprite(TFT_BLACK);

	DrawSprite(&sprite[TOP_SIDE]);
	_tft->pushImageDMA(PROFILE_GRAPH_X, PROFILE_GRAPH_Y, PROFILE_GRAPH_W, PROFILE_GRAPH_SPRITE_TOP_H, sprPtr[TOP_SIDE]);


	DrawSprite(&sprite[BOTTOM_SIDE]);
	_tft->pushImageDMA(PROFILE_GRAPH_X, PROFILE_GRAPH_Y + PROFILE_GRAPH_SPRITE_H, PROFILE_GRAPH_W, PROFILE_GRAPH_SPRITE_H, sprPtr[BOTTOM_SIDE]);

	_tft->dmaWait();
	sprite[TOP_SIDE].deleteSprite();
	sprite[BOTTOM_SIDE].deleteSprite();
}

void PS_ProfileGraphPanel::DrawSprite(TFT_eSprite* sprite)
{
	sprite->drawRect(GRID_X, GRID_Y, GRID_W, GRID_H, GRID_LINE_COLOR);

	//The lowest temperature
	sprite->setTextDatum(BR_DATUM);
	sprite->drawNumber(BASE_TEMPERATURE, TEXT_WIDTH, PROFILE_GRAPH_H - TEXT_HEIGHT);
	sprite->setTextDatum(BL_DATUM);
	sprite->drawNumber(BASE_TEMPERATURE, PROFILE_GRAPH_W - TEXT_WIDTH, PROFILE_GRAPH_H - TEXT_HEIGHT);
	
	//The highest temperature
	sprite->setTextDatum(TR_DATUM);
	sprite->drawNumber(_maxTemp, TEXT_WIDTH, 2);
	sprite->setTextDatum(TL_DATUM);
	sprite->drawNumber(_maxTemp, PROFILE_GRAPH_W - TEXT_WIDTH, 2);

	//The shortest time
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(0, TEXT_WIDTH, PROFILE_GRAPH_H);

	//The longest time
	sprite->drawNumber(_maxTime, PROFILE_GRAPH_W - TEXT_WIDTH, PROFILE_GRAPH_H);

	if (_showGrid) {
		//Horizontal grid lines
		for (int i = GRID_V_SPACING; i < GRID_H; i += GRID_V_SPACING) {
			sprite->drawFastHLine(GRID_X, GRID_Y + GRID_H - i, GRID_W, GRID_LINE_COLOR);
		}

		//Vertical grid lines
		for (int i = GRID_H_SPACING; i < GRID_W - (GRID_H_SPACING / 2); i += GRID_H_SPACING) {
			sprite->drawFastVLine(GRID_X + i, GRID_Y, GRID_H, GRID_LINE_COLOR);
		}
	}
	

}