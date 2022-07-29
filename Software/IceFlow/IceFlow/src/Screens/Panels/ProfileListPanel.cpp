#include "ProfileListPanel.h"
#include "../Widgets/Box.h"


ProfileListPanel::ProfileListPanel()
{
}

ProfileListPanel::ProfileListPanel(TFT_eSPI* tft)
{
	_tft = tft;
	_sprite = new TFT_eSprite(tft);
	_sprPtr = (uint16_t*)_sprite->createSprite(PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H);

	_numberProfiles = ProfileManager.GetListOfProfileFileNames(_profileList);
	if (_numberProfiles <= 0) {
		return;
	}
	/*
	Serial.print("\nNumber of profiles: ");
	Serial.println(_numberProfiles);
	Serial.print("Item 1: '");
	Serial.print(_profileList[0]);
	Serial.println("'");
	*/
	_itemList = new ProfileListItem[_numberProfiles];

	int slotOnPage = 0;
	_numPages = 1;
	for (int i = 0; i < _numberProfiles; i++) {
		String name = ProfileManager.GetNameOfProfileByFileName(_profileList[i]);
		/*
		Serial.print("Name: ");
		Serial.println(name);
		Serial.print("Filename: ");
		Serial.println(_profileList[i]);
		*/
		ProfileListItem x = ProfileListItem(
			ProfileListItemDto(
				DMCoordinates(
					LIST_ITEM_X,
					ITEM_LIST_Y_OFFSET + (LIST_ITEM_HEIGHT * slotOnPage),
					LIST_ITEM_W,
					LIST_ITEM_HEIGHT,
					LIST_ITEM_X + PROFILE_FILE_LISTBOX_X,
					PROFILE_FILE_LISTBOX_Y + ITEM_LIST_Y_OFFSET + (LIST_ITEM_HEIGHT * slotOnPage)
				),
				GlobalTheme,
				name,
				_profileList[i]
			)
		);
		//Serial.println("");
		_itemList[i] = x;

		slotOnPage++;
		if (slotOnPage == ITEMS_PER_PAGE) {
			_numPages++;
			slotOnPage = 0;
		}
	}

	//means we happened to increment our  page counter,
	//but then didn't have any new list items to add to the new page
	//so decrement the page count, unless there is only one page
	// it'll be empty
	if (slotOnPage == 0 && _numPages != 1) {
		_numPages--; 
	}

	//Serial.print("Created pages: ");
	//Serial.println(_numPages);
	_scrollButtons = new ScrollButtons(
		DMCoordinates(
			SCROLL_BUTTONS_X,
			SCROLL_BUTTONS_Y,
			SCROLL_BUTTONS_W,
			SCROLL_BUTTONS_H,
			PROFILE_FILE_LISTBOX_X + SCROLL_BUTTONS_X,
			PROFILE_FILE_LISTBOX_Y + SCROLL_BUTTONS_Y
		));
}

ProfileListPanel::~ProfileListPanel()
{
	if (_profileList != nullptr) {
		delete[] _profileList;
	}
	if (_itemList != nullptr) {
		delete[] _itemList;
	}
	if (_scrollButtons != nullptr) {
		delete _scrollButtons;
	}
	if (_sprite != nullptr) {
		delete _sprite;
	}
}

void ProfileListPanel::Draw(String selectedProfileFileName)
{
	_currentProfileFileName = selectedProfileFileName;
	_currentPage = 0;
	Update();
}

void ProfileListPanel::Update()
{
	_sprite->fillSprite(TFT_BLACK);
	DrawRoundedBox(_sprite, DMCoordinates(0, 0, PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H, 0, 0), 5, GlobalTheme);

	_sprite->setFreeFont(SMALL_FONT);
	_sprite->setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	_sprite->setTextDatum(TL_DATUM);
	String msg = "Found " + String(_numberProfiles) + " files.";
	_sprite->drawString(msg, 10, 5);
	//_sprite->drawFastHLine(5, 15, PROFILE_FILE_LISTBOX_W - 10, GlobalTheme.panelBorderColor);

	//_sprite->drawFastHLine(5, PROFILE_FILE_LISTBOX_H - 23, PROFILE_FILE_LISTBOX_W - 10, GlobalTheme.panelBorderColor);
	//_sprite->drawString("Free Space:", 6, PROFILE_FILE_LISTBOX_H - 24);
	msg = "Free: " + String(ProfileManager.GetFreeSpaceKB()) + " KB";
	_sprite->drawString(msg, 6, PROFILE_FILE_LISTBOX_H - 14);

	//_sprite->drawString(msg, 6, PROFILE_FILE_LISTBOX_H - 14);

	//Serial.print("Drawing the list on page: ");
	//Serial.println(_currentPage);
	for (int i = (_currentPage * ITEMS_PER_PAGE); (i - (_currentPage * ITEMS_PER_PAGE)) < ITEMS_PER_PAGE && i < _numberProfiles; i++) {
		//Serial.print("i is: ");
		//Serial.println(i);
		bool status = false;
		if (_itemList[i].GetFileName() == _currentProfileFileName) {
			//Serial.println("Selected is true");
			status = true;
		}
		else {
			//Serial.println("Status is false");

		}
		_itemList[i].Draw(_sprite, status);
	}
	//Serial.println("Ended the loop");

	bool scrollDownEnabled = false; //go up in page number
	bool scrollUpEnabled = false; // go down in page number
	if (_currentPage < _numPages - 1) {
		scrollDownEnabled = true;
	}
	if (_currentPage > 0) {
		scrollUpEnabled = true;
	}


	_scrollButtons->Draw(_sprite, scrollDownEnabled, scrollUpEnabled);
	_tft->pushImageDMA(PROFILE_FILE_LISTBOX_X, PROFILE_FILE_LISTBOX_Y, PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H, _sprPtr);
	_tft->dmaWait();
}

bool ProfileListPanel::Touched(int x, int y, String& option)
{
	for (int i = (_currentPage * ITEMS_PER_PAGE); (i - (_currentPage * ITEMS_PER_PAGE)) < ITEMS_PER_PAGE && i < _numberProfiles; i++) {
		if (_itemList[i].Touched(x, y)) {
			option = _itemList[i].GetFileName();
			return true;
		}
	}

	Scrolldirection scrollDirection = _scrollButtons->Touched(x, y);
	if (scrollDirection != SCROLL_NONE) {
		//Serial.println("Scroll was touched: ");
		bool updateRequired = false;
		if (scrollDirection == SCROLL_DOWN) {
			//Serial.println("Scroll DOWN: ");
			if (_currentPage < _numPages ) {
				_currentPage++;
				updateRequired = true;
			}
		}
		else {
			//Serial.println("Scroll UP: ");
			if (_currentPage > 0) {
				_currentPage--;
				updateRequired = true;
			}
		}
		if (updateRequired) {
			//Serial.println("Running the update");
			_tft->startWrite();
			Update();
			_tft->dmaWait();
			_tft->endWrite();
		}
	}
	return false;
}