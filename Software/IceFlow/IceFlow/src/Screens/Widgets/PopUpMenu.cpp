#include "PopUpMenu.h"
#include "Box.h"

PopUpMenu::PopUpMenu()
{
}

PopUpMenu::~PopUpMenu()
{
	Clear();	
}

void PopUpMenu::Clear()
{
	if (_sprite != nullptr) {
		_sprite->deleteSprite();
		delete(_sprite);
	}

	if (_menuOptions != nullptr) {
		delete[]_menuOptions;
	}

	if (_screenReadBuffer != nullptr) {
		free(_screenReadBuffer);
	}

	_tft = nullptr;
	_sprite = nullptr;
	_screenReadBuffer = nullptr;
	_menuOptions = nullptr;
	_numOptions = 0;
	_lengthOfLongestOptionText = 0;
	_open = false;
	_opening = false;
	_closing = false;
}

void PopUpMenu::Configure(PopUpMenuDto configDto, String* menuOptions, int numMenuOptions, TFT_eSPI* tft)
{
	Clear();
	_config = configDto;
	_numOptions = numMenuOptions;
	_tft = tft;	

	_sprite = new TFT_eSprite(_tft);
	_sprite->setFreeFont(_config.font);
	_sprite->setTextColor(_config.theme.textColor, _config.theme.panelLightColor);
	_sprite->setTextDatum(MC_DATUM);
	_menuOptions = new MenuOption[_numOptions];

	int textWidth = 0;
	for (int i = 0; i < _numOptions; i++) {
		textWidth = _sprite->textWidth(menuOptions[i]);
		if (textWidth > _lengthOfLongestOptionText) {
			_lengthOfLongestOptionText = textWidth;
		}
	}

	int menuOptionHeight = (_config.fontHeight + OPTION_TEXT_V_PADDING + OPTION_BORDER_SIZE);
	_config.coordinates.w = (OPTION_BORDER_PADDING * 2) + _lengthOfLongestOptionText + menuOptionHeight + 6;
	_config.coordinates.h = (OPTION_BORDER_PADDING * 2)  + ((menuOptionHeight + (OPTION_SPACING * (_numOptions - 1))) * _numOptions);

	switch (_config.alignment) {
	case TR_DATUM:
		_config.coordinates.x = _config.coordinates.x - _config.coordinates.w;
		break;
	case BL_DATUM:
		_config.coordinates.y = _config.coordinates.y - _config.coordinates.h;
		break;
	case BR_DATUM:
		_config.coordinates.y = _config.coordinates.y - _config.coordinates.h;
		_config.coordinates.x = _config.coordinates.x - _config.coordinates.w;
		break;
	}
	if (_config.coordinates.y + _config.coordinates.h > _tft->height()) {
		_config.coordinates.y -= (_config.coordinates.y + _config.coordinates.h) - _tft->height();
	}
	if (_config.coordinates.y < 0) {
		_config.coordinates.y = 0;
	}

	for (int i = 0; i < _numOptions; i++) {
		PopUpMenuDto optionDto = _config;
		optionDto.coordinates.x = OPTION_BORDER_PADDING;
		optionDto.coordinates.y = OPTION_BORDER_PADDING + ((menuOptionHeight + OPTION_SPACING) * i);
		optionDto.coordinates.w = _lengthOfLongestOptionText;
		optionDto.coordinates.h = menuOptionHeight;
		optionDto.coordinates.p_x = _config.coordinates.x + optionDto.coordinates.x;
		optionDto.coordinates.p_y = _config.coordinates.y + optionDto.coordinates.y;
		_menuOptions[i] = MenuOption(menuOptions[i], optionDto);
	}
	
	_screenReadBuffer = (uint16_t*)calloc(_config.coordinates.w * _config.coordinates.h, sizeof(uint16_t));
}

void PopUpMenu::Open(PopUpMenuDto configDto, String* menuOptions, int numMenuOptions, TFT_eSPI* tft)
{
	Configure(configDto, menuOptions, numMenuOptions, tft);
	_tft->readRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w, _config.coordinates.h, _screenReadBuffer);
	
	_sprite->createSprite(_config.coordinates.w, _config.coordinates.h);
	//_sprite->pushImage(0, 0, _config.coordinates.w, _config.coordinates.h, _screenReadBuffer);
	
	_sprite->fillSprite(TFT_TRANSPARENT);
	DrawRoundedBox(_sprite, DMCoordinates(0, 0, _config.coordinates.w, _config.coordinates.h), 5, _config.theme, TFT_TRANSPARENT);
	
	for (int i = 0; i < _numOptions; i++) {
		_menuOptions[i].Draw(_sprite);
	}
	
	_sprite->pushSprite(_config.coordinates.x, _config.coordinates.y);
	_open = true;
}

void PopUpMenu::Close()
{
	_tft->pushRect(_config.coordinates.x, _config.coordinates.y, _config.coordinates.w, _config.coordinates.h, _screenReadBuffer);
	Clear();
}

bool PopUpMenu::Touched(int x, int y, String& menuString)
{
	for (int i = 0; i < _numOptions; i++) {
		if (_menuOptions[i].Touched(x, y)) {
			menuString = _menuOptions[i].GetName();
			return true;
		}
	}
	return false;
}
