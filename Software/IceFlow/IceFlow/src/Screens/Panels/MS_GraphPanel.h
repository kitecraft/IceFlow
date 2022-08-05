#pragma once
#include <Arduino.h>
#include "../../IceFlow_Config.h"
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"
#include "GraphAutoScaler.h"


#define UPPER_TEMPERATURE_DRAW_BUFFER 20
#define LOWER_TEMPERATURE_DRAW_BUFFER 10

#define TEMPERATURE_REDRAW_LIMT 5

#define TEMPERATURE_LEGEND_WIDTH 23
#define TEMPERATURE_LEGEND_TEXT_COLOR 0x2f3e

//#define GP_TEXT_WIDTH 17
#define TIME_LEGEND_HEIGHT 10
#define TIME_LEGEND_TEXT_COLOR 0x919f


class MS_GraphPanel
{
private:
	TFT_eSPI* _tft;
	DMCoordinates _coordinates;

	int _bgSprite_X = 0;
	int _bgSpriteTop_Y = 0;
	int _bgSpriteBottom_Y = 0;
	int _bgSprite_W = 0;
	int _bgSprite_H = 0;
	int _bgSpriteTop_H = 0;
	int _bgSpriteBottom_H = 0;
	TFT_eSprite* _bgSprite[2] = { nullptr };
	uint16_t* _bgSprPtr[2] = { nullptr };

	TFT_eSprite* _temperatureSprite[2] = { nullptr };
	//uint16_t* _temperatureSprPtr[2] = { nullptr };
	int _temperatureSprite_W = 0;
	int _temperatureSprite_H = 0;
	int _temperatureSpriteTop_H = 0;
	int _temperatureSpriteBottom_H = 0;

	float _currentScaleDegreesPerPixel;

	int _temperatureLegendHeight;

	int _timeLegend_W = 0;
	int _timeLegend_X = 0;
	int _timeLegend_Y = 0;
	bool _ignoreSecondary = false;
	bool _ignoreProfile = false;

	GraphAutoScaler* _primaryTemperatureAutoScaler = nullptr;
	float _maximumTemperature = UPPER_TEMPERATURE_DRAW_BUFFER;
	float _minimumTemperature = LOWER_TEMPERATURE_DRAW_BUFFER;


	void DrawTemperatureLegends();
	void DrawTimeLegend();
	void DrawGraph();
	void DrawGraphGrid(int slot);

	void ReDrawTemperatureSprites();

public:
	MS_GraphPanel();
	MS_GraphPanel(TFT_eSPI* tft, DMCoordinates coordinates);
	~MS_GraphPanel();
	void IgnoreSecondary(bool ignore) { _ignoreSecondary = ignore; }
	void IgnoreProfile(bool ignore) { _ignoreProfile = ignore; }
	//void Draw(Profile* profile);
	void Draw();
	void Update(float primaryTemperature, float secondaryTemperature = 0, float profileTemperate = 0);
};

