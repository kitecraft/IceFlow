#include "SideBar.h"
#include "../../DisplayManager/Utilities/DisplayQueue.h"
#include "../Utilities/ScreenNames.h"


SideBar::SideBar()
{
}

SideBar::SideBar(TFT_eSPI* tft, DMCoordinates coordinates)
{
	_tft = tft;
	_coordinates = coordinates;
	_settingsIcon = new SettingsIcon(
		IconBaseDto(
			DMCoordinates(ICON_X, SETTINGS_ICON_Y, ICON_WIDTH, ICON_HEIGHT - ICON_H_PADDING, _coordinates.p_x + ICON_X, _coordinates.p_y + SETTINGS_ICON_Y),
			GlobalTheme), 
		_tft);

	_profileIcon = new ProfileIcon(
		IconBaseDto(
			DMCoordinates(ICON_X, PROFILE_ICON_Y, ICON_WIDTH, ICON_HEIGHT - ICON_H_PADDING, _coordinates.p_x + ICON_X, _coordinates.p_y + PROFILE_ICON_Y),
			GlobalTheme),
		_tft);
}

SideBar::~SideBar()
{
	delete(_settingsIcon);
	delete(_profileIcon);
}

void SideBar::Draw()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(_coordinates.w, _coordinates.h);

	sprite.fillSprite(TFT_BLACK);
	_settingsIcon->Draw(&sprite);
	_profileIcon->Draw(&sprite);

	_tft->pushImageDMA(_coordinates.x, _coordinates.y, _coordinates.w, _coordinates.h, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

bool SideBar::Touched(int x, int y)
{
	if (_popUpMenu.isOpen()) {
		String option;
		if (_popUpMenu.Touched(x, y, option)) {
			Serial.print("Option touched: ");
			Serial.println(option);
			_popUpMenu.Close();
			return true;
		}

		switch (_activeIcon) {
		case SettingsIconID:
			if (_settingsIcon->Touched(x, y)) {
				_popUpMenu.Close();
				return true;
			}
			break;
		default:
			return false;
		}
		return false;
	}

	if (_settingsIcon->Touched(x, y)) {
		String options[2] = { "Settings","Info" };		
		_popUpMenu.Open(
			PopUpMenuDto(
				DMCoordinates(_settingsIcon->GetPX() - 3, _settingsIcon->GetPY(), 0, 0),
				TR_DATUM,
				GlobalTheme,
				MEDIUM_FONT,
				MEDIUM_FONT_TEXT_H),
			options,
			2,
			_tft);
		_activeIcon = SettingsIconID;
		return true;
	}

	if (_profileIcon->Touched(x, y)) {
		DisplayQueue.QueueScreenChange(SN_PROFILES_SCREEN);
	}

	return false;
}