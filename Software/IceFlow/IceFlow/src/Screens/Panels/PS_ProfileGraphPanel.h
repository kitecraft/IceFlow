#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"

#define PROFILE_GRAPH_X 0
#define PROFILE_GRAPH_Y 35
#define PROFILE_GRAPH_W 239
#define PROFILE_GRAPH_H 101 //this is half actual height, 1/2 for each sprite.  //25,812 

#define TOP_SIDE 0
#define BOTTOM_SIDE 1
#define BASE_TEMPERATURE 20
class PS_ProfileGraphPanel
{
private:
	TFT_eSPI* _tft;
	Profile _profile;

	int _maxTime = 0;
	int _maxTemp = 0;

	void CalculateMaxTimeAndTemp();
	void DrawSprite(TFT_eSprite *sprite);

public:
	PS_ProfileGraphPanel();
	PS_ProfileGraphPanel(TFT_eSPI* tft);
	~PS_ProfileGraphPanel();
	void Draw(String profileFileName);

};

