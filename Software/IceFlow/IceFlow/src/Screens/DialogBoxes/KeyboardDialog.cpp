#include "KeyboardDialog.h"
#include "../Widgets/Box.h"

KeyboardDialog::KeyboardDialog(TFT_eSPI* tft, String title)
{
	_tft = tft;
	_coordinates = DMCoordinates(0, 0, 320, 100, 0, 0);
	_theme = GlobalTheme;
	_title = title;

	_valueTB = new TextBox(TextBoxDto(
		DMCoordinates(
			KBD_VALUE_TB_X,
			KBD_VALUE_TB_Y,
			KBD_VALUE_TB_W,
			KBD_VALUE_TB_H,
			KBD_VALUE_TB_X,
			KBD_VALUE_TB_Y
		),
		GlobalTheme,
		MEDIUM_FONT,
		MR_DATUM),
		_tft);
	CreateButtons();
}

KeyboardDialog::~KeyboardDialog()
{

	for (int i = 0; i < NUMBER_OF_KEYBOARD_BUTTONS; i++) {
		delete _kbButtons[i];
	}

	if (_valueTB != nullptr) {
		delete _valueTB;
	}
}

void KeyboardDialog::CreateButtons()
{
	int currKey = 0;
	for (int row = 0; row < KBD_NUM_ROWS; row++) {
		int rowY = KBD_BUTTON_ROW_START_Y + (KBD_BUTTON_HEIGHT * row);
		for (int column = 0; column < KDB_NUM_COLUMS; column++){
			int x = KBD_BUTTON_WIDTH * column;
			_kbButtons[currKey] = new KeyboardButton(_tft, DMCoordinates(x, 0, KBD_BUTTON_WIDTH, KBD_BUTTON_HEIGHT, x, rowY), static_cast<KeyboardButtonType>(currKey));
			currKey++;
		}
	}
}

void KeyboardDialog::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(KBD_WIDTH, KBD_HEADER_H);

	DrawSquaredBox(&sprite, DMCoordinates(0, 0, KBD_WIDTH, KBD_HEADER_H, 0, 0), GlobalTheme, true);

	if (_value.length() == _maxLength && !_selected) {
		_valueTB->Draw(&sprite, _value, (uint16_t)KBG_MAX_LENGTH_BG_COLOR);
	}
	else {
		_valueTB->Draw(&sprite, _value, _selected);
	}

	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextDatum(ML_DATUM);
	sprite.setTextColor(GlobalTheme.textColor);
	sprite.drawString(_title, 8, 12);

	_tft->pushImageDMA(0, 0, KBD_WIDTH, KBD_HEADER_H, sprPtr);
	_tft->dmaWait();
	sprite.deleteSprite();
}

void KeyboardDialog::DrawButtons()
{
	TFT_eSprite sprite(_tft);
	uint16_t* buffer = (uint16_t*)ps_malloc(((KBD_WIDTH * KBD_BUTTON_HEIGHT) * 2) + 1);
	uint16_t* sprPtr;

	int currKey = 0;
	for (int row = 0; row < KBD_NUM_ROWS; row++) {
		sprPtr = (uint16_t*)sprite.createSprite(KBD_WIDTH, KBD_BUTTON_HEIGHT);
		sprite.fillSprite(TFT_BLACK);

		for (int column = 0; column < KDB_NUM_COLUMS; column++) {
			_kbButtons[currKey]->Draw(&sprite, _shiftSelected);
			currKey++;
		}
		_tft->pushImageDMA(0, KBD_BUTTON_ROW_START_Y + (KBD_BUTTON_HEIGHT * row), KBD_WIDTH, KBD_BUTTON_HEIGHT, sprPtr, buffer);
	
		sprite.deleteSprite();
	}

	_tft->dmaWait();
	free(buffer);

}

void KeyboardDialog::Show()
{
	_tft->fillScreen(TFT_BLACK);
	_selected = true;
	_shiftSelected = true;
	DrawHeader();
	DrawButtons();

	_visible = true;
}

void KeyboardDialog::Hide()
{
	_visible = false;
}

DialogButtonType KeyboardDialog::Touched(int x, int y)
{
	if (!_visible) {
		return DB_NONE;
	}
	if (_valueTB->Touched(x, y)) {
		_selected = !_selected;

		if (_value.length() == _maxLength && !_selected) {
			_valueTB->Update(_value, (uint16_t)KBG_MAX_LENGTH_BG_COLOR);
		}
		else {
			_valueTB->Update(_value, _selected);
		}
		
		return DB_NONE;
	}

	for (int currButton = 0; currButton < NUMBER_OF_KEYBOARD_BUTTONS; currButton++) {
		KeyboardButtonType buttonPressed = _kbButtons[currButton]->Touched(x, y);

		switch (buttonPressed) {
		case KBT_CANCEL:
			return DB_Cancel;
			break;
		case KBT_SHIFT:
			_shiftSelected = !_shiftSelected;
			DrawButtons();
			break;
		case KBT_BSP:
			BackSpaceValue();
			break;
		case KBT_ENTER:
			return DB_Continue;
			break;
		case KBT_NO_BUTTON:
			break;
		default:
			UpdateValueTextbox(_kbButtons[currButton]->GetDisplayedValue());
			break;
		}
	}

	return DB_NONE;
}

void KeyboardDialog::SetValue(String newValue)
{
	if (newValue.length() >= _maxLength) {
		_value = newValue.substring(0, _maxLength);
	}
	else {
		_value = newValue;
	}
}

void KeyboardDialog::BackSpaceValue()
{
	bool origShift = _shiftSelected;
	if (_selected) {
		_value = "";
		_shiftSelected = true;
	}
	else if (_value.length() > 0) {
		_value.remove(_value.length() - 1);
		_shiftSelected = false;
		if (_value.length() == 0) {
			_shiftSelected = true;
		}
	}

	if (_shiftSelected != origShift){
		DrawButtons();
	}
	_valueTB->Update(_value, false);
	_selected = false;
}

void KeyboardDialog::UpdateValueTextbox(String newChar)
{
	if (_selected) {
		_value = newChar;
	}
	else if (_value.length() + 1 > _maxLength && newChar != _value) {
		return;
	}
	else if (newChar.length() == 1) {
		_value += newChar;
	}
	else {
		_value = newChar;
	}

	if (_value.length() == _maxLength) {
		_valueTB->Update(_value, (uint16_t)KBG_MAX_LENGTH_BG_COLOR);
	}
	else {
		_valueTB->Update(_value, false);
	}

	if (_shiftSelected) {
		_shiftSelected = false;
		DrawButtons();
	}
	if (newChar == " ") {
		_shiftSelected = true;
		DrawButtons();
	}
	_selected = false;
}