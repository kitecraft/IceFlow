#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "PopUpMenuDto.h"

//NOT to be used by anyone.
//Used only by the PopUpMenu class
class MenuOption {
private:
	String _name = "";
	PopUpMenuDto _config;
public:
	MenuOption();
	MenuOption(String name_, PopUpMenuDto config_);
	MenuOption(const MenuOption& orig);
	void Draw(TFT_eSprite* sprite);
	bool Touched(int x, int y);
	String GetName() { return _name; }
};