#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>

#define WIFI_SYMBOL_WIDTH 23
#define WIFI_SYMBOL_HEIGHT 20
#define WIFI_SYMBOL_STA_COLOR TFT_GREEN
#define WIFI_SYMBOL_AP_COLOR TFT_YELLOW

enum WiFiStatus {
	WiFiStatus_Off = 0,
	WiFiStatus_AP,
	WiFiStatus_STA,
};

class WiFiSymbol
{
private:
	static void FillArc(TFT_eSprite* sprite, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour);

public:
	WiFiSymbol();
	~WiFiSymbol();
	static void Draw(int x, int y, WiFiStatus status, uint16_t bgColor, TFT_eSPI* tft);
};

