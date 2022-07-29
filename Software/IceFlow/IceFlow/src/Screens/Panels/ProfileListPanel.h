#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/DMTheme.h"
#include "../Utilities/Screens_Config.h"
#include "../../ProfileManager/ProfileManager.h"
#include "ProfileListItem.h"
#include "ScrollButtons.h"

#define PROFILE_FILE_LISTBOX_Y 35
#define PROFILE_FILE_LISTBOX_W 80
#define PROFILE_FILE_LISTBOX_X (320 - PROFILE_FILE_LISTBOX_W)
#define PROFILE_FILE_LISTBOX_H 182

#define ITEMS_PER_PAGE 3
#define LIST_ITEM_X 4
#define ITEM_LIST_Y_OFFSET 19
#define LIST_ITEM_W (PROFILE_FILE_LISTBOX_W - (LIST_ITEM_X*2))
#define LIST_ITEM_HEIGHT 42

#define SCROLL_BUTTONS_X 5
#define SCROLL_BUTTONS_Y 144
#define SCROLL_BUTTONS_W (PROFILE_FILE_LISTBOX_W - (SCROLL_BUTTONS_X/2) - 6)
#define SCROLL_BUTTONS_H 22

class ProfileListPanel
{
private:
	TFT_eSPI* _tft = nullptr;
	TFT_eSprite* _sprite = nullptr;
	uint16_t* _sprPtr = nullptr; 

	String _currentProfileFileName;
	String* _profileList = nullptr;
	int _numberProfiles = 0;
	ProfileListItem* _itemList = nullptr;
	ScrollButtons* _scrollButtons = nullptr;
	int _currentPage = 0;
	int _numPages = 0;

	void Update();
public:
	ProfileListPanel();
	~ProfileListPanel();
	ProfileListPanel(TFT_eSPI* tft);
	void Draw(String selectedProfileFileName);
	bool Touched(int x, int y, String& option);
};

