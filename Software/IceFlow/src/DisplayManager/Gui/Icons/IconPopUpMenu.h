#pragma once
#include <TFT_eSPI.h>
#include "../../../IceFlow_Config.h"
#include "../../Utilities/StarsideCoordinates.h"
#include "../../Utilities/StarsideTheme.h"


#define MENU_FONT AA_FONT_14PT
#define MENU_FONT_HEIGHT 14
#define SELECTED_ITEM_TEXT_COLOR TFT_GREEN


#define MENU_AREA_MARGIN 6
#define TEXT_HORIZONTAL_BUFFER 9
#define TEXT_VERTICAL_BUFFER 6
#define TEXT_VERTICAL_SPACEING 3


class MenuOption
{
public:
	String name;
	StarsideCoordinates coordinates;
};

class IconPopUpMenu
{
private:
	TFT_eSPI* TFT;
	TFT_eSprite* sprite = NULL;
	//uint16_t* screenReadBuffer = NULL;

	StarsideCoordinates coordinates;
	bool menuIsOpen = false;
	int longestStringLength = 0;
	
	MenuOption* options = NULL;
	int numOptions = 0;
	
	void CleanUp();
	bool clean = true;

	void DrawPillBoxWithDropShadow(StarsideCoordinates coordinates);
	void DrawRoundedBoxWithDropShadow(StarsideCoordinates coordinates, int radius);

public:
	~IconPopUpMenu();
	void Init(TFT_eSPI* inTFT);

	void Create(StarsideCoordinates inCoordinates, String* menuOptions, int numMenuOptions);
	void Open(String selected = "");
	void Close();

	bool IsMenuOpen() { return menuIsOpen; }
	String Touched(int x, int y);
};

