#include "MS_GraphPanel.h"

MS_GraphPanel::MS_GraphPanel()
{
}

MS_GraphPanel::MS_GraphPanel(TFT_eSPI* tft, DMCoordinates coordinates)
{
	_tft = tft;
	_coordinates = coordinates;
	_temperatureLegendHeight = _coordinates.h;
	_timeLegendWidth = _coordinates.w - (TEMPERATURE_LEGEND_WIDTH * 2);
	_primaryTemperatureAutoScaler = new GraphAutoScaler(_timeLegendWidth - 2);
}

MS_GraphPanel::~MS_GraphPanel()
{
	if (_primaryTemperatureAutoScaler != nullptr) {
		delete _primaryTemperatureAutoScaler;
	}
}

//void MS_GraphPanel::Draw(Profile* profile)
void MS_GraphPanel::Draw()
{
	_maximumTemperature = 10 + UPPER_TEMPERATURE_DRAW_BUFFER;
	_minimumTemperature = LOWER_TEMPERATURE_DRAW_BUFFER;

	DrawTemperatureLegends();
	DrawTimeLegend();
}

void MS_GraphPanel::Update(float primaryTemperature, float secondaryTemperature, float profileTemperate)
{
	//Serial.printf("Received:  prim: %f  sec: %f  prof: %f\n", primaryTemperature, secondaryTemperature, profileTemperate);
	_primaryTemperatureAutoScaler->AddItem(primaryTemperature);

	float maxTemp = _primaryTemperatureAutoScaler->GetMax();
	float minTemp = _primaryTemperatureAutoScaler->GetMin();

	//Serial.printf("Actuals:  Max: %f  Min: %f\n", _maximumTemperature, _minimumTemperature);
	//Serial.printf("New:  Max: %f  Min: %f\n", maxTemp, minTemp);
	//Serial.println();

	if (maxTemp > _maximumTemperature - TEMPERATURE_REDRAW_LIMT ||
		minTemp < _minimumTemperature + TEMPERATURE_REDRAW_LIMT) {
		_maximumTemperature = maxTemp + UPPER_TEMPERATURE_DRAW_BUFFER;
		_minimumTemperature = minTemp - LOWER_TEMPERATURE_DRAW_BUFFER;
		DrawTemperatureLegends();
	}
}

void MS_GraphPanel::DrawTemperatureLegends()
{
	Serial.printf("_minimumTemperature = %f", _minimumTemperature);
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight);
	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(GlobalTheme.textColor, TFT_BLACK);

	//Left side
	sprite.fillSprite(TFT_BLACK);
	sprite.setTextDatum(TR_DATUM);
	sprite.drawFloat(_maximumTemperature, 1, TEMPERATURE_LEGEND_WIDTH, 0);

	sprite.setTextDatum(MR_DATUM);
	sprite.drawFloat(_minimumTemperature, 1, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight - GP_TEXT_HEIGHT);
	float temperatureSegment = (_maximumTemperature - _minimumTemperature) / 8.0;
	float pixleSegment = ((float)_temperatureLegendHeight - (float)GP_TEXT_HEIGHT) / 8.0;
	for (float i = 1; i < 8; i++) {
		sprite.drawFloat(_minimumTemperature + (temperatureSegment * i), 1, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight - GP_TEXT_HEIGHT - (round(pixleSegment * i)));
	}

	uint16_t* buffer = (uint16_t*)ps_malloc(2*(TEMPERATURE_LEGEND_WIDTH * _temperatureLegendHeight) + 1);
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight, sprPtr, buffer);


	//Right side
	sprite.fillSprite(TFT_BLACK);
	sprite.setTextDatum(TL_DATUM);
	sprite.drawFloat(_maximumTemperature, 1, 0, 0);

	sprite.setTextDatum(ML_DATUM);
	sprite.drawFloat(_minimumTemperature, 1, 0, _temperatureLegendHeight - GP_TEXT_HEIGHT);
	for (float i = 1; i < 8; i++) {
		sprite.drawFloat(_minimumTemperature + (temperatureSegment * i), 1, 0, _temperatureLegendHeight - GP_TEXT_HEIGHT - (round(pixleSegment * i)));
	}
	_tft->pushImageDMA(_coordinates.p_x + (_coordinates.w - TEMPERATURE_LEGEND_WIDTH), _coordinates.p_y, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight, sprPtr);

	free(buffer);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void MS_GraphPanel::DrawTimeLegend()
{

}