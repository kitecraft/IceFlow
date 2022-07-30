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

	TFT_eSprite sprite[2] = { TFT_eSprite(_tft), TFT_eSprite(_tft) };
	uint16_t* sprPtr[2] = { nullptr };

	sprPtr[TOP_SIDE] = (uint16_t*)sprite[TOP_SIDE].createSprite(PROFILE_GRAPH_W, PROFILE_GRAPH_H);
	sprPtr[BOTTOM_SIDE] = (uint16_t*)sprite[BOTTOM_SIDE].createSprite(PROFILE_GRAPH_W, PROFILE_GRAPH_H);
	
	// Move the sprite 1 coordinate datum upwards half the screen height
	// so from coordinate point of view it occupies the bottom of screen
	sprite[BOTTOM_SIDE].setViewport(0, -PROFILE_GRAPH_H, PROFILE_GRAPH_W, PROFILE_GRAPH_H * 2);


	sprite[TOP_SIDE].setFreeFont(SMALL_FONT);
	sprite[BOTTOM_SIDE].setFreeFont(SMALL_FONT);

	sprite[TOP_SIDE].fillSprite(TFT_BLUE);
	sprite[BOTTOM_SIDE].fillSprite(TFT_YELLOW);

	DrawSprite(&sprite[TOP_SIDE]);
	_tft->pushImageDMA(PROFILE_GRAPH_X, PROFILE_GRAPH_Y, PROFILE_GRAPH_W, PROFILE_GRAPH_H, sprPtr[TOP_SIDE]);

	DrawSprite(&sprite[BOTTOM_SIDE]);
	_tft->pushImageDMA(PROFILE_GRAPH_X, PROFILE_GRAPH_Y + PROFILE_GRAPH_H, PROFILE_GRAPH_W, PROFILE_GRAPH_H, sprPtr[BOTTOM_SIDE]);
	sprite[TOP_SIDE].deleteSprite();
	_tft->dmaWait();
	sprite[BOTTOM_SIDE].deleteSprite();

	/*
	sprite[TOP_SIDE].drawLine(0, 0, PROFILE_GRAPH_W, PROFILE_GRAPH_H * 2, TFT_BLACK);
	sprite[BOTTOM_SIDE].drawLine(0, 0, PROFILE_GRAPH_W, PROFILE_GRAPH_H * 2, TFT_BLACK);
	sprite[TOP_SIDE].drawLine(PROFILE_GRAPH_W, 0, 0, PROFILE_GRAPH_H * 2, TFT_BLACK);
	sprite[BOTTOM_SIDE].drawLine(PROFILE_GRAPH_W, 0, 0, PROFILE_GRAPH_H * 2, TFT_BLACK);

	//uint16_t* _sprPtrBuffer = (uint16_t*)ps_malloc((PROFILE_GRAPH_W * PROFILE_GRAPH_H) * 2);
	sprite[TOP_SIDE].pushSprite(PROFILE_GRAPH_X, PROFILE_GRAPH_Y);
	sprite[BOTTOM_SIDE].pushSprite(PROFILE_GRAPH_X, PROFILE_GRAPH_Y + PROFILE_GRAPH_H);
	*/
	


}

void PS_ProfileGraphPanel::DrawSprite(TFT_eSprite* sprite)
{
	sprite->drawLine(0, 0, PROFILE_GRAPH_W, PROFILE_GRAPH_H * 2, TFT_BLACK);
	sprite->drawLine(PROFILE_GRAPH_W, 0, 0, PROFILE_GRAPH_H * 2, TFT_BLACK);

}