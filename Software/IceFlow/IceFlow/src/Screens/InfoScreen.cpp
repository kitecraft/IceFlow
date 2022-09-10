#include "InfoScreen.h"
#include "Utilities/ScreenNames.h"
#include "Utilities/ScreenUpdateKeys.h"
#include "../DisplayManager/Utilities/DisplayQueue.h"
#include "../Utilities/ControlCommands.h"
#include "../DisplayManager/Utilities/CommandQueue.h"
#include "Widgets/Box.h"
#include "../Utilities/PreferencesManager.h"

InfoScreen::InfoScreen(TFT_eSPI* tft)
{
	_messageBox = nullptr;
	_messageBoxActive = IS_MBAF_NONE;
	_numberPad = nullptr;

	_tft = tft;
	_exitButton = new Button(
		ButtonDto(
			DMCoordinates(
				0,
				0,
				IS_EXIT_BUTTON_W,
				IS_EXIT_BUTTON_H,
				IS_EXIT_BUTTON_X,
				IS_EXIT_BUTTON_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			GlobalTheme.panelDarkColor),
		"Exit",
		_tft,
		true
	);

	_deleteTouchCalibarationButton = new Button(
		ButtonDto(
			DMCoordinates(
				IS_DISPLAY_DELETE_CALIBRATION_X,
				IS_DISPLAY_DELETE_CALIBRATION_Y,
				IS_DISPLAY_DELETE_CALIBRATION_W,
				IS_DISPLAY_DELETE_CALIBRATION_H,
				IS_DISPLAY_BOX_X + IS_DISPLAY_DELETE_CALIBRATION_X,
				IS_DISPLAY_BOX_Y + IS_DISPLAY_DELETE_CALIBRATION_Y
			),
			GlobalTheme,
			SMALL_FONT,
			GlobalTheme.panelDarkColor),
		"Delete Calibration",
		_tft,
		true
	);

	_maximumOvenTemperature = new TextBox(TextBoxDto(
		DMCoordinates(
			IS_DISPLAY_MAX_TEMP_TB_X,
			IS_DISPLAY_MAX_TEMP_TB_Y,
			IS_DISPLAY_MAX_TEMP_TB_W,
			IS_DISPLAY_MAX_TEMP_TB_H,
			IS_DISPLAY_MAX_TEMP_X + IS_DISPLAY_MAX_TEMP_TB_X,
			IS_DISPLAY_MAX_TEMP_Y + IS_DISPLAY_MAX_TEMP_TB_Y
		),
		GlobalTheme,
		MEDIUM_FONT,
		ML_DATUM),
		_tft);

	_applyNewMaximumTemperature = new Button(
		ButtonDto(
			DMCoordinates(
				IS_DISPLAY_APPLY_BUTTON_X,
				IS_DISPLAY_APPLY_BUTTON_Y,
				IS_DISPLAY_APPLY_BUTTON_W,
				IS_DISPLAY_APPLY_BUTTON_H,
				IS_DISPLAY_MAX_TEMP_X + IS_DISPLAY_APPLY_BUTTON_X,
				IS_DISPLAY_MAX_TEMP_Y + IS_DISPLAY_APPLY_BUTTON_Y
			),
			GlobalTheme,
			SMALL_FONT,
			GlobalTheme.panelDarkColor),
		"Apply",
		_tft,
		true
	);

	CreateNetworkWidgets();

	Draw();

	CommandQueue.QueueCommand(CC_REQUEST_NET_NAME);
	CommandQueue.QueueCommand(CC_REQUEST_NET_IP);
	CommandQueue.QueueCommand(CC_REQUEST_MAC_ADDRESS);
}

InfoScreen::~InfoScreen()
{
	delete _exitButton;
	delete _networkNameTB;
	delete _networkIPAddressTB;
	delete _macAddressTB;
	delete _clearNetworkInfoButton;
	delete _deleteTouchCalibarationButton;
	delete _maximumOvenTemperature;
	delete _applyNewMaximumTemperature;

	if (_messageBox != nullptr) {
		delete _messageBox;
	}

	if (_numberPad != nullptr) {
		delete _numberPad;
	}
}

void InfoScreen::CreateNetworkWidgets()
{
	_networkNameTB = new TextBox(TextBoxDto(
		DMCoordinates(
			IS_NETWORK_NAME_TB_X,
			IS_NETWORK_NAME_TB_Y,
			150,
			MEDIUM_FONT_TEXT_BOX_H,
			IS_NETWORK_BOX_X + IS_NETWORK_NAME_TB_X,
			IS_NETWORK_BOX_Y + IS_NETWORK_NAME_TB_Y
		),
		GlobalTheme,
		MEDIUM_FONT,
		ML_DATUM),
		_tft);

	_networkIPAddressTB = new TextBox(TextBoxDto(
		DMCoordinates(
			IS_NETWORK_IP_ADDRESS_TB_X,
			IS_NETWORK_IP_ADDRESS_TB_Y,
			150,
			MEDIUM_FONT_TEXT_BOX_H,
			IS_NETWORK_BOX_X + IS_NETWORK_IP_ADDRESS_TB_X,
			IS_NETWORK_BOX_Y + IS_NETWORK_IP_ADDRESS_TB_Y
		),
		GlobalTheme,
		MEDIUM_FONT,
		ML_DATUM),
		_tft);

	_macAddressTB = new TextBox(TextBoxDto(
		DMCoordinates(
			IS_NETWORK_MAC_ADDRESS_TB_X,
			IS_NETWORK_MAC_ADDRESS_TB_Y,
			150,
			MEDIUM_FONT_TEXT_BOX_H,
			IS_NETWORK_BOX_X + IS_NETWORK_MAC_ADDRESS_TB_X,
			IS_NETWORK_BOX_Y + IS_NETWORK_MAC_ADDRESS_TB_Y
		),
		GlobalTheme,
		MEDIUM_FONT,
		ML_DATUM),
		_tft);

	_clearNetworkInfoButton = new Button(
		ButtonDto(
			DMCoordinates(
				IS_NETWORK_CLEAR_BUTTON_X,
				IS_NETWORK_CLEAR_BUTTON_Y,
				60,
				MEDIUM_FONT_TEXT_BOX_H,
				IS_NETWORK_BOX_X + IS_NETWORK_CLEAR_BUTTON_X,
				IS_NETWORK_BOX_Y + IS_NETWORK_CLEAR_BUTTON_Y
			),
			GlobalTheme,
			MEDIUM_FONT,
			GlobalTheme.panelDarkColor),
		"Clear",
		_tft
	);

}

void InfoScreen::UpdateScreen(int inKey, char* value)
{

	SCREEN_UPDATE_KEYS key = static_cast<SCREEN_UPDATE_KEYS>(inKey);

	_tft->startWrite();
	switch (key) {
	case suk_Network_Name:
		_networkNameTB->Update(value);
		break;
	case suk_IP_Address:
		_networkIPAddressTB->Update(value);
		break;
	case suk_Mac_Address:
		_macAddressTB->Update(value);
		break;
	default:
		break;
	}

	_tft->dmaWait();
	_tft->endWrite();
}

void InfoScreen::HandleTouch(int x, int y)
{
	_tft->startWrite();
	ProcessTouch(x, y);
	_tft->dmaWait();
	_tft->endWrite();
}

void InfoScreen::ProcessTouch(int x, int y)
{
	if (_messageBoxActive != IS_MBAF_NONE) {
		HandleMessageBoxTouch(x, y);
		return;
	}

	if (_numberPad != nullptr) {
		if (_numberPad->Touched(x, y)) {
			int num = (int)_numberPad->GetNumber();
			if (num != 0) {
				_maximumOvenTemperature->Update(int(_numberPad->GetNumber()));
			}
			_numberPad->Hide();
			delete _numberPad;
			_numberPad = nullptr;
			return;
		}
	}

	if (_applyNewMaximumTemperature->Touched(x, y)) {
		SaveOvenDoNotExceedTemperature(_maximumOvenTemperature->GetText().toInt());

		_messageBox = new MessageBox(
			_tft,
			"Maximum Saved.",
			String("The maximum allowed temperature has been saved."),
			MESSAGE_BOX_ICON_INFORMATION,
			MESSAGE_BOX_BUTTONS_OK
		);
		_messageBox->Show();
		_messageBoxActive = IS_MBAF_SAVED_DO_NOT_EXCEED;
		CommandQueue.QueueCommand(CC_UPDATE_DO_NOT_EXCEED_TEMPERATURE);
		return;
	}

	if (_maximumOvenTemperature->Touched(x, y)) {
		_numberPad = new NumberPadDialogBox(_tft, "Max");
		_numberPad->SetNumber(_maximumOvenTemperature->GetText());
		_numberPad->Show();
		return;
	}

	if (_clearNetworkInfoButton->Touched(x, y)) {
		_messageBox = new MessageBox(
			_tft,
			"Clear Network?",
			String("This will clear any stored network data.\nContinue?"),
			MESSAGE_BOX_ICON_QUESTION,
			MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL
		);
		_messageBox->Show();
		_messageBoxActive = IS_MBAF_CONFRIM_CLEAR_NETWORK;
		return;
	}

	if (_deleteTouchCalibarationButton->Touched(x, y)) {
		_messageBox = new MessageBox(
			_tft,
			"Delete Calibration?",
			String("This will delete the touch calibration data\nContinue?"),
			MESSAGE_BOX_ICON_QUESTION,
			MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL
		);
		_messageBox->Show();
		_messageBoxActive = IS_MBAF_CONFIRM_DELETE_CALIBRATION;
		return;
	}

	if (_exitButton->Touched(x, y)) {
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
		return;
	}
}

void InfoScreen::HandleMessageBoxTouch(int x, int y)
{
	DialogButtonType mbRet = _messageBox->Touched(x, y);
	if (mbRet == DB_NONE) {
		return;
	}

	switch (_messageBoxActive) {
	case IS_MBAF_CONFRIM_CLEAR_NETWORK:
		EndMessageBox();
		if (mbRet == DB_CONTINUE) {
			SetSsid("");
			SetSsidPassword("");
			_messageBox = new MessageBox(
				_tft,
				"Network Cleared.",
				"The stored network info has been cleared.",
				MESSAGE_BOX_ICON_INFORMATION,
				MESSAGE_BOX_BUTTONS_OK
			);
			_messageBox->Show();
			_messageBoxActive = IS_MBAF_NETWORK_CLEARED;
		}
		break;
	case IS_MBAF_NETWORK_CLEARED:
		EndMessageBox();
		CommandQueue.QueueCommand(CC_REQUEST_NET_NAME);
		CommandQueue.QueueCommand(CC_REQUEST_NET_IP);
		break;
	case IS_MBAF_CONFIRM_DELETE_CALIBRATION:
		EndMessageBox();
		if (mbRet == DB_CONTINUE) {
			CommandQueue.QueueCommand(CC_DELETE_TOUCH_CALIBRATION);
			_messageBox = new MessageBox(
				_tft,
				"Calibration deleted.",
				"The touchscreen will be recalibrated upon next system boot.",
				MESSAGE_BOX_ICON_INFORMATION,
				MESSAGE_BOX_BUTTONS_OK
			);
			_messageBox->Show();
			_messageBoxActive = IS_MBAF_CALIBRATION_DELETED;
		}
		break;
	case IS_MBAF_CALIBRATION_DELETED:
	case IS_MBAF_SAVED_DO_NOT_EXCEED:
		EndMessageBox();
		break;
	default:
		break;
	}

}

void InfoScreen::EndMessageBox()
{
	if (_messageBox != nullptr) {
		_messageBox->Hide();
		delete _messageBox;
		_messageBox = nullptr;
	}
	_messageBoxActive = IS_MBAF_NONE;
}

void InfoScreen::Draw()
{
	_tft->fillScreen(TFT_BLACK);

	_tft->startWrite();
	DrawHeader();
	DrawNetwork();
	DrawTouchCalibration();
	DrawMaximumTemperature();

	_exitButton->Draw();

	_tft->dmaWait();
	_tft->endWrite();
}

void InfoScreen::DrawHeader()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(IS_HEADER_W, IS_HEADER_H);
	uint16_t* buffer = (uint16_t*)ps_malloc(((IS_HEADER_W * IS_HEADER_H) * 2) + 1);

	sprite.fillSprite(TFT_BLACK);
	DrawRoundedBox(&sprite, DMCoordinates(IS_HEADER_X, IS_HEADER_Y, IS_HEADER_W, IS_HEADER_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);

	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextColor(GlobalTheme.textColor, GlobalTheme.panelLightColor);
	String message = String(__DEVICE_NAME__) + "  v" + String(__DEVICE_VERSION__);
	sprite.drawString(message, IS_DEVICE_NAME_X, IS_DEVICE_NAME_Y);

	message = "Author: " + String(__DEVICE_AUTHOR__);
	sprite.drawString(message, IS_AUTHOR_NAME_X, IS_DEVICE_NAME_Y);

	_tft->pushImageDMA(IS_HEADER_X, IS_HEADER_Y, IS_HEADER_W, IS_HEADER_H, sprPtr);


	_tft->dmaWait();
	sprite.deleteSprite();
}

void InfoScreen::DrawNetwork()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(IS_NETWORK_BOX_W, IS_NETWORK_BOX_H);
	sprite.fillSprite(TFT_BLACK);
	sprite.setFreeFont(LARGE_FONT);
	sprite.setTextColor(GlobalTheme.textColor);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, IS_NETWORK_BOX_W, IS_NETWORK_BOX_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);
	sprite.drawString("Network", 9, 5);
	sprite.drawFastHLine(2, 25, 82, GlobalTheme.panelBorderColor);
	sprite.drawFastHLine(2, 26, 82, GlobalTheme.panelBorderColor);
	sprite.drawFastVLine(82, 2, 23, GlobalTheme.panelBorderColor);
	sprite.drawFastVLine(83, 2, 23, GlobalTheme.panelBorderColor);

	sprite.setFreeFont(MEDIUM_FONT);
	sprite.drawString("Name", 90, IS_NETWORK_NAME_TB_Y + 3);
	sprite.setTextDatum(TR_DATUM);
	sprite.drawString("IP Address", IS_NETWORK_IP_ADDRESS_TB_X - 3, IS_NETWORK_IP_ADDRESS_TB_Y + 3);
	sprite.drawString("Mac Address", IS_NETWORK_MAC_ADDRESS_TB_X - 3, IS_NETWORK_MAC_ADDRESS_TB_Y + 3);

	_networkNameTB->Draw(&sprite, "");
	_networkIPAddressTB->Draw(&sprite, "");
	_macAddressTB->Draw(&sprite, "");
	_clearNetworkInfoButton->Draw(&sprite);

	_tft->pushImageDMA(IS_NETWORK_BOX_X, IS_NETWORK_BOX_Y, IS_NETWORK_BOX_W, IS_NETWORK_BOX_H, sprPtr);
	
	_tft->dmaWait();
	sprite.deleteSprite();
}

void InfoScreen::DrawTouchCalibration()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(IS_DISPLAY_BOX_W, IS_DISPLAY_BOX_H);
	sprite.fillSprite(TFT_BLACK);
	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(GlobalTheme.textColor);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, IS_DISPLAY_BOX_W, IS_DISPLAY_BOX_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);
	sprite.drawString("Touch Calibration data can be deleted and", 9, 5);
	sprite.drawString("reset upon next system boot.", 9, 22);

	_deleteTouchCalibarationButton->Draw(&sprite);
	_tft->pushImageDMA(IS_DISPLAY_BOX_X, IS_DISPLAY_BOX_Y, IS_DISPLAY_BOX_W, IS_DISPLAY_BOX_H, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}

void InfoScreen::DrawMaximumTemperature()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(IS_DISPLAY_MAX_TEMP_W, IS_DISPLAY_MAX_TEMP_H);
	sprite.fillSprite(TFT_BLACK);
	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(GlobalTheme.textColor);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, IS_DISPLAY_MAX_TEMP_W, IS_DISPLAY_MAX_TEMP_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);
	sprite.drawString("If the oven exceeds this temperature,", 9, 5);
	sprite.drawString("it will be immediatly shut down.", 9, 22);

	_maximumOvenTemperature->Draw(&sprite, GetOvenDoNotExceedTemperature());
	_applyNewMaximumTemperature->Draw(&sprite);

	_tft->pushImageDMA(IS_DISPLAY_MAX_TEMP_X, IS_DISPLAY_MAX_TEMP_Y, IS_DISPLAY_MAX_TEMP_W, IS_DISPLAY_MAX_TEMP_H, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();

}