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
		(_profile.reflow_max_temperature - _profile.pre_heat_target_temperature) / _profile.cooling_ramp_down_speed;  //time to pre heat temp
		//(_profile.reflow_max_temperature - 20) / _profile.cooling_ramp_down_speed;  // time to 20 degrees

	_maxTemp = _profile.reflow_max_temperature;

	//Serial.printf("maxTime: %i\nmaxTemp: %i\n", _maxTime, _maxTemp);
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

	//Draw the top half then push it
	DrawSprite(&sprite[TOP_SIDE]);
	_tft->pushImageDMA(PROFILE_GRAPH_X, PROFILE_GRAPH_Y, PROFILE_GRAPH_W, PROFILE_GRAPH_SPRITE_TOP_H, sprPtr[TOP_SIDE]);

	//Draw the bottom half then push it real good
	DrawSprite(&sprite[BOTTOM_SIDE]);
	_tft->pushImageDMA(PROFILE_GRAPH_X, PROFILE_GRAPH_Y + PROFILE_GRAPH_SPRITE_H, PROFILE_GRAPH_W, PROFILE_GRAPH_SPRITE_H, sprPtr[BOTTOM_SIDE]);

	_tft->dmaWait();
	sprite[TOP_SIDE].deleteSprite();
	sprite[BOTTOM_SIDE].deleteSprite();
}

void PS_ProfileGraphPanel::DrawSprite(TFT_eSprite* sprite)
{
	//The lowest temperature
	sprite->setTextDatum(BR_DATUM);
	sprite->drawNumber(BASE_TEMPERATURE, TEXT_WIDTH, PROFILE_GRAPH_H - TEXT_HEIGHT);
	//sprite->setTextDatum(BL_DATUM);
	//sprite->drawNumber(BASE_TEMPERATURE, PROFILE_GRAPH_W - TEXT_WIDTH, PROFILE_GRAPH_H - TEXT_HEIGHT);
	
	//The highest temperature
	//sprite->setTextDatum(TR_DATUM);
	//sprite->drawNumber(_maxTemp, TEXT_WIDTH, 2);
	sprite->setTextDatum(TL_DATUM);
	sprite->drawNumber(_maxTemp, PROFILE_GRAPH_W - TEXT_WIDTH, 2);

	//The shortest time
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(0, TEXT_WIDTH, PROFILE_GRAPH_H);

	//The longest time
	sprite->drawNumber(_maxTime, PROFILE_GRAPH_W - TEXT_WIDTH, PROFILE_GRAPH_H);

	uint16_t gridLinecolor = sprite->alphaBlend(100, GRID_LINE_COLOR, TFT_BLACK);
	//uint16_t gridLinecolor = sprite->alphaBlend(25, GRID_LINE_COLOR, TFT_BLACK);
	//uint16_t gridLinecolor = sprite->alphaBlend(200, GRID_LINE_COLOR, TFT_BLACK);

	sprite->drawRect(GRID_X, GRID_Y, GRID_W, GRID_H, gridLinecolor);
	if (_showGrid) {
		//Horizontal grid lines
		for (int i = GRID_V_SPACING; i < GRID_H; i += GRID_V_SPACING) {
			sprite->drawFastHLine(GRID_X, GRID_Y + GRID_H - i, GRID_W, gridLinecolor);
		}

		//Vertical grid lines
		for (int i = GRID_H_SPACING; i < GRID_W - (GRID_H_SPACING / 2); i += GRID_H_SPACING) {
			sprite->drawFastVLine(GRID_X + i, GRID_Y, GRID_H, gridLinecolor);
		}
	}

	DrawProfileGraph(sprite);
}

void PS_ProfileGraphPanel::DrawProfileGraph(TFT_eSprite* sprite)
{
	double gridBottom = (double)GRID_Y + (double)GRID_H;
	int pointA_X = round((double)GRID_X + ((double)_profile.pre_heat_ramp_time / (double)_maxTime) * (double)GRID_W);
	int pointA_Y = round((gridBottom - ((double)_profile.pre_heat_target_temperature / (double)_maxTemp) * (double)GRID_H));
	
	//The pre-heat segment
	sprite->drawFastHLine(GRID_X, pointA_Y, pointA_X - GRID_X, GRID_LINE_COLOR);
	sprite->drawFastVLine(pointA_X, pointA_Y, gridBottom - pointA_Y, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(_profile.pre_heat_ramp_time, pointA_X, PROFILE_GRAPH_H);
	sprite->setTextDatum(TR_DATUM);
	sprite->drawNumber(_profile.pre_heat_target_temperature, TEXT_WIDTH, pointA_Y);
	sprite->setTextDatum(TL_DATUM);
	sprite->drawNumber(_profile.pre_heat_target_temperature, PROFILE_GRAPH_W - TEXT_WIDTH, pointA_Y);

	
	//The pre-heat soak segment
	int runTime = _profile.pre_heat_ramp_time + _profile.pre_heat_soak_time;
	int pointB_X = round((double)GRID_X + ((double)runTime / (double)_maxTime) * (double)GRID_W);
	int pointB_Y = round((gridBottom - ((double)_profile.pre_heat_soak_end_temperature / (double)_maxTemp) * (double)GRID_H));
	//if the start and end y points are too close together, then don't draw a full line
	if (pointA_Y - pointB_Y > 2) {
		sprite->drawFastHLine(GRID_X, pointB_Y, pointB_X - GRID_X, GRID_LINE_COLOR);
		sprite->setTextDatum(BR_DATUM);
		sprite->drawNumber(_profile.pre_heat_soak_end_temperature, TEXT_WIDTH, pointB_Y);
	}
	sprite->setTextDatum(BC_DATUM);
	sprite->drawString("Soak", pointB_X / 2, pointB_Y);
	sprite->drawFastVLine(pointB_X, pointB_Y, gridBottom - pointB_Y, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(runTime, pointB_X, PROFILE_GRAPH_H);

	sprite->drawFastHLine(pointA_X, gridBottom - ((gridBottom - pointA_Y) / 2), pointB_X - pointA_X, GRID_LINE_COLOR);
	sprite->drawNumber(_profile.pre_heat_soak_time, pointA_X + ((pointB_X - pointA_X) / 2) , gridBottom - (((gridBottom - pointA_Y) / 2)));


	//Ramp to reflow
	runTime += _profile.reflow_ramp_time;
	int pointC_X = round((double)GRID_X + ((double)runTime / (double)_maxTime) * (double)GRID_W);
	int pointC_Y = round((gridBottom - ((double)_profile.reflow_target_temperature / (double)_maxTemp) * (double)GRID_H));

	sprite->drawFastHLine(GRID_X, pointC_Y, pointC_X - GRID_X, GRID_LINE_COLOR);
	sprite->drawFastVLine(pointC_X, pointC_Y, gridBottom - pointC_Y, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(runTime, pointC_X, PROFILE_GRAPH_H);
	sprite->setTextDatum(TR_DATUM);
	sprite->drawNumber(_profile.reflow_target_temperature, TEXT_WIDTH, pointC_Y);
	sprite->setTextDatum(TC_DATUM);

	sprite->drawString("Ramp to reflow", pointC_X/2, pointC_Y + 1);

	sprite->drawFastHLine(pointB_X, gridBottom - ((gridBottom - pointB_Y) / 2), pointC_X - pointB_X, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(_profile.reflow_ramp_time, pointB_X + ((pointC_X - pointB_X) / 2), gridBottom - (((gridBottom - pointB_Y) / 2)));

	//Reflow
	runTime += _profile.reflow_soak_time;
	int pointD_X = round((double)GRID_X + ((double)runTime / (double)_maxTime) * (double)GRID_W);

	sprite->drawFastVLine(pointD_X, pointC_Y, gridBottom - pointC_Y, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(runTime, pointD_X, PROFILE_GRAPH_H);

	sprite->drawFastHLine(pointC_X, gridBottom - ((gridBottom - pointC_Y) / 2), pointD_X - pointC_X, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(_profile.reflow_soak_time , pointC_X + ((pointD_X - pointC_X) / 2), gridBottom - ((gridBottom - pointC_Y) / 2));
	sprite->setTextDatum(TC_DATUM);
	sprite->drawString("Reflow", pointC_X + ((pointD_X - pointC_X) / 2), gridBottom - ((gridBottom - pointC_Y) / 2) + 1);

	//Cooling stage 1
	sprite->drawFastHLine(pointD_X, gridBottom - ((gridBottom - pointA_Y) / 2), (GRID_X + GRID_W) - pointD_X, GRID_LINE_COLOR);
	sprite->setTextDatum(BC_DATUM);
	sprite->drawNumber(_profile.reflow_soak_time, pointD_X + (((GRID_X + GRID_W) - pointD_X) / 2), gridBottom - ((gridBottom - pointA_Y) / 2));

	sprite->drawLine(GRID_X, gridBottom, pointA_X, pointA_Y, TFT_RED);
	sprite->drawLine(pointA_X, pointA_Y, pointB_X, pointB_Y, TFT_RED);
	sprite->drawLine(pointB_X, pointB_Y, pointC_X, pointC_Y, TFT_RED);
	sprite->drawLine(pointC_X, pointC_Y, pointD_X, pointC_Y, TFT_RED);
	sprite->drawLine(pointD_X, pointC_Y - 2, GRID_X + GRID_W - 1, pointA_Y, TFT_BLUE);
}