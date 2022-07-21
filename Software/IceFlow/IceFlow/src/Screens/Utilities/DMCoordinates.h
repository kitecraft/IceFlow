#pragma once
#include <Arduino.h>

class DMCoordinates {
public:
	int x;
	int y;
	int w;
	int h;
	int p_x;
	int p_y;

	DMCoordinates() {
		x = 0;
		y = 0;
		w = 0;
		h = 0;
		p_x = 0;
		p_y = 0;
	} 
	DMCoordinates(int x_, int y_, int w_, int h_, int p_x_ = 0, int p_y_ = 0) {
		x = x_;
		y = y_;
		w = w_;
		h = h_;
		p_x = p_x_;
		p_y = p_y_;
	}
	DMCoordinates(const DMCoordinates& orig) {
		x = orig.x;
		y = orig.y;
		w = orig.w;
		h = orig.h;
		p_x = orig.p_x;
		p_y = orig.p_y;
	}
	void Print() {
		Serial.printf("x: %i, y: %i, w: %i, h: %i, p_x: %i, p_y: %i\n", x, y, w, h, p_x, p_y);
	}
};
