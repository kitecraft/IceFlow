#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/Screens_Config.h"
#include "../Utilities/DMTheme.h"
#include "../../ProfileManager/ProfileManager.h"

#define PROFILE_GRAPH_X 0
#define PROFILE_GRAPH_Y 35
#define PROFILE_GRAPH_W 239

//this is half actual height and is used for the bottom sprite.  //25,812 
#define PROFILE_GRAPH_SPRITE_H 101 

//top sprite needs to be taller to accomadte text writen across the middle of the graph
#define PROFILE_GRAPH_SPRITE_TOP_H 106 

//The actual height of the graph is this
#define PROFILE_GRAPH_H (PROFILE_GRAPH_SPRITE_H * 2)

#define TEXT_WIDTH 17
#define TEXT_HEIGHT 10

#define GRID_X (TEXT_WIDTH + 1)
#define GRID_Y 0
#define GRID_W (PROFILE_GRAPH_W - (TEXT_WIDTH*2) - 2)
#define GRID_H (PROFILE_GRAPH_H - TEXT_HEIGHT - 1)
#define GRID_H_SPACING 20
#define GRID_V_SPACING 24
#define GRID_LINE_COLOR 0xe73c


#define TOP_SIDE 0
#define BOTTOM_SIDE 1
#define BASE_TEMPERATURE 10

class PS_ProfileGraphPanel
{
private:
	TFT_eSPI* _tft;
	Profile _profile;
	bool _showGrid = true;

	int _maxTime = 0;
	int _maxTemp = 0;

	void CalculateMaxTimeAndTemp();
	void DrawSprite(TFT_eSprite *sprite);
public:
	PS_ProfileGraphPanel();
	PS_ProfileGraphPanel(TFT_eSPI* tft);
	~PS_ProfileGraphPanel();
	void Draw(String profileFileName);
	void ReDraw();
	void ShowGrid(bool showGrid) { _showGrid = showGrid; }
	bool GridVisible() { return _showGrid; }
};

