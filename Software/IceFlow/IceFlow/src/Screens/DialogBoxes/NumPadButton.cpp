#include "NumPadButton.h"

NumPadButton::NumPadButton(TFT_eSPI* tft, NUM_PAD_BUTTON type, DMCoordinates coordinates, DMTheme theme)
{
	SetLabel(type);

	_button = new Button(
		ButtonDto(
			coordinates,
			theme,
			MEDIUM_FONT,
			BUTTON_COLOR,
			GlobalTheme.panelDarkColor),
		_label,
		tft,
		false);
}

NumPadButton::~NumPadButton()
{
	delete _button;
}

void NumPadButton::Draw(TFT_eSprite* sprite)
{
	_button->Draw(sprite);
}

NUM_PAD_BUTTON NumPadButton::Touched(int x, int y)
{
	if (_button->Touched(x, y)) {
		return _type;
	}
	return NPB_NO_BUTTON;
}

void NumPadButton::SetLabel(NUM_PAD_BUTTON type)
{
	_type = type;
	switch (_type) {
	case NPB_ZERO:
		_label = "0";
		break;
	case NPB_ONE:
		_label = "1";
		break;
	case NPB_TWO:
		_label = "2";
		break;
	case NPB_THREE:
		_label = "3";
		break;
	case NPB_FOUR:
		_label = "4";
		break;
	case NPB_FIVE:
		_label = "5";
		break;
	case NPB_SIX:
		_label = "6";
		break;
	case NPB_SEVEN:
		_label = "7";
		break;
	case NPB_EIGHT:
		_label = "8";
		break;
	case NPB_NINE:
		_label = "9";
		break;
	case NPB_BSP:
		_label = "X";
		break;
	case NPB_ENTER:
		_label = "->";
		break;
	case NPB_DECIMAL:
		_label = ".";
		break;
	default:
		_label = "NN";
		break;
	}
}