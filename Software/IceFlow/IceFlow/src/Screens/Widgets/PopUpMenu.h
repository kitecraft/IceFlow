#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "PopUpMenuDto.h"
#include "MenuOption.h"

#define OPTION_BORDER_PADDING 5
#define OPTION_SPACING 3
#define OPTION_BORDER_SIZE 3
#define OPTION_TEXT_V_PADDING 2

class PopUpMenu
{
private:
	TFT_eSPI* _tft = nullptr;;
	TFT_eSprite* _sprite = nullptr;

	PopUpMenuDto _config;
	MenuOption* _menuOptions = nullptr;
	uint16_t* _screenReadBuffer = nullptr;

	int _numOptions = 0;
	int _lengthOfLongestOptionText = 0;
	bool _open = false;

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
	bool Touched(int x, int y, String& menuString);
	
	void Redraw();

	//Returns true if the menu is open
	bool isOpen() { return _open; }
};

