#include "KeyboardButton.h"


KeyboardButton::KeyboardButton()
{

}

KeyboardButton::KeyboardButton(TFT_eSPI* tft, DMCoordinates coordinates, KeyboardButtonType  type)
{
	_tft = tft;
	_coordinates = coordinates;
	_type = type;
	CreateKey();
}

KeyboardButton::~KeyboardButton()
{
	delete _button;
}

KeyboardButtonType KeyboardButton::Touched(int x, int y)
{
	if (_button->Touched(x, y)) {
		return _type;
	}
	return KBT_NO_BUTTON;
}

String KeyboardButton::GetDisplayedValue()
{
	if (_type == KBT_SPACE) {
		return(" ");
	}

	if (_shifted) {
		return(String(_upperText));
	}
	else {
		return(String(_text));
	}
}

void KeyboardButton::Draw(TFT_eSprite* sprite, bool shift)
{
	_shifted = shift;

	if (_shifted) {
		_button->Draw(sprite, String(_upperText));
	}
	else {
		_button->Draw(sprite, String(_text));
	}
}

void KeyboardButton::CreateKey()
{
	SetKeyText();
	String label = String(_text);

	_button = new Button(
		ButtonDto(
			DMCoordinates(_coordinates.x, _coordinates.y, _coordinates.w, _coordinates.h, _coordinates.p_x, _coordinates.p_y),
			GlobalTheme,
			MEDIUM_FONT,
			BUTTON_COLOR,
			GlobalTheme.panelDarkColor),
		_text,
		_tft,
		false);
}

void KeyboardButton::SetKeyText()
{
	switch (_type) {
	case KBT_SHIFT:
		strcpy(_text, "Sh");
		strcpy(_upperText, "Sh");
		break;
	case KBT_1:
		strcpy(_text, "1");
		strcpy(_upperText, "1");
		break;
	case KBT_2:
		strcpy(_text, "2");
		strcpy(_upperText, "2");
		break;
	case KBT_3:
		strcpy(_text, "3");
		strcpy(_upperText, "3");
		break;
	case KBT_4:
		strcpy(_text, "4");
		strcpy(_upperText, "4");
		break;
	case KBT_5:
		strcpy(_text, "5");
		strcpy(_upperText, "5");
		break;
	case KBT_6:
		strcpy(_text, "6");
		strcpy(_upperText, "6");
		break;
	case KBT_7:
		strcpy(_text, "7");
		strcpy(_upperText, "7");
		break;
	case KBT_8:
		strcpy(_text, "8");
		strcpy(_upperText, "8");
		break;
	case KBT_9:
		strcpy(_text, "9");
		strcpy(_upperText, "(");
		break;
	case KBT_0:
		strcpy(_text, "0");
		strcpy(_upperText, ")");
		break;
	case KBT_BSP:
		strcpy(_text, "<-");
		strcpy(_upperText, "<-");
		break;
	case KBT_q:
		strcpy(_text, "q");
		strcpy(_upperText, "Q");
		break;
	case KBT_w:
		strcpy(_text, "w");
		strcpy(_upperText, "W");
		break;
	case KBT_e:
		strcpy(_text, "e");
		strcpy(_upperText, "E");
		break;
	case KBT_r:
		strcpy(_text, "r");
		strcpy(_upperText, "R");
		break;
	case KBT_t:
		strcpy(_text, "t");
		strcpy(_upperText, "T");
		break;
	case KBT_y:
		strcpy(_text, "y");
		strcpy(_upperText, "Y");
		break;
	case KBT_u:
		strcpy(_text, "u");
		strcpy(_upperText, "U");
		break;
	case KBT_i:
		strcpy(_text, "i");
		strcpy(_upperText, "I");
		break;
	case KBT_o:
		strcpy(_text, "o");
		strcpy(_upperText, "O");
		break;
	case KBT_p:
		strcpy(_text, "p");
		strcpy(_upperText, "P");
		break;
	case KBT_HYPEN:
		strcpy(_text, "-");
		strcpy(_upperText, "-");
		break;
	case KBT_a:
		strcpy(_text, "a");
		strcpy(_upperText, "A");
		break;
	case KBT_s:
		strcpy(_text, "s");
		strcpy(_upperText, "S");
		break;
	case KBT_d:
		strcpy(_text, "d");
		strcpy(_upperText, "D");
		break;
	case KBT_f:
		strcpy(_text, "f");
		strcpy(_upperText, "F");
		break;
	case KBT_g:
		strcpy(_text, "g");
		strcpy(_upperText, "G");
		break;
	case KBT_h:
		strcpy(_text, "h");
		strcpy(_upperText, "H");
		break;
	case KBT_j:
		strcpy(_text, "j");
		strcpy(_upperText, "J");
		break;
	case KBT_k:
		strcpy(_text, "k");
		strcpy(_upperText, "K");
		break;
	case KBT_l:
		strcpy(_text, "l");
		strcpy(_upperText, "L");
		break;
	case KBT_ENTER:
		strcpy(_text, "->");
		strcpy(_upperText, "->");
		break;
	case KBT_CANCEL:
		strcpy(_text, "XX");
		strcpy(_upperText, "XX");
		break;
	case KBT_z:
		strcpy(_text, "z");
		strcpy(_upperText, "Z");
		break;
	case KBT_x:
		strcpy(_text, "x");
		strcpy(_upperText, "X");
		break;
	case KBT_c:
		strcpy(_text, "c");
		strcpy(_upperText, "C");
		break;
	case KBT_v:
		strcpy(_text, "v");
		strcpy(_upperText, "V");
		break;
	case KBT_b:
		strcpy(_text, "b");
		strcpy(_upperText, "B");
		break;
	case KBT_SPACE:
		strcpy(_text, "__");
		strcpy(_upperText, "__");
		break;
	case KBT_n:
		strcpy(_text, "n");
		strcpy(_upperText, "N");
		break;
	case KBT_m:
		strcpy(_text, "m");
		strcpy(_upperText, "M");
		break;
	case KBT__:
		strcpy(_text, "_");
		strcpy(_upperText, "_");
		break;
	case KBT_DOT:
		strcpy(_text, ".");
		strcpy(_upperText, ".");
		break;
	}

}