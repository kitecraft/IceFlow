#include "SplashScreen.h"

SplashScreen::SplashScreen(TFT_eSPI *newTFT) : ScreenBase(newTFT)
{
	TFT->fillScreen(TFT_BLACK);
	TFT->setTextDatum(TL_DATUM);

	TFT->loadFont(AA_FONT_18PT);

	col[0] = 0xED7F;
	col[1] = 0xD4DB;
	col[2] = 0xBC38;
	col[3] = 0xABD6;
	col[4] = 0x9374;
	col[5] = 0x8332;
	col[6] = 0x72AF;
	col[7] = 0x624D;

	circleLoopCounter = 8;
	secondaryLoopCounter = 0;

	DrawPanel(SS_NET_PANEL_Y, "Network");
	DrawPanel(SS_IP_PANEL_Y,  "IP Addr");
	Update_NetworkConnectionInfo(TFT_RED);

	if(CheckForErrorOnLast())
	{
		//this is where I can pop-up an error message
		//return;
		//allQueues.PrintDebugMessage("Error:  AboutScreen::AboutScreen() " + GetLastError().msg);
	}
}

SplashScreen::~SplashScreen()
{
}

void SplashScreen::UpdateScreen(DISPLAY_UPDATE_KEYS inKey, String inPayload)
{
    switch (inKey) {
    case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_WIFI_SSID:
		networkSSID = inPayload;
		Update_NetworkConnectionInfo(TFT_RED);
        break;
	case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_WIFI_LOCAL_IP:
		iAddress = inPayload;
		Update_IPAddrInfo(TFT_RED);
		break;
    case DISPLAY_UPDATE_KEYS::DISPLAY_UPDATE_KEY_WIFI_STATUS_CHANGED:
		if (inPayload == "true") {
			Update_NetworkConnectionInfo(TFT_GREEN);
			Update_IPAddrInfo(TFT_GREEN);
		}
		else {
			Update_NetworkConnectionInfo(TFT_RED);
			Update_IPAddrInfo(TFT_RED);
		}
        break;
    default:
        break;
    }
}

void SplashScreen::UpdateScreenOnInterval()
{
	if (millis() - lastUpdate > updateScreenInterval)
	{
		lastUpdate = millis();
		UpdateCircle();
	}
}

void SplashScreen::UpdateCircle()
{
	TFT->fillCircle(SPINNER_X + 30 * (cos(-(circleLoopCounter + secondaryLoopCounter) * PI / 4)), SPINNER_Y + 30 * (sin(-(circleLoopCounter + secondaryLoopCounter) * PI / 4)), 6, col[secondaryLoopCounter]);

	secondaryLoopCounter++;
	if (secondaryLoopCounter > 7)
	{
		secondaryLoopCounter = 0;
		circleLoopCounter--;
	}

	if (circleLoopCounter <= 0)
	{
		circleLoopCounter = 8;
	}
}

void SplashScreen::Update_NetworkConnectionInfo(uint16_t circleColor)
{
	UpdatePanel(SS_NET_PANEL_Y, networkSSID, circleColor);
}

void SplashScreen::Update_IPAddrInfo(uint16_t circleColor)
{
	UpdatePanel(SS_IP_PANEL_Y, iAddress, circleColor);
}

void SplashScreen::DrawPanel(int yOffset, String text)
{
	StarsideCoordinates localCoords(SS_PANEL_X, yOffset, SS_PANEL_WIDTH, SS_PANEL_HEIGHT);

	DrawBoxWithBorderAndDropShadow(localCoords, g_GlobalTheme);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, localCoords.h - 7, g_GlobalTheme.panelBGColor);
	TFT->fillRect(localCoords.x + 3, localCoords.y + 4, localCoords.w - 8, 16, g_GlobalTheme.panelHeaderColor);

	TFT->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelHeaderColor);
	TFT->drawString(text, localCoords.x + 40, localCoords.y + 4);

	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 20, localCoords.w - 6, g_GlobalTheme.panelBorderColor);
	TFT->drawFastHLine(localCoords.x + 3, localCoords.y + 21, localCoords.w - 6, g_GlobalTheme.panelBorderColor);
}

void SplashScreen::UpdatePanel(int yOffset, String text, uint16_t circleColor)
{
	TFT_eSprite* sprite = new TFT_eSprite(TFT);
	sprite->createSprite(SS_PANEL_TEXT_WIDTH, SS_PANEL_TEXT_HEIGHT);

	sprite->setTextDatum(TL_DATUM);
	sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);

	sprite->fillSprite(g_GlobalTheme.panelBGColor);
	sprite->loadFont(AA_FONT_18PT);
	sprite->drawString(text, 0, 0);
	sprite->pushSprite(SS_PANEL_X + 28, yOffset + 25);

	sprite->unloadFont();
	sprite->deleteSprite();
	delete(sprite);
	TFT->fillCircle(SS_PANEL_X + 14, yOffset + 32, 7, circleColor);
}



