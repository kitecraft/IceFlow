#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMCoordinates.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"
#include "GraphAutoScaler.h"


#define UPPER_TEMPERATURE_DRAW_BUFFER 20
#define LOWER_TEMPERATURE_DRAW_BUFFER 10

#define TEMPERATURE_REDRAW_LIMT 5

#define TEMPERATURE_LEGEND_WIDTH 22

//#define GP_TEXT_WIDTH 17
#define GP_TEXT_HEIGHT 10


class MS_GraphPanel
{
private:
	TFT_eSPI* _tft;
	DMCoordinates _coordinates;
	//Profile* _profile;
	int _temperatureLegendHeight;
	int _timeLegendWidth;
	bool _ignoreSecondary = false;
	bool _ignoreProfile = false;
	GraphAutoScaler* _primaryTemperatureAutoScaler = nullptr;
	float _maximumTemperature = UPPER_TEMPERATURE_DRAW_BUFFER;
	float _minimumTemperature = LOWER_TEMPERATURE_DRAW_BUFFER;


	void DrawTemperatureLegends();
	void DrawTimeLegend();

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

