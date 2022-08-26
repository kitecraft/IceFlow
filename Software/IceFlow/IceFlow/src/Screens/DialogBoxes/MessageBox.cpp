#include "MessageBox.h"


MessageBox::MessageBox() : DialogBase()
{

}

MessageBox::~MessageBox()
{
	if (_buttonA != nullptr) {
		delete _buttonA;
	}
	if (_buttonB != nullptr) {
		delete _buttonB;
	}
}

MessageBox::MessageBox(TFT_eSPI* tft, String title, String message, MESSAGE_BOX_ICON icon, MESSAGE_BOX_BUTTONS buttons)
	:DialogBase(tft, DMCoordinates(0, 0, MESSAGE_BOX_WIDTH, 150, 20, 20), GlobalTheme, title)
{
	_buttonA = nullptr;
	_buttonB = nullptr;

	_buttonY = _coordinates.h - LARGE_FONT_TEXT_BOX_H - 6;
	_singleButtonX = (_coordinates.w / 2) - (DIALOG_BUTTON_WIDTH / 2);
	_dualButtons_ButtonA_X = (_coordinates.w / 2) - DIALOG_BUTTON_WIDTH - 5;
	_dualButtons_ButtonB_X = (_coordinates.w / 2) + 5;

	PrintMessage(message);
	SetButtons(buttons);
	DrawIcon(icon);
}

void MessageBox::PrintMessage(String message)
{
	_sprite->setTextColor(_theme.textColor);
	_sprite->setTextDatum(ML_DATUM);
	_sprite->setFreeFont(MEDIUM_FONT);

	int charAt = MESSAGE_BOX_MAX_LINE_LENGTH;
	if (charAt >= message.length()) {
		_sprite->drawString(message, MESSAGE_BOX_TEXT_X, MESSAGE_BOX_TEXT_Y_START);
	}
	else {
		String line = "";
		int currentLineStart = 0;
		int lineNumber = 0;
		bool continueLooking = true;

		while (continueLooking) {
			char thisChar = message.charAt(charAt);
			while (thisChar != ' ' && thisChar != '\n' && charAt > currentLineStart) {
				charAt--;;
				thisChar = message.charAt(charAt);
			}

			for (int i = currentLineStart; i < charAt; i++) {
				if (message.charAt(i) == '\n') {
					charAt = i;
					break;
				}
			}

			if (charAt > currentLineStart) {
				line = message.substring(currentLineStart, charAt);
				_sprite->drawString(line, MESSAGE_BOX_TEXT_X, MESSAGE_BOX_TEXT_Y_START + (MESSAGE_BOX_LINE_HEIGHT * lineNumber));
				currentLineStart = charAt + 1;
				charAt += MESSAGE_BOX_MAX_LINE_LENGTH;
				lineNumber++;
			}
			else {
				continueLooking == false;
				break;
			}

			if (charAt >= message.length()) {
				line = message.substring(currentLineStart, charAt);
				if (line.indexOf("\n") == -1) {
					_sprite->drawString(line, MESSAGE_BOX_TEXT_X, MESSAGE_BOX_TEXT_Y_START + (MESSAGE_BOX_LINE_HEIGHT * lineNumber));
					continueLooking = false;
				}
			}
		}
	}
}

void MessageBox::SetButtons(MESSAGE_BOX_BUTTONS buttons)
{
	switch (buttons) {
	case MESSAGE_BOX_BUTTONS_OK:
		CreateOK();
		break;
	case MESSAGE_BOX_BUTTONS_OK_CANCEL:
		CreateOKCancel();
		break;
	case MESSAGE_BOX_BUTTONS_SUBMIT:
		CreateSubmit();
		break;
	case MESSAGE_BOX_BUTTONS_SUBMIT_CANCEL:
		CreateSubmitCancel();
		break;
	case MESSAGE_BOX_BUTTONS_CONTINUE:
		CreateContinue();
		break;
	case MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL:
		CreateContinueCancel();
		break;
	}
}

void MessageBox::CreateOK()
{
	_buttonA = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_singleButtonX, _buttonY, 0, 0, _coordinates.p_x + _singleButtonX, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_OK),
		_tft
	);
	_buttonA->Draw(_sprite);
}

void MessageBox::CreateOKCancel()
{
	_buttonA = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonA_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_OK),
		_tft
	);

	_buttonB = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonB_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CANCEL),
		_tft
	);
	_buttonA->Draw(_sprite);
	_buttonB->Draw(_sprite);
}

void MessageBox::CreateSubmit()
{
	_buttonA = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonA_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_SUBMIT),
		_tft
	);
	_buttonA->Draw(_sprite);
}

void MessageBox::CreateSubmitCancel()
{
	_buttonA = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonA_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_SUBMIT),
		_tft
	);

	_buttonB = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonB_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CANCEL),
		_tft
	);
	_buttonA->Draw(_sprite);
	_buttonB->Draw(_sprite);
}

void MessageBox::CreateContinue()
{
	_buttonA = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonA_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CONTINUE),
		_tft
	);
	_buttonA->Draw(_sprite);
}

void MessageBox::CreateContinueCancel()
{
	_buttonA = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonA_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonA_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CONTINUE),
		_tft
	);

	_buttonB = new DialogButton(
		DialogButtonDto(
			DMCoordinates(_dualButtons_ButtonB_X, _buttonY, 0, 0, _coordinates.p_x + _dualButtons_ButtonB_X, _coordinates.p_y + _buttonY),
			GlobalTheme,
			MEDIUM_FONT,
			DB_CANCEL),
		_tft
	);
	_buttonA->Draw(_sprite);
	_buttonB->Draw(_sprite);
}



DialogButtonType MessageBox::Touched(int x, int y)
{
	if (!_visible) {
		return DB_NONE;
	}

	if (_buttonA != nullptr) {
		if (_buttonA->Touched(x, y))
		{
			return _buttonA->GetType();
		}
	}
	
	if (_buttonB != nullptr) {
		if (_buttonB->Touched(x, y))
		{
			return _buttonB->GetType();
		}
	}
	return DB_NONE;
}


void MessageBox::DrawIcon(MESSAGE_BOX_ICON icon)
{
	_sprite->drawFastVLine(_coordinates.w - 34, _coordinates.y, LARGE_FONT_TEXT_BOX_H, _theme.panelBorderColor);
	switch (icon) {
	case MESSAGE_BOX_ICON_ERROR:
		DrawErrorIcon();
		break;
	case MESSAGE_BOX_ICON_WARNING:
		DrawWarningIcon();
		break;
	case MESSAGE_BOX_ICON_INFORMATION:
		DrawInfoIcon();
		break;
	case MESSAGE_BOX_ICON_QUESTION:
		DrawQuestionIcon();
		break;
	}
}


void MessageBox::DrawErrorIcon()
{
	for (int i = 0; i < 5; i++) {
		_sprite->drawFastHLine(_coordinates.w - 20 - i, _coordinates.y + 6 + i, 6 + (i * 2), TFT_RED);
		_sprite->drawFastHLine(_coordinates.w - 20 - i, _coordinates.y + 6 + (14 - i), 6 + (i * 2), TFT_RED);
		_sprite->drawFastHLine(_coordinates.w - 25, _coordinates.y + 6 + (5 + i), 16, TFT_RED);
	}
	_sprite->setTextColor(TFT_WHITE);
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextDatum(MC_DATUM);
	_sprite->drawString("X", _coordinates.w - 17, _coordinates.y + 13);
}

void MessageBox::DrawWarningIcon()
{
	//_sprite->drawFastVLine(_coordinates.w - 34, _coordinates.y, LARGE_FONT_TEXT_BOX_H, _theme.panelBorderColor);
	_sprite->fillTriangle(
		_coordinates.w - 20,
		_coordinates.y + 6,
		_coordinates.w - 28,
		_coordinates.y + 21,
		_coordinates.w - 12,
		_coordinates.y + 21,
		TFT_YELLOW
	);
	_sprite->setTextColor(TFT_BLACK);
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextDatum(MC_DATUM);
	_sprite->drawString("!", _coordinates.w - 20, _coordinates.y + 13);
}

void MessageBox::DrawInfoIcon()
{
	_sprite->fillCircle(_coordinates.w - 20, _coordinates.y + 13, 7, TFT_BLUE);
	_sprite->setTextColor(TFT_WHITE);
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextDatum(MC_DATUM);
	_sprite->drawString("i", _coordinates.w - 20, _coordinates.y + 13);
}

void MessageBox::DrawQuestionIcon()
{
	_sprite->fillCircle(_coordinates.w - 20, _coordinates.y + 13, 7, TFT_BLUE);
	_sprite->setTextColor(TFT_WHITE);
	_sprite->setFreeFont(MEDIUM_FONT);
	_sprite->setTextDatum(MC_DATUM);
	_sprite->drawString("?", _coordinates.w - 20, _coordinates.y + 13);
}