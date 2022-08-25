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

	if (_messageBox != nullptr) {
		delete _messageBox;
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

	if (_clearNetworkInfoButton->Touched(x, y)) {
		
		_messageBox = new MessageBox(
			_tft,
			"Clear Network?",
			String("This will clear any stored network data.\nContinue?"),
			MESSAGE_BOX_ICON_QUESTION,
			MESSAGE_BOX_BUTTONS_CONTINUE_CANCEL
		);
		_messageBox->Show();
		_messageBoxActive = IS_MBAF_CLEAR_NETWORK;
	}

	if (_exitButton->Touched(x, y)) {
		DisplayQueue.QueueScreenChange(SN_MAIN_SCREEN);
	}
}

void InfoScreen::HandleMessageBoxTouch(int x, int y)
{
	DialogButtonType mbRet = _messageBox->Touched(x, y);
	if (mbRet == DB_NONE) {
		return;
	}

	switch (_messageBoxActive) {
	case IS_MBAF_CLEAR_NETWORK:
		EndMessageBox();
		if (mbRet == DB_CONTINUE) {
			SetSsid("");
			SetSsidPassword("");
			_messageBox = new MessageBox(
				_tft,
				"Network Cleared",
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
	DrawDisplay();

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

void InfoScreen::DrawDisplay()
{
	TFT_eSprite sprite(_tft);
	uint16_t* sprPtr = (uint16_t*)sprite.createSprite(IS_DISPLAY_BOX_W, IS_DISPLAY_BOX_H);
	sprite.fillSprite(TFT_BLACK);
	sprite.setFreeFont(MEDIUM_FONT);
	sprite.setTextColor(GlobalTheme.textColor);

	DrawRoundedBox(&sprite, DMCoordinates(0, 0, IS_DISPLAY_BOX_W, IS_DISPLAY_BOX_H, 0, 0), IS_HEADER_H / 2, GlobalTheme);
	sprite.drawString("Display Calibration data can be deleted and", 9, 5);
	sprite.drawString("reset upon next system boot.", 9, 22);


	_tft->pushImageDMA(IS_DISPLAY_BOX_X, IS_DISPLAY_BOX_Y, IS_DISPLAY_BOX_W, IS_DISPLAY_BOX_H, sprPtr);

	_tft->dmaWait();
	sprite.deleteSprite();
}