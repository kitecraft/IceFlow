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

	_manualHeatIcon = new ManualHeatIcon(
		IconBaseDto(
			DMCoordinates(ICON_X, MANUAL_HEAT_ICON_Y, ICON_WIDTH, ICON_HEIGHT - ICON_H_PADDING, _coordinates.p_x + ICON_X, _coordinates.p_y + MANUAL_HEAT_ICON_Y),
			GlobalTheme),
		_tft);

	_reflowIcon = new ReflowIcon(
		IconBaseDto(
			DMCoordinates(ICON_X, REFLOW_ICON_Y, ICON_WIDTH, ICON_HEIGHT - ICON_H_PADDING, _coordinates.p_x + ICON_X, _coordinates.p_y + REFLOW_ICON_Y),
			GlobalTheme),
		_tft);
	_stopIcon = new StopIcon(
		IconBaseDto(
			DMCoordinates(ICON_X, STOP_ICON_Y, ICON_WIDTH, ICON_HEIGHT - ICON_H_PADDING, _coordinates.p_x + ICON_X, _coordinates.p_y + STOP_ICON_Y),
			GlobalTheme),
		_tft);

	_stopIcon->SetEnabled(false);
}

SideBar::~SideBar()
{
	delete _settingsIcon;
	delete _profileIcon;
	delete _manualHeatIcon;
	delete _reflowIcon;
	delete _stopIcon;
}

void SideBar::Draw()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(_coordinates.w, _coordinates.h);

	sprite.fillSprite(TFT_BLACK);
	_settingsIcon->Draw(&sprite);
	_profileIcon->Draw(&sprite);
	_manualHeatIcon->Draw(&sprite);
	_reflowIcon->Draw(&sprite);
	_stopIcon->Draw(&sprite);

	_tft->pushImageDMA(_coordinates.x, _coordinates.y, _coordinates.w, _coordinates.h, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

SB_TOUCHED_RETURN SideBar::Touched(int x, int y)
{
	if (_popUpMenu.isOpen()) {
		String option;
		if (_popUpMenu.Touched(x, y, option)) {
			Serial.print("Option touched: ");
			Serial.println(option);

			_popUpMenu.Close();
			if (option == "AutoTune") {
				return SB_START_AUTO_TUNE;
			}
			return SB_MENU_CLOSED;
		}

		switch (_activeIcon) {
		case SettingsIconID:
			if (_settingsIcon->Touched(x, y)) {
				_popUpMenu.Close();
				return SB_MENU_CLOSED;
			}
			break;
		default:
			return SB_NOT_TOUCHED;
		}
		return SB_NOT_TOUCHED;
	}

	if (_settingsIcon->Touched(x, y)) {
		String options[3] = { "Settings", "AutoTune", "Info" };		
		_popUpMenu.Open(
			PopUpMenuDto(
				DMCoordinates(_settingsIcon->GetPX() - 3, _settingsIcon->GetPY(), 0, 0),
				TR_DATUM,
				GlobalTheme,
				MEDIUM_FONT,
				MEDIUM_FONT_TEXT_H),
			options,
			3,
			_tft);
		_activeIcon = SettingsIconID;
		return SB_TOUCHED;
	}

	if (_profileIcon->Touched(x, y)) {
		DisplayQueue.QueueScreenChange(SN_PROFILES_SCREEN);
	}
	else if (_manualHeatIcon->Touched(x, y)) {
		return SB_START_MANUAL_HEAT;
	}
	else if (_reflowIcon->Touched(x, y)) {
		return SB_START_REFLOW;
	}
	else if (_stopIcon->Touched(x, y)) {
		return SB_STOP;
	}

	return SB_NOT_TOUCHED;
}

void SideBar::ManualHeatIconEnabled(bool enabled)
{
	_manualHeatIcon->SetEnabled(enabled); 
}

void SideBar::ReflowIconEnabled(bool enabled)
{
	_reflowIcon->SetEnabled(enabled);
}

void SideBar::StopIconEnabled(bool enabled)
{
	_stopIcon->SetEnabled(enabled);
}