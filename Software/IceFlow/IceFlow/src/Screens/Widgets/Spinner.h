#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "TextBoxDto.h"

#define DFAULT_COLOR_1 0xED7F
#define DFAULT_COLOR_2 0xD4DB
#define DFAULT_COLOR_3 0xBC38
#define DFAULT_COLOR_4 0xABD6
#define DFAULT_COLOR_5 0x9374
#define DFAULT_COLOR_6 0x8332
#define DFAULT_COLOR_7 0x72AF
#define DFAULT_COLOR_8 0x624D

#define SPINNER_SIDE_LENGTH 60

class Spinner
{
private:
	TFT_eSPI* _tft = nullptr;

	int _x = 0;
	int _y = 0;

	unsigned long _nextUpdate = 0;
	int _updateScreenInterval = 15;
	int _circleLoopCounter = 8;
	int _secondaryLoopCounter = 0;
	uint32_t _col[8];

	void UpdateCircle();

public:
	Spinner(TFT_eSPI* tft, int x_, int y_);
	void SetColors(uint32_t colors[8]);
	void Draw();
	void Update();
};

