#include "DialogButton.h"


DialogButton::DialogButton(DialogButtonDto config, TFT_eSPI* tft)
{
	_config = config;
	String label = "";
	switch (_config.type) {
	case DB_Cancel:
		label = "Cancel";
		break;
	case DB_Save:
		label = "Save";
		break;
	case DB_Continue:
		label = "Continue";
		break;
	case DB_OK:
		label = "OK";
		break;
	default:
		label = "Nope";
		break;
	}

	_button = new Button(
		ButtonDto(
			DMCoordinates(_config.coordinates.x, _config.coordinates.y, DIALOG_BUTTON_WIDTH, DIALOG_BUTTON_HEIGHT, _config.coordinates.p_x, _config.coordinates.p_y),
			_config.theme,
			_config.font,
			BUTTON_COLOR,
			GlobalTheme.panelDarkColor),
		label,
		tft,
		false);
}


DialogButton::~DialogButton()
{
	if (_button != nullptr) {
		delete _button;
	}
}

void DialogButton::Draw(TFT_eSprite *sprite)
{
	_button->Draw(sprite);
}

bool DialogButton::Touched(int x, int y)
{
	return _button->Touched(x, y);
}