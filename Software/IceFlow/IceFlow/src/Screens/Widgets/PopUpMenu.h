#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "PopUpMenuDto.h"

struct MenuOption {
	String name = "";
	PopUpMenuDto config;
	MenuOption() {}
	MenuOption(String name_, PopUpMenuDto config_) {
		name = name_;
		config = config_;
	}
	MenuOption(const MenuOption& orig) {
		name = orig.name;
		config = orig.config;
	}
};

class PopUpMenu
{
private:
	TFT_eSPI* _tft = nullptr;;
	PopUpMenuDto _config;
	MenuOption* _menuOptions = nullptr;
	uint16_t* _screenReadBuffer = nullptr;

	void* (*pop_malloc)(size_t size) = nullptr;

	int _numOptions = 0;
	int _lengthOfLongestOptionName = 0;
	bool _open = false;
	bool _opening = false;
	bool _closing = false;

	void Configure(PopUpMenuDto configDto, String* menuOptions, int numMenuOptions, TFT_eSPI* tft);
	void Clear();
public:
	PopUpMenu();
	~PopUpMenu();
	
	// Opens the menu with the supplied options
	// Will use PSRAM if available
	void Open(PopUpMenuDto configDto, String* menuOptions, int numMenuOptions, TFT_eSPI* tft);
	
	// Closes the menu
	// The destructor does not close the menu if it's open
	void Close();
	
	// Check if touched
	String Touched(int x, int y);
	
	//Returns true if the menu is open
	bool isOpen() { return _open; }

	bool Opening() { return _opening; }
	bool Closing() { return _closing; }
};

