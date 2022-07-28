#include "ProfileListPanel.h"
#include "../Widgets/Box.h"

ProfileListPanel::ProfileListPanel()
{
	_numberProfiles = ProfileManager.GetListOfProfileFileNames(_profileList);
	if (_numberProfiles <= 0) {
		return;
	}

	_itemList = new ProfileListItem[_numberProfiles];
	int slotOnPage = 0;
	for (int i = 0; i < _numberProfiles; i++) {
		_itemList[i] = ProfileListItem(
			ProfileListItemDto(
				DMCoordinates(
					LIST_ITEM_X + PROFILE_FILE_LISTBOX_X,
					PROFILE_FILE_LISTBOX_Y + (ITEM_LIST_Y_OFFSET * slotOnPage),
					LIST_ITEM_W,
					LIST_ITEM_HEIGHT,
					LIST_ITEM_X,
					ITEM_LIST_Y_OFFSET * i
				),
				GlobalTheme,
				ProfileManager.GetNameOfProfileByFileName(_profileList[i]),
				_profileList[i]
			)
		);

		slotOnPage++;
		if (slotOnPage == ITEMS_PER_PAGE) {
			_numPages++;
			slotOnPage = 0;
		}
	}
}

ProfileListPanel::~ProfileListPanel()
{
	if (_profileList != nullptr) {
		delete[] _profileList;
	}
	if (_itemList != nullptr) {
		delete[] _itemList;
	}
}



void ProfileListPanel::Draw(TFT_eSPI* tft, String selectedProfileFileName)
{
	_currentProfileFileName = selectedProfileFileName;
	_currentPage = 0;

	TFT_eSprite sprite(tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(0, 0, PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H, 0, 0), 5, GlobalTheme);

	sprite.setFreeFont(SMALL_FONT);
	sprite.setTextColor(TFT_GREEN, GlobalTheme.panelLightColor);
	sprite.setTextDatum(TL_DATUM);
	String msg = "Found " + String(_numberProfiles) + " files.";
	sprite.drawString(msg, 10, 8);
	sprite.drawFastHLine(5, 20, PROFILE_FILE_LISTBOX_W - 10, GlobalTheme.panelBorderColor);

	for (int i = (_currentPage * ITEMS_PER_PAGE); i < ITEMS_PER_PAGE && i < _numberProfiles; i++) {
		bool status = false;
		if (_itemList[i].GetFileName() == selectedProfileFileName) {
			status = true;
		}
		_itemList[i].Draw(&sprite, status);
	}


	sprite.drawFastHLine(5, PROFILE_FILE_LISTBOX_H - 26, PROFILE_FILE_LISTBOX_W - 10, GlobalTheme.panelBorderColor);
	sprite.drawString("Free Space:", 6, PROFILE_FILE_LISTBOX_H - 24);
	msg = String(ProfileManager.GetFreeSpaceKB()) + " KB";
	sprite.drawString(msg, 6, PROFILE_FILE_LISTBOX_H - 14);

	tft->pushImageDMA(PROFILE_FILE_LISTBOX_X, PROFILE_FILE_LISTBOX_Y, PROFILE_FILE_LISTBOX_W, PROFILE_FILE_LISTBOX_H, sprPtr);
	tft->dmaWait();
	sprite.deleteSprite();
}

void ProfileListPanel::Update(SCROLL_DIRECTION scrollDir, String selectedProfileFileName)
{

}