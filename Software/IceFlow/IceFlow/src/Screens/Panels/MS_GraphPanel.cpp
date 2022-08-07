#include "MS_GraphPanel.h"

MS_GraphPanel::MS_GraphPanel()
{
}

MS_GraphPanel::MS_GraphPanel(TFT_eSPI* tft, DMCoordinates coordinates, bool ignoreSecondary)
{
	_tft = tft;
	_coordinates = coordinates;
	_ignoreSecondary = ignoreSecondary;

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

}

MS_GraphPanel::~MS_GraphPanel()
{
	if (_primaryTemperatureAutoScaler != nullptr) {
		delete _primaryTemperatureAutoScaler;
	}
	if (_secondaryTemperatureAutoScaler != nullptr) {
		delete _secondaryTemperatureAutoScaler;
	}

	if (_bgSprite[TOP_SIDE] != nullptr) {
		_bgSprite[TOP_SIDE]->deleteSprite();
		delete _bgSprite[TOP_SIDE];
	}
	if (_bgSprite[BOTTOM_SIDE] != nullptr) {
		_bgSprite[BOTTOM_SIDE]->deleteSprite();
		delete _bgSprite[BOTTOM_SIDE];
	}

	if (_temperatureSprite[TOP_SIDE] != nullptr) {
		_temperatureSprite[TOP_SIDE]->deleteSprite();
		delete _temperatureSprite[TOP_SIDE];
	}
	if (_temperatureSprite[BOTTOM_SIDE] != nullptr) {
		_temperatureSprite[TOP_SIDE]->deleteSprite();
		delete _temperatureSprite[BOTTOM_SIDE];
	}
}

void MS_GraphPanel::Draw()
{
	_maximumTemperature = 10 + TEMPERATURE_DRAW_BUFFER_LARGE;
	_minimumTemperature = TEMPERATURE_DRAW_BUFFER_SMALL;

	DrawTemperatureLegends();
	DrawTimeLegend();
	DrawGraph();
}

void MS_GraphPanel::ReDraw()
{
	ReDrawTemperatureSprites();
	DrawTemperatureLegends();
	DrawTimeLegend();
}

void MS_GraphPanel::Update(float primaryTemperature, float secondaryTemperature, float profileTemperate)
{
	float primMax, primMin;
	bool primMaxRedraw = false;
	bool primMinRedraw = false; 
	bool secMaxRedraw = false;
	bool SecMinRedraw = false;

	bool temperatureLegendRedrawRequired = false;
	bool secondaryRecalculated = false;

	bool primaryRecalculated = _primaryTemperatureAutoScaler->AddItem(primaryTemperature);
	float origMax = _maximumTemperature;
	float origMin = _minimumTemperature;


	if (primaryRecalculated)
	{
		primMax = _primaryTemperatureAutoScaler->GetMax();
		primMin = _primaryTemperatureAutoScaler->GetMin();

		if (primMax > _maximumTemperature - TEMPERATURE_DRAW_BUFFER_SMALL) {
			_maximumTemperature = primMax + TEMPERATURE_DRAW_BUFFER_LARGE;
			primMaxRedraw = true;
		} else if (primMax < _maximumTemperature - TEMPERATURE_DRAW_BUFFER_LARGE) {
			_maximumTemperature = primMax + TEMPERATURE_DRAW_BUFFER_SMALL;
			primMaxRedraw = true;
		}

		if (primMin < _minimumTemperature + TEMPERATURE_DRAW_BUFFER_SMALL) {
			_minimumTemperature = primMin - TEMPERATURE_DRAW_BUFFER_LARGE;
			primMinRedraw = true;
		}
		else if (primMin > _minimumTemperature + TEMPERATURE_DRAW_BUFFER_LARGE) {
			_minimumTemperature = primMin - TEMPERATURE_DRAW_BUFFER_SMALL;
			primMinRedraw = true;
		}
		temperatureLegendRedrawRequired = primMaxRedraw || primMinRedraw ? true : false;
	}

	if (!_ignoreSecondary) {
		secondaryRecalculated = _secondaryTemperatureAutoScaler->AddItem(secondaryTemperature);
		if (secondaryRecalculated || temperatureLegendRedrawRequired) {
			float secMax = _secondaryTemperatureAutoScaler->GetMax();
			float secMin = _secondaryTemperatureAutoScaler->GetMin();

			if (secMax > primMax) {
				_maximumTemperature = origMax;
				primMaxRedraw = false;
				if (secMax > _maximumTemperature - TEMPERATURE_DRAW_BUFFER_SMALL) {
					_maximumTemperature = secMax + TEMPERATURE_DRAW_BUFFER_LARGE;
					secMaxRedraw = true;
				}
				else if (secMax < _maximumTemperature - TEMPERATURE_DRAW_BUFFER_LARGE) {
					_maximumTemperature = secMax + TEMPERATURE_DRAW_BUFFER_SMALL;
					secMaxRedraw = true;
				}
			}
			if (secMin < primMin) {
				_minimumTemperature = origMin;
				primMinRedraw = false;
				if (secMin < _minimumTemperature + TEMPERATURE_DRAW_BUFFER_SMALL) {
					_minimumTemperature = secMin - TEMPERATURE_DRAW_BUFFER_LARGE;
					SecMinRedraw = true;
				}
				else if (secMin > _minimumTemperature + TEMPERATURE_DRAW_BUFFER_LARGE) {
					_minimumTemperature = secMin - TEMPERATURE_DRAW_BUFFER_SMALL;
					SecMinRedraw = true;
				}
			}

			//temperatureLegendRedrawRequired = primMaxRedraw || primMinRedraw ? true : false;
			temperatureLegendRedrawRequired = primMaxRedraw || primMinRedraw || secMaxRedraw || SecMinRedraw ? true : false;
		}
	}

	
	if (temperatureLegendRedrawRequired) {
		if (_minimumTemperature < 0) {
			_minimumTemperature = 0;
		}
		_currentScaleDegreesPerPixel = _temperatureSprite_H / (_maximumTemperature - _minimumTemperature);
		DrawTemperatureLegends();
		ReDrawTemperatureSprites();
	}
	else {
		_temperatureSprite[TOP_SIDE]->scroll(-1, 0);
		_temperatureSprite[BOTTOM_SIDE]->scroll(-1, 0);
		_temperatureSprite[TOP_SIDE]->drawFastVLine(_temperatureSprite_W - 1, 0, _temperatureSprite_H, TFT_TRANSPARENT);
		_temperatureSprite[BOTTOM_SIDE]->drawFastVLine(_temperatureSprite_W - 1, 0, _temperatureSprite_H, TFT_TRANSPARENT);

		if (!_ignoreSecondary) {
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

void MS_GraphPanel::UpdateValuesOnly(float primaryTemperature, float secondaryTemperature, float profileTemperate)
{
	_primaryTemperatureAutoScaler->AddItem(primaryTemperature);
	if (!_ignoreSecondary) {
		_secondaryTemperatureAutoScaler->AddItem(secondaryTemperature);
	}
}

void MS_GraphPanel::DrawTemperatureLegends()
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
	//_tft->pushImageDMA(_coordinates.p_x, _coordinates.p_y, TEMPERATURE_LEGEND_WIDTH, _temperatureLegendHeight, sprPtr);

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

void MS_GraphPanel::DrawTimeLegend()
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

void MS_GraphPanel::DrawGraph()
{
	DrawGraphGrid(TOP_SIDE);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteTop_Y, _bgSprite_W, _bgSpriteTop_H, _bgSprPtr[TOP_SIDE]);

	DrawGraphGrid(BOTTOM_SIDE);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteBottom_Y, _bgSprite_W, _bgSpriteBottom_H, _bgSprPtr[BOTTOM_SIDE]);

	/*
	_temperatureSprite[TOP_SIDE]->fillSprite(TFT_TRANSPARENT);
	_temperatureSprite[BOTTOM_SIDE]->fillSprite(TFT_TRANSPARENT);
	_temperatureSprite[TOP_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 25, TFT_PINK);
	_temperatureSprite[TOP_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 5, TFT_TRANSPARENT);

	_temperatureSprite[TOP_SIDE]->pushToSprite(_bgSprite[TOP_SIDE], 1, 1, TFT_TRANSPARENT);

	_tft->pushImageDMA(_bgSprite_X, _bgSpriteTop_Y, _bgSprite_W, _bgSpriteTop_H, _bgSprPtr[TOP_SIDE]);



	_temperatureSprite[BOTTOM_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 25, TFT_PINK);
	_temperatureSprite[BOTTOM_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 5, TFT_TRANSPARENT);

	_temperatureSprite[BOTTOM_SIDE]->pushToSprite(_bgSprite[BOTTOM_SIDE], 1, _bgSpriteTop_H, TFT_TRANSPARENT);
	_tft->pushImageDMA(_bgSprite_X, _bgSpriteBottom_Y, _bgSprite_W, _bgSpriteBottom_H, _bgSprPtr[BOTTOM_SIDE]);

	*/

	/*
	_bgSprite[TOP_SIDE]->fillSprite(TFT_BLACK);
	_bgSprite[TOP_SIDE]->drawRect(0, 0, _bgSprite_W, _bgSprite_H - 1, GlobalTheme.panelBorderColor);

	_bgSprite[TOP_SIDE]->drawLine(0, 0, _bgSprite_W, _bgSprite_H - 1, GlobalTheme.panelBorderColor);
	_bgSprite[TOP_SIDE]->drawLine(_bgSprite_W, 0, 0, _bgSprite_H - 1, GlobalTheme.panelBorderColor);
	_temperatureSprite[TOP_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 25, TFT_PINK);
	_temperatureSprite[TOP_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 5, TFT_TRANSPARENT);
	
	_temperatureSprite[TOP_SIDE]->pushToSprite(_bgSprite[TOP_SIDE], 1, 1, TFT_TRANSPARENT);

	_tft->pushImageDMA(_bgSprite_X, _bgSpriteTop_Y, _bgSprite_W, _bgSpriteTop_H, _bgSprPtr[TOP_SIDE]);


	_bgSprite[BOTTOM_SIDE]->fillSprite(TFT_BLACK);
	_bgSprite[BOTTOM_SIDE]->drawRect(0, 0, _bgSprite_W, _bgSprite_H - 1, GlobalTheme.panelBorderColor);

	_bgSprite[BOTTOM_SIDE]->drawLine(0, 0, _bgSprite_W, _bgSprite_H - 1, GlobalTheme.panelBorderColor);
	_bgSprite[BOTTOM_SIDE]->drawLine(_bgSprite_W, 0, 0, _bgSprite_H - 1, GlobalTheme.panelBorderColor);

	_temperatureSprite[BOTTOM_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 25, TFT_PINK);
	_temperatureSprite[BOTTOM_SIDE]->fillCircle(_temperatureSprite_W / 2, _temperatureSprite_H / 2, 5, TFT_TRANSPARENT);
	
	_temperatureSprite[BOTTOM_SIDE]->pushToSprite(_bgSprite[BOTTOM_SIDE], 1, _bgSpriteTop_H, TFT_TRANSPARENT);

	_tft->pushImageDMA(_bgSprite_X, _bgSpriteBottom_Y, _bgSprite_W, _bgSpriteBottom_H, _bgSprPtr[BOTTOM_SIDE]);
	_tft->dmaWait();
	*/
}

void MS_GraphPanel::DrawGraphGrid(int slot)
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

void MS_GraphPanel::ReDrawTemperatureSprites()
{
	_temperatureSprite[TOP_SIDE]->fillSprite(TFT_TRANSPARENT);
	_temperatureSprite[BOTTOM_SIDE]->fillSprite(TFT_TRANSPARENT);

	_currentScaleDegreesPerPixel = _temperatureSprite_H / (_maximumTemperature - _minimumTemperature);


	if (!_ignoreSecondary) {
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