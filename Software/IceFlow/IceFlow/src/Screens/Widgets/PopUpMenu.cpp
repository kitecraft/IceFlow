#include "PopUpMenu.h"

PopUpMenu::PopUpMenu()
{
	if (psramFound()) {
		pop_malloc = ps_malloc;
	}
	else {
		pop_malloc = malloc;
	}
}

PopUpMenu::~PopUpMenu()
{
	Clear();	
}

void PopUpMenu::Clear()
{
	if (_menuOptions != nullptr) {
		delete[]_menuOptions;
		_menuOptions = nullptr;
	}

	if (_screenReadBuffer != nullptr) {
		free(_screenReadBuffer);
		_screenReadBuffer = nullptr;
	}
	_tft = nullptr;
	_numOptions = 0;
	_lengthOfLongestOptionName = 0;
	_open = false;
	_opening = false;
	_closing = false;
}

void PopUpMenu::Configure(PopUpMenuDto configDto, String* menuOptions, int numMenuOptions, TFT_eSPI* tft)
{
	Clear();
	_tft = tft;
	_config = configDto;
	_numOptions = numMenuOptions;

	if (_numOptions > 0) {
		_menuOptions = new MenuOption[_numOptions];
	}

	for (int i = 0; i < _numOptions; i++) {
		_menuOptions[i].name = menuOptions[i];
		if (_menuOptions[i].name.length() > _lengthOfLongestOptionName) {
			_lengthOfLongestOptionName = _menuOptions[i].name.length();
		}
		PopUpMenuDto optionDto = _config;
		//optionDto.coordinates.x = 
		//optionDto.coordinates.x =
	}
}

void PopUpMenu::Open(PopUpMenuDto configDto, String* menuOptions, int numMenuOptions, TFT_eSPI* tft)
{
	Configure(configDto, menuOptions, numMenuOptions, tft);

	_screenReadBuffer = (uint16_t*)pop_malloc(((_config.coordinates.w + 2) * (_config.coordinates.h + 2)) * 2);
	_tft->readRect(_config.coordinates.p_x, _config.coordinates.p_y, _config.coordinates.w, _config.coordinates.h, _screenReadBuffer);



	_open = true;
}

void PopUpMenu::Close()
{
	if (_tft == nullptr) {
		return;
	}

	_tft->pushRect(_config.coordinates.p_x, _config.coordinates.p_y, _config.coordinates.w, _config.coordinates.h, _screenReadBuffer);
	if (_screenReadBuffer != nullptr) {
		free(_screenReadBuffer);
		_screenReadBuffer = nullptr;
	}

	Clear();
}

String PopUpMenu::Touched(int x, int y)
{
	if (_tft == nullptr) {
		return "";
	}

	for (int i = 0; i < _numOptions; i++) {

	}
	return "Testable";
}
