#pragma once
#include <Arduino.h>
#include <TFT_eSPI.h>
#include "../Utilities/DMTheme.h"
#include "../Utilities/Screens_Config.h"
#include "../../ProfileManager/ProfileManager.h"
#include "ProfileListItem.h"

#define PROFILE_FILE_LISTBOX_Y 35
#define PROFILE_FILE_LISTBOX_W 80
#define PROFILE_FILE_LISTBOX_X (320 - PROFILE_FILE_LISTBOX_W)
#define PROFILE_FILE_LISTBOX_H 182

#define ITEMS_PER_PAGE 5
#define LIST_ITEM_X 6
#define ITEM_LIST_Y_OFFSET 22
#define LIST_ITEM_W (PROFILE_FILE_LISTBOX_W - (LIST_ITEM_X*2))
#define LIST_ITEM_HEIGHT 26

enum SCROLL_DIRECTION {
	SCROLL_NONE = -1,
	SCROLL_UP,
	SCROLL_DOWN
};

class ProfileListPanel
{
private:
	String _currentProfileFileName;
	String* _profileList = nullptr;
	int _numberProfiles = 0;
	ProfileListItem* _itemList = nullptr;
	int _currentPage = 0;
	int _numPages = 0;

public:
	ProfileListPanel();
	~ProfileListPanel();
	void Draw(TFT_eSPI* tft, String selectedProfileFileName);
	void Update(SCROLL_DIRECTION scrollDir, String selectedProfileFileName);
};

