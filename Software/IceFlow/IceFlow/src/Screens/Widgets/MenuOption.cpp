#include "MenuOption.h"
#include "Box.h"

MenuOption::MenuOption()
{
}

MenuOption::MenuOption(String name, PopUpMenuDto config)
{
	_name = name;
	_config = config;
	_config.coordinates.w = (_config.coordinates.w + _config.coordinates.h + 6);
}

MenuOption::MenuOption(const MenuOption& orig)
{
	_name = orig._name;
	_config = orig._config;
}
void MenuOption::Draw(TFT_eSprite* sprite)
{
	DrawRoundedBox(sprite, _config.coordinates, _config.coordinates.h/2, _config.theme, _config.theme.panelLightColor, true);
	sprite->setTextDatum(MC_DATUM);
	sprite->setTextColor(_config.theme.textColor, _config.theme.panelDarkColor);
	sprite->drawString(_name, _config.coordinates.x + (_config.coordinates.w / 2), (_config.coordinates.y + (_config.coordinates.h / 2)) - 1);
}

bool MenuOption::Touched(int x, int y)
{
	if (x >= _config.coordinates.p_x &&
		x <= _config.coordinates.p_x + _config.coordinates.w &&
		y >= _config.coordinates.p_y &&
		y <= _config.coordinates.p_y + _config.coordinates.h)
	{
		return true;
	}
	return false;
}