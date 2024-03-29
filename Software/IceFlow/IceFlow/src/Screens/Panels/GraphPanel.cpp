#include "GraphPanel.h"

GraphPanel::GraphPanel()
{
}

GraphPanel::GraphPanel(TFT_eSPI* tft, DMCoordinates coordinates, bool ignoreSecondary, bool ignoreTertiary)
{
	_tft = tft;
	_coordinates = coordinates;
	_ignoreSecondary = ignoreSecondary;
	_ignoreTertiary = ignoreTertiary;
	_primaryTemperatureAutoScaler = nullptr;
	_secondaryTemperatureAutoScaler = nullptr;
	_tertiaryTemperatureAutoScaler = nullptr;

	_temperatureLegendHeight = _coordinates.h;
	_timeLegend_W = _coordinates.w - (TEMPERATURE_LEGEND_WIDTH * 2);
	_timeLegend_X = TEMPERATURE_LEGEND_WIDTH + _coordinates.p_x;
	_timeLegend_Y = _coordinates.p_y + _coordinates.h - TIME_LEGEND_HEIGHT;


	//Background sprite
	_bgSprite_X = _coordinates.p_x + TEMPERATURE_LEGEND_WIDTH;
	_bgSprite_W = _coordinates.w - (TEMPERATURE_LEGEND_WIDTH * 2);
	_bgSprite_H = _coordinates.h - TIME_LEGEND_HEIGHT;

	_bgSpriteTop_H = _bgSprite_H / 2;
	_bgSpriteBottom_H = _bgSprite_H - _bgSpriteTop_H;
	
	_bgSpriteTop_Y = _coordinates.p_y;
	_bgSpriteBottom_Y = _bgSpriteTop_Y + _bgSpriteTop_H;

	_bgSprite[TOP_SIDE] = new TFT_eSprite(_tft);
	_bgSprite[BOTTOM_SIDE] = new TFT_eSprite(_tft);
	
	_bgSprPtr[TOP_SIDE] = (uint16_t*)_bgSprite[TOP_SIDE]->createSprite(_bgSprite_W, _bgSpriteTop_H);
	_bgSprPtr[BOTTOM_SIDE] = (uint16_t*)_bgSprite[BOTTOM_SIDE]->createSprite(_bgSprite_W, _bgSpriteBottom_H);
	_bgSprite[BOTTOM_SIDE]->setViewport(0, -_bgSpriteTop_H, _bgSprite_W, _bgSprite_H);

	//The temperature graphing sprites.
	_temperatureSprite_W = _bgSprite_W - 2;
	_temperatureSprite_H = _bgSprite_H - 2;
	_temperatureSpriteTop_H = _bgSpriteTop_H - 1;
	_temperatureSpriteBottom_H = _bgSpriteBottom_H;

	_temperatureSprite[TOP_SIDE] = new TFT_eSprite(_tft);
	_temperatureSprite[BOTTOM_SIDE] = new TFT_eSprite(_tft);

	_temperatureSprite[TOP_SIDE]->createSprite(_temperatureSprite_W, _temperatureSpriteTop_H);
	_temperatureSprite[BOTTOM_SIDE]->createSprite(_temperatureSprite_W, _temperatureSpriteBottom_H);

	_temperatureSprite[BOTTOM_SIDE]->setViewport(0, -_temperatureSpriteTop_H, _temperatureSprite_W, _temperatureSprite_H);

	_temperatureSprite[TOP_SIDE]->fillSprite(TFT_TRANSPARENT);
	_temperatureSprite[BOTTOM_SIDE]->fillSprite(TFT_TRANSPARENT);


	_primaryTemperatureAutoScaler = new GraphAutoScaler(_temperatureSprite_W);
	_secondaryTemperatureAutoScaler = new GraphAutoScaler(_temperatureSprite_W);
	_tertiaryTemperatureAutoScaler = new GraphAutoScaler(_temperatureSprite_W);
}

GraphPanel::~GraphPanel()
{
	if (_primaryTemperatureAutoScaler != nullptr) {
		delete _primaryTemperatureAutoScaler;
	}
	if (_secondaryTemperatureAutoScaler != nullptr) {
		delete _secondaryTemperatureAutoScaler;
	}
	if (_tertiaryTemperatureAutoScaler != nullptr) {
		delete _tertiaryTemperatureAutoScaler;
	}

	_bgSprite[TOP_SIDE]->deleteSprite();
	delete _bgSprite[TOP_SIDE];

	
	_bgSprite[BOTTOM_SIDE]->deleteSprite();
	delete _bgSprite[BOTTOM_SIDE];

	_temperatureSprite[TOP_SIDE]->deleteSprite();
	delete _temperatureSprite[TOP_SIDE];

	
	_temperatureSprite[TOP_SIDE]->deleteSprite();
	delete _temperatureSprite[BOTTOM_SIDE];
}

void GraphPanel::IgnoreSecondary(bool ignore)
{ 
	_ignoreSecondary = ignore; 
	if (_ignoreSecondary) {
		_secondaryTemperatureAutoScaler->Clear();
	}
	else {
		SecondaryVisible(true);
	}
	_redrawRequired = true;
}

void GraphPanel::IgnoreTertiary(bool ignore)
{
	_ignoreTertiary = ignore;
	if (_ignoreTertiary) {
		_tertiaryTemperatureAutoScaler->Clear();
	}
	else {
		TertiaryVisible(true);
	}
	_redrawRequired = true;
}

void GraphPanel::CalculateNewMaxMins()
{
	float max = _primaryTemperatureAutoScaler->GetMax();
	float min = _primaryTemperatureAutoScaler->GetMin();

	if (!_ignoreSecondary && _secondaryVisible) {
		float secMax = _secondaryTemperatureAutoScaler->GetMax();
		float secMin = _secondaryTemperatureAutoScaler->GetMin();
		if (secMax > max) {
			max = secMax;
		}
		if (secMin < min) {
			min = secMin;
		}
	}
	if (!_ignoreTertiary && _tertiaryVisible) {
		float terMax = _tertiaryTemperatureAutoScaler->GetMax();
		float terMin = _tertiaryTemperatureAutoScaler->GetMin();
		if (terMax > max) {
			max = terMax;
		}
		if (terMin < min) {
			min = terMin;
		}
	}

	if (max > _maximumTemperature - TEMPERATURE_DRAW_BUFFER_SMALL) {
		_maximumTemperature = max + TEMPERATURE_DRAW_BUFFER_LARGE;
		_redrawRequired = true;
	}
	else if (max < _maximumTemperature - TEMPERATURE_DRAW_BUFFER_LARGE) {
		_maximumTemperature = max + TEMPERATURE_DRAW_BUFFER_SMALL;
		_redrawRequired = true;
	}

	if (min < _minimumTemperature + TEMPERATURE_DRAW_BUFFER_SMALL) {
		_minimumTemperature = min - TEMPERATURE_DRAW_BUFFER_LARGE;
		_redrawRequired = true;
	}
	else if (min > _minimumTemperature + TEMPERATURE_DRAW_BUFFER_LARGE) {
		_minimumTemperature = min - TEMPERATURE_DRAW_BUFFER_SMALL;
		_redrawRequired = true;
	}

	if (_minimumTemperature < 0) {
		_minimumTemperature = 0;
		_redrawRequired = true;
	}
}

void GraphPanel::Draw()
{
	_maximumTemperature = 10 + TEMPERATURE_DRAW_BUFFER_LARGE;
	_minimumTemperature = TEMPERATURE_DRAW_BUFFER_SMALL;

	DrawTemperatureLegends();
	DrawTimeLegend();
	DrawGraph();
}

void GraphPanel::ReDraw()
{
	CalculateNewMaxMins();
	ReDrawTemperatureSprites();
	DrawTemperatureLegends();
	DrawTimeLegend();
}

void GraphPanel::Update(float primaryTemperature, float secondaryTemperature, float tertiaryTemperature)
{
	bool secondaryRecalculated = false;
	bool tertiaryReclculated = false;

	bool primaryRecalculated = _primaryTemperatureAutoScaler->AddItem(primaryTemperature);

	if (!_ignoreSecondary) {
		secondaryRecalculated = _secondaryTemperatureAutoScaler->AddItem(secondaryTemperature);
		if (!_secondaryVisible) {
			secondaryRecalculated = false;
		}
	}

	if (!_ignoreTertiary) {
		tertiaryReclculated = _tertiaryTemperatureAutoScaler->AddItem(tertiaryTemperature);
		if (!_tertiaryVisible) {
			tertiaryReclculated = false;
		}
	}

	if (primaryRecalculated || secondaryRecalculated || tertiaryReclculated || _redrawRequired) {
		CalculateNewMaxMins();
	}

		
	if (_redrawRequired) {
		_currentScaleDegreesPerPixel = _temperatureSprite_H / (_maximumTemperature - _minimumTemperature);
		DrawTemperatureLegends();
		ReDrawTemperatureSprites();
		_redrawRequired = false;
	}
	else {
		_temperatureSprite[TOP_SIDE]->scroll(-1, 0);
		_temperatureSprite[BOTTOM_SIDE]->scroll(-1, 0);
		_temperatureSprite[TOP_SIDE]->drawFastVLine(_temperatureSprite_W - 1, 0, _temperatureSprite_H, TFT_TRANSPARENT);
		_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(_temperatureSprite_W - 1, 0, _temperatureSprite_H, TFT_TRANSPARENT);

		if (!_ignoreTertiary && _tertiaryVisible) {
			_temperatureSprite[TOP_SIDE]->drawFastVLine(_temperatureSprite_W - 1,
				_temperatureSprite_H - round((tertiaryTemperature - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				TER_TEMPERATURE_COLOR
			);

			_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(_temperatureSprite_W - 1,
				_temperatureSprite_H - round((tertiaryTemperature - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				TER_TEMPERATURE_COLOR
			);
		}

		if (!_ignoreSecondary && _secondaryVisible) {
			_temperatureSprite[TOP_SIDE]->drawFastVLine(_temperatureSprite_W - 1,
				_temperatureSprite_H - round((secondaryTemperature - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				SEC_TEMPERATURE_COLOR
			);

			_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(_temperatureSprite_W - 1,
				_temperatureSprite_H - round((secondaryTemperature - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				SEC_TEMPERATURE_COLOR
			);
		}

		_temperatureSprite[TOP_SIDE]->drawFastVLine(_temperatureSprite_W - 1,
			_temperatureSprite_H - round((primaryTemperature - _minimumTemperature) * _currentScaleDegreesPerPixel),
			2,
			PRIMARY_TEMPERATURE_COLOR
		);

		_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(_temperatureSprite_W - 1,
			_temperatureSprite_H - round((primaryTemperature - _minimumTemperature) * _currentScaleDegreesPerPixel),
			2,
			PRIMARY_TEMPERATURE_COLOR
		);
	}

	_temperatureSprite[TOP_SIDE]->pushToSprite(_bgSprite[TOP_SIDE], 1, 1, TFT_TRANSPARENT);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteTop_Y, _bgSprite_W, _bgSpriteTop_H, _bgSprPtr[TOP_SIDE]);

	_temperatureSprite[BOTTOM_SIDE]->pushToSprite(_bgSprite[BOTTOM_SIDE], 1, _bgSpriteTop_H, TFT_TRANSPARENT);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteBottom_Y, _bgSprite_W, _bgSpriteBottom_H, _bgSprPtr[BOTTOM_SIDE]);


	DrawGraphGrid(TOP_SIDE);

	DrawGraphGrid(BOTTOM_SIDE);
}

void GraphPanel::UpdateValuesOnly(float primaryTemperature, float secondaryTemperature, float tertiaryTemperature)
{
	_primaryTemperatureAutoScaler->AddItem(primaryTemperature);
	if (!_ignoreSecondary) {
		_secondaryTemperatureAutoScaler->AddItem(secondaryTemperature);
	}
	if (!_ignoreTertiary) {
		_tertiaryTemperatureAutoScaler->AddItem(tertiaryTemperature);
	}
}

void GraphPanel::DrawTemperatureLegends()
{
	//Serial.printf("_minimumTemperature = %f", _minimumTemperature);
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight);

	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(TEMPERATURE_LEGEND_TEXT_COLOR, TFT_BLACK);

	//Left side
	sprite.fillSprite(TFT_BLACK);
	sprite.setTextDatum(TR_DATUM);
	sprite.drawFloat(_maximumTemperature, 1, TEMPERATURE_LEGEND_WIDTH - 1, 0);

	sprite.setTextDatum(MR_DATUM);
	sprite.drawFloat(_minimumTemperature, 1, TEMPERATURE_LEGEND_WIDTH - 1, _temperatureLegendHeight - TIME_LEGEND_HEIGHT - 3);
	float temperatureSegment = (_maximumTemperature - _minimumTemperature) / 8.0;
	float pixleSegment = ((float)_temperatureLegendHeight - (float)TIME_LEGEND_HEIGHT) / 8.0;
	for (float i = 1; i < 8; i++) {
		sprite.drawFloat(_minimumTemperature + (temperatureSegment * i), 1, TEMPERATURE_LEGEND_WIDTH - 1, _temperatureLegendHeight - TIME_LEGEND_HEIGHT - (round(pixleSegment * i)));
	}

	uint16_t* buffer = (uint16_t*)ps_malloc(2*(TEMPERATURE_LEGEND_WIDTH * _temperatureLegendHeight) + 1);
	_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight, sprPtr, buffer);

	//Right side
	sprite.fillSprite(TFT_BLACK);
	sprite.setTextDatum(TL_DATUM);
	sprite.drawFloat(_maximumTemperature, 1, 1, 0);

	sprite.setTextDatum(ML_DATUM);
	sprite.drawFloat(_minimumTemperature, 1, 1, _temperatureLegendHeight - TIME_LEGEND_HEIGHT - 3);
	for (float i = 1; i < 8; i++) {
		sprite.drawFloat(_minimumTemperature + (temperatureSegment * i), 1, 1, _temperatureLegendHeight - TIME_LEGEND_HEIGHT - (round(pixleSegment * i)));
	}

	_tft->pushImageDMA(_coordinates.p_x + (_coordinates.w - TEMPERATURE_LEGEND_WIDTH), _coordinates.p_y, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight, sprPtr);

	free(buffer);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void GraphPanel::DrawTimeLegend()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(_timeLegend_W, TIME_LEGEND_HEIGHT);

	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(TIME_LEGEND_TEXT_COLOR, TFT_BLACK);

	sprite.fillSprite(TFT_BLACK);
	sprite.setTextDatum(TR_DATUM);
	sprite.drawNumber(0, _timeLegend_W, 0);

	int timeSpan = 5;
	sprite.setTextDatum(TC_DATUM);
	for (int i = 20, x = 1; i < _timeLegend_W - 10; i += 20, x++) {
		if (_timeLegend_W - i < 9) {
			sprite.setTextDatum(TL_DATUM);
		}
		sprite.drawNumber(timeSpan * x, _timeLegend_W - i, 0);
	}
	
	sprite.setTextDatum(TL_DATUM);
	sprite.drawNumber(round(_timeLegend_W * 0.25), 0, 0);

	_tft->pushImageDMA(_timeLegend_X, _timeLegend_Y, _timeLegend_W, TIME_LEGEND_HEIGHT, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void GraphPanel::DrawGraph()
{
	DrawGraphGrid(TOP_SIDE);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteTop_Y, _bgSprite_W, _bgSpriteTop_H, _bgSprPtr[TOP_SIDE]);

	DrawGraphGrid(BOTTOM_SIDE);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteBottom_Y, _bgSprite_W, _bgSpriteBottom_H, _bgSprPtr[BOTTOM_SIDE]);
}

void GraphPanel::DrawGraphGrid(int slot)
{
	_bgSprite[slot]->fillSprite(TFT_BLACK);

	_bgSprite[slot]->drawRect(0, 0, _bgSprite_W, _bgSprite_H - 1, GlobalTheme.panelBorderColor);


	for (int i = 20, x = 1; i < _timeLegend_W - 10; i += 20, x++) {
		_bgSprite[slot]->drawFastVLine(_timeLegend_W - i, 0, _bgSprite_H, GlobalTheme.panelBorderColor);
	}

	float pixleSegment = ((float)_temperatureLegendHeight - (float)TIME_LEGEND_HEIGHT) / 8.0;
	for (float i = 1; i < 8; i++) {
		_bgSprite[slot]->drawFastHLine( 0, _bgSprite_H - (round(pixleSegment * i)), _bgSprite_W,  GlobalTheme.panelBorderColor);
	}
}

void GraphPanel::ReDrawTemperatureSprites()
{
	_temperatureSprite[TOP_SIDE]->fillSprite(TFT_TRANSPARENT);
	_temperatureSprite[BOTTOM_SIDE]->fillSprite(TFT_TRANSPARENT);

	_currentScaleDegreesPerPixel = _temperatureSprite_H / (_maximumTemperature - _minimumTemperature);

	if (!_ignoreTertiary && _tertiaryVisible) {
		int column = _temperatureSprite_W - 1;
		float curr = _tertiaryTemperatureAutoScaler->GetNewest();
		if ((int)curr != GAS_NO_VALUE) {
			_temperatureSprite[TOP_SIDE]->drawFastVLine(
				column,
				_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				TER_TEMPERATURE_COLOR
			);

			_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(
				column,
				_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				TER_TEMPERATURE_COLOR
			);

			curr = _tertiaryTemperatureAutoScaler->GetNext();
			while ((int)curr != GAS_NO_VALUE) {
				column--;

				_temperatureSprite[TOP_SIDE]->drawFastVLine(
					column,
					_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
					2,
					TER_TEMPERATURE_COLOR
				);

				_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(
					column,
					_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
					2,
					TER_TEMPERATURE_COLOR
				);

				curr = _tertiaryTemperatureAutoScaler->GetNext();
			}
		}
	}
	
	if (!_ignoreSecondary && _secondaryVisible) {
		int column = _temperatureSprite_W - 1;
		float curr = _secondaryTemperatureAutoScaler->GetNewest();
		if ((int)curr != GAS_NO_VALUE) {
			_temperatureSprite[TOP_SIDE]->drawFastVLine(
				column,
				_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				SEC_TEMPERATURE_COLOR
			);

			_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(
				column,
				_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
				2,
				SEC_TEMPERATURE_COLOR
			);

			curr = _secondaryTemperatureAutoScaler->GetNext();
			while ((int)curr != GAS_NO_VALUE) {
				column--;

				_temperatureSprite[TOP_SIDE]->drawFastVLine(
					column,
					_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
					2,
					SEC_TEMPERATURE_COLOR
				);

				_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(
					column,
					_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
					2,
					SEC_TEMPERATURE_COLOR
				);

				curr = _secondaryTemperatureAutoScaler->GetNext();
			}
		}
	}

	
	int column = _temperatureSprite_W - 1;
	float curr = _primaryTemperatureAutoScaler->GetNewest();
	if ((int)curr == GAS_NO_VALUE) {
		return;
	}

	_temperatureSprite[TOP_SIDE]->drawFastVLine(
		column,
		_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
		2,
		PRIMARY_TEMPERATURE_COLOR
	);
	_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(
		column,
		_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
		2,
		PRIMARY_TEMPERATURE_COLOR
	);
	
	curr = _primaryTemperatureAutoScaler->GetNext();
	while ((int)curr != GAS_NO_VALUE) {
		column--;
		_temperatureSprite[TOP_SIDE]->drawFastVLine(
			column,
			_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
			2,
			PRIMARY_TEMPERATURE_COLOR
		);
		_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(
			column,
			_temperatureSprite_H - round((curr - _minimumTemperature) * _currentScaleDegreesPerPixel),
			2,
			PRIMARY_TEMPERATURE_COLOR
		);
		curr = _primaryTemperatureAutoScaler->GetNext();
	}
}

void GraphPanel::SecondaryVisible(bool visible)
{
	bool oldState = _secondaryVisible;
	_secondaryVisible = visible;
	if (oldState != _secondaryVisible) {
		_redrawRequired = true;
	}
}

void GraphPanel::TertiaryVisible(bool visible)
{
	bool oldState = _tertiaryVisible;
	_tertiaryVisible = visible;
	if (oldState != _tertiaryVisible) {
		_redrawRequired = true;
	}
}

void GraphPanel::ToggleSecondaryVisible()
{
	bool newState = _secondaryVisible ? false : true;
	SecondaryVisible(newState);
}

void GraphPanel::ToggleTertiaryVisible()
{
	bool newState = _tertiaryVisible ? false : true;
	TertiaryVisible(newState);
}