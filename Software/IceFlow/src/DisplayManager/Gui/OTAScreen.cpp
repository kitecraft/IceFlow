#include "OTAScreen.h"

OTAScreen::OTAScreen(TFT_eSPI *newTFT) : ScreenBase(newTFT)
{
	//exitButton.Init(TFT);
	TFT->fillScreen(TFT_BLACK);

	if(CheckForErrorOnLast())
	{
		//this is where I can pop-up an error message
		//return;
		//allQueues.PrintDebugMessage("Error:  OTAScreen::OTAScreen() " + GetLastError().msg);
	}

	DrawEntireScreen();
	//allQueues.PrintDebugMessage("OTA B: Free Heap: " + String(ESP.getFreeHeap()));
}

OTAScreen::~OTAScreen()
{
}

void OTAScreen::DrawEntireScreen()
{
	TFT->fillScreen(TFT_BLACK);

	circleLoopCounter = 1;
	OTA_Update_In_Progress = false;

	Draw_OTA_Information_Panel();
	//exitButton.Draw();
}

void OTAScreen::UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
	bool found = true;
	switch (inKey) {

	case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_OTA_STARTING:
		Starting_OTA_Update();
		break;
	case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_WIFI_SSID:
		networkName = inPayload;
		RedrawNetworkName();
		break;
	case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_WIFI_LOCAL_IP:
		RedrawIPAddress(inPayload);
		break;
	case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_OTA_COMPLETE:
		Ending_OTA_Update();
		break;
	case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_OTA_PROGRESS_UPDATE:
		Update_In_Progress(inPayload.toInt());
		break;
	default:
		found = false;
		break;
	}
}

void OTAScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		lastUpdate = millis();
		if(!OTA_Update_In_Progress)
		{
			Draw_Waiting_Animation();
		}
	}
}


void OTAScreen::HandleTouch(int x, int y)
{
	/*
	if (exitButton.Touched(x, y))
	{
		allQueues.AddItemToCommandQueue(STARSIDE_CMD_START_SLOBSTERDECK, "");
	}
	*/
}

void OTAScreen::Draw_Waiting_Animation()
{
    TFT_eSprite* sprite = new TFT_eSprite(TFT);

	sprite->createSprite(OTA_PANEL_WIDTH - 25, 30);
	sprite->fillSprite(g_GlobalTheme.panelBGColor);
	sprite->fillCircle(20 * circleLoopCounter, 14, 10, g_GlobalTheme.panelHeaderColor);

	sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 88);

	sprite->deleteSprite();
	delete(sprite);

	circleLoopCounter++;
	if(circleLoopCounter > 11)
	{
		circleLoopCounter = 1;
	}
}

void OTAScreen::Draw_OTA_Information_Panel()
{
	StarsideCoordinates localCoords(0, 0, OTA_PANEL_WIDTH, OTA_PANEL_HEIGHT);

	TFT_eSprite* otaBoxSprite = new TFT_eSprite(TFT);
	otaBoxSprite->createSprite(OTA_PANEL_WIDTH, OTA_PANEL_HEIGHT);

	DrawBoxWithBorderAndDropShadow(otaBoxSprite, localCoords, g_GlobalTheme);

	otaBoxSprite->fillRect(3, 4, localCoords.w - 8, localCoords.h - 7, g_GlobalTheme.panelBGColor);
	otaBoxSprite->fillRect(3, 4, localCoords.w - 8, 19, g_GlobalTheme.panelHeaderColor);

	// Horizontal line seperator UNDER the header
	otaBoxSprite->drawFastHLine(3, 23, localCoords.w - 6, g_GlobalTheme.panelBorderColor);
	otaBoxSprite->drawFastHLine(3, 24, localCoords.w - 6, g_GlobalTheme.panelBorderColor);

	// Horizontal line seperator UNDER SSID
	otaBoxSprite->drawFastHLine(3, 43, localCoords.w - 6, g_GlobalTheme.panelBorderColor);
	otaBoxSprite->drawFastHLine(3, 44, localCoords.w - 6, g_GlobalTheme.panelBorderColor);

	// Horizontal line seperator UNDER IP
	otaBoxSprite->drawFastHLine(3, 63, localCoords.w - 6, g_GlobalTheme.panelBorderColor);
	otaBoxSprite->drawFastHLine(3, 64, localCoords.w - 6, g_GlobalTheme.panelBorderColor);

	//This is the progress indicator box
	DrawBoxWithBorderAndDropShadow(otaBoxSprite, StarsideCoordinates(6, 83, OTA_PANEL_WIDTH - 12, 40), g_GlobalTheme);

	otaBoxSprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelHeaderColor);
	otaBoxSprite->loadFont(AA_FONT_18PT);
	otaBoxSprite->setTextDatum(TC_DATUM);
	otaBoxSprite->drawString("Firmware Update", 120, 5);
	otaBoxSprite->unloadFont();

	otaBoxSprite->loadFont(AA_FONT_14PT);
	otaBoxSprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);
	otaBoxSprite->drawString("OTA ready and waiting....", 120, 69);

	otaBoxSprite->setTextDatum(TL_DATUM);
	otaBoxSprite->drawString("SSID:", 6, 29);
	otaBoxSprite->drawString("IP:", 6, 49);

	otaBoxSprite->pushSprite(OTA_PANEL_X, OTA_PANEL_Y);

	otaBoxSprite->unloadFont();
	otaBoxSprite->deleteSprite();
	delete(otaBoxSprite);
}

void OTAScreen::RedrawNetworkName()
{
	TFT_eSprite* sprite = new TFT_eSprite(TFT);
	sprite->loadFont(AA_FONT_14PT);
	sprite->setTextDatum(TL_DATUM);
	sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);

	sprite->createSprite(224, sprite->fontHeight());

	sprite->fillSprite(g_GlobalTheme.panelBGColor);
	sprite->drawString("SSID:", 0, 0);
	sprite->drawString(networkName, 35, 0);

	sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 29);

	sprite->unloadFont();
	sprite->deleteSprite();
	delete(sprite);
}

void OTAScreen::RedrawIPAddress(String ipAddr)
{
	TFT_eSprite* sprite = new TFT_eSprite(TFT);
	sprite->loadFont(AA_FONT_14PT);
	sprite->setTextDatum(TL_DATUM);
	sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);

	sprite->createSprite(224, sprite->fontHeight());

	sprite->fillSprite(g_GlobalTheme.panelBGColor);
	sprite->drawString("IP:", 0, 0);
	sprite->drawString(ipAddr, 18, 0);

	sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 49);

    sprite->unloadFont();
	sprite->deleteSprite();
	delete(sprite);
}

void OTAScreen::Starting_OTA_Update()
{
	OTA_Update_In_Progress = true;

	//exitButton.Erase();

	TFT_eSprite* sprite = new TFT_eSprite(TFT);
    sprite->loadFont(AA_FONT_14PT);
	sprite->setTextDatum(TC_DATUM);
	sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);
	sprite->createSprite(224, sprite->fontHeight());

	sprite->fillSprite(g_GlobalTheme.panelBGColor);
	sprite->drawString("Firmware is being updated.", 120, 0);

	sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 69);
	sprite->deleteSprite();


	sprite->createSprite(OTA_PANEL_WIDTH - 25, 30);
	sprite->fillSprite(0xEDE2);

	sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 88);

    sprite->unloadFont();
	sprite->deleteSprite();
	delete(sprite);
}

void OTAScreen::Update_In_Progress(int percentage)
{
	float gaugeWidth = OTA_PANEL_WIDTH - 25;
	int doneWidth = (gaugeWidth / (float)100) * percentage;

	TFT_eSprite* sprite = new TFT_eSprite(TFT);

	sprite->createSprite(gaugeWidth, 30);
	sprite->fillSprite(0xEDE2);
	sprite->fillRect(0,0,doneWidth,30,TFT_GREEN);

	sprite->pushSprite(OTA_PANEL_X + 11, OTA_PANEL_Y + 88);

	sprite->deleteSprite();
	delete(sprite);
}

void OTAScreen::Ending_OTA_Update()
{
	TFT_eSprite* sprite = new TFT_eSprite(TFT);
    sprite->loadFont(AA_FONT_14PT);
	sprite->setTextDatum(TL_DATUM);
	sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);
	sprite->createSprite(224, sprite->fontHeight());

	sprite->fillSprite(g_GlobalTheme.panelBGColor);
	sprite->drawString("Update has completed. Restarting...", 10, 0);

	sprite->pushSprite(OTA_PANEL_X + 6, OTA_PANEL_Y + 69);

    sprite->unloadFont();
	sprite->deleteSprite();
	delete(sprite);
}