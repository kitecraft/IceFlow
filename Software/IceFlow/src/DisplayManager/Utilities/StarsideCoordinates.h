#pragma once
#include <Arduino.h>

struct StarsideCoordinates {
	int x;
	int y;
	int w;
	int h;

	StarsideCoordinates() {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
	}
	StarsideCoordinates(int inX, int inY, int inW, int inH) {
		x = inX;
		y = inY;
		w = inW;
		h = inH;
	}
	StarsideCoordinates(const StarsideCoordinates& orig) {
		x = orig.x;
		y = orig.y;
		w = orig.w;
		h = orig.h;
	}
	void Print() {
		Serial.printf("x: %i, y: %i, w: %i, h: %i\n", x, y, w, h);
	}
};
