#include "WiFiSymbol.h"


WiFiSymbol::WiFiSymbol()
{

}

WiFiSymbol::~WiFiSymbol()
{

}

void WiFiSymbol::Draw(int x, int y, WiFiStatus status, uint16_t bgColor, TFT_eSPI* tft)
{
	uint16_t iconColor;
	int midPoint = WIFI_SYMBOL_WIDTH / 2;
	TFT_eSprite sprite(tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(WIFI_SYMBOL_WIDTH, WIFI_SYMBOL_HEIGHT);
	
	switch (status) {
	case WiFiStatus_AP:
		iconColor = WIFI_SYMBOL_AP_COLOR;
		break;
	case WiFiStatus_STA:
		iconColor = WIFI_SYMBOL_STA_COLOR;
		break;
	default:
		iconColor = TFT_WHITE;
		break;
	}

	sprite.fillSprite(bgColor);
	sprite.drawSpot(midPoint, WIFI_SYMBOL_HEIGHT - 3, 2, iconColor, bgColor);
	FillArc(&sprite, midPoint, WIFI_SYMBOL_HEIGHT - 3, 327, 11, 6, 6, 1, iconColor);
	FillArc(&sprite, midPoint, WIFI_SYMBOL_HEIGHT - 3, 321, 13, 11, 11, 1, iconColor);
	FillArc(&sprite, midPoint, WIFI_SYMBOL_HEIGHT - 3, 315, 15, 16, 16, 1, iconColor);

	tft->pushImageDMA(x, y, WIFI_SYMBOL_WIDTH, WIFI_SYMBOL_HEIGHT, sprPtr);
	tft->dmaWait();
	sprite.deleteSprite();
}

// x,y == coords of centre of arc
// start_angle = 0 - 359
// seg_count = number of 6 degree segments to draw (60 => 360 degree arc)
// rx = x axis outer radius
// ry = y axis outer radius
// w  = width (thickness) of arc in pixels
// colour = 16 bit colour value
// Note if rx and ry are the same then an arc of a circle is drawn
#define DEG2RAD 0.0174532925
void WiFiSymbol::FillArc(TFT_eSprite* sprite, int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

	uint8_t seg = 6; // Segments are 3 degrees wide = 120 segments for 360 degrees
	uint8_t inc = 6; // Draw segments every 3 degrees, increase to 6 for segmented ring

	// Calculate first pair of coordinates for segment start
	float sx = cos((start_angle - 90) * DEG2RAD);
	float sy = sin((start_angle - 90) * DEG2RAD);
	uint16_t x0 = sx * (rx - w) + x;
	uint16_t y0 = sy * (ry - w) + y;
	uint16_t x1 = sx * rx + x;
	uint16_t y1 = sy * ry + y;

	// Draw colour blocks every inc degrees
	for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {

		// Calculate pair of coordinates for segment end
		float sx2 = cos((i + seg - 90) * DEG2RAD);
		float sy2 = sin((i + seg - 90) * DEG2RAD);
		int x2 = sx2 * (rx - w) + x;
		int y2 = sy2 * (ry - w) + y;
		int x3 = sx2 * rx + x;
		int y3 = sy2 * ry + y;

		sprite->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
		sprite->fillTriangle(x1, y1, x2, y2, x3, y3, colour);

		// Copy segment end to sgement start for next segment
		x0 = x2;
		y0 = y2;
		x1 = x3;
		y1 = y3;
	}
}