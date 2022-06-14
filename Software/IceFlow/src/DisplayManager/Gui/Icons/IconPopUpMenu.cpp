#include "IconPopUpMenu.h"

IconPopUpMenu::~IconPopUpMenu()
{
	CleanUp();
}

void IconPopUpMenu::CleanUp()
{
	if (sprite != NULL)
	{
		sprite->unloadFont();
		sprite->deleteSprite();
		delete(sprite);
		sprite = NULL;
	}

	/*
	if (screenReadBuffer != NULL)
	{
		//free(screenReadBuffer);
		delete[] screenReadBuffer;
		screenReadBuffer = NULL;
	}
	*/

	if (options != NULL)
	{
		delete[] options;
		options = NULL;
	}

	menuIsOpen = false;
	numOptions = 0;
	longestStringLength = 0;
}

void IconPopUpMenu::Init(TFT_eSPI* inTFT)
{
	TFT = inTFT;
}

void IconPopUpMenu::Create(StarsideCoordinates inCoordinates, String* menuOptions, int numMenuOptions)
{
	if (numMenuOptions <= 0)
	{
		return;
	}
	if (!clean)
	{
		CleanUp();
	}
	coordinates = inCoordinates;
	numOptions = numMenuOptions;

	TFT->loadFont(MENU_FONT);
	options = new MenuOption[numOptions];
	longestStringLength = 0;

	for (int i = 0; i < numOptions; i++)
	{
		options[i].name = menuOptions[i];
		int length = TFT->textWidth(options[i].name);
		options[i].coordinates = StarsideCoordinates(TEXT_HORIZONTAL_BUFFER, MENU_AREA_MARGIN + (i * ((TEXT_VERTICAL_BUFFER * 2) + MENU_FONT_HEIGHT + MENU_AREA_MARGIN)), length, MENU_FONT_HEIGHT);
		if (length > longestStringLength)
		{
			longestStringLength = length;
		}
	}
	coordinates.w = longestStringLength + (TEXT_HORIZONTAL_BUFFER * 2) + (TEXT_HORIZONTAL_BUFFER * 2);
	coordinates.x = coordinates.x - coordinates.w;
	coordinates.h = (numOptions * ((TEXT_VERTICAL_BUFFER * 2) + MENU_FONT_HEIGHT + MENU_AREA_MARGIN)) + MENU_AREA_MARGIN;

	if (coordinates.y + coordinates.h > TFT_DISPLAY_HEIGHT)
	{
		coordinates.y = TFT_DISPLAY_HEIGHT - coordinates.h;
	}

	TFT->unloadFont();
	clean = false;
}

void IconPopUpMenu::Open(String selected)
{
	if (clean)
	{
		return;
	}

	//screenReadBuffer = (uint16_t*)malloc(((coordinates.w + 1) * (coordinates.h + 1)) * 2);
	/*
	if (screenReadBuffer != NULL)
	{
		//free(screenReadBuffer);
		delete[] screenReadBuffer;
		screenReadBuffer = NULL;
	}
	Serial.print("Before: ");
	Serial.println(ESP.getFreeHeap());
	screenReadBuffer = new uint16_t[(coordinates.w * coordinates.h) * 2];
	Serial.print("After: ");
	Serial.println(ESP.getFreeHeap());

	TFT->readRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, screenReadBuffer);
	*/

	if (sprite != NULL)
	{
		sprite->unloadFont();
		sprite->deleteSprite();
		delete(sprite);
		sprite = NULL;
	}

	sprite = new TFT_eSprite(TFT);
	sprite->createSprite(coordinates.w, coordinates.h);
	sprite->loadFont(MENU_FONT);
	sprite->setTextDatum(TL_DATUM);
	sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);

	sprite->fillSprite(TFT_TRANSPARENT);

	sprite->fillRoundRect(0, 0, coordinates.w, coordinates.h, 8, g_GlobalTheme.panelBGColor);
	DrawRoundedBoxWithDropShadow(StarsideCoordinates(0, 0, coordinates.w, coordinates.h), 5);

	for (int i = 0; i < numOptions; i++)
	{
		DrawPillBoxWithDropShadow(options[i].coordinates);
		
		if (options[i].name == selected)
		{
			sprite->setTextColor(SELECTED_ITEM_TEXT_COLOR, g_GlobalTheme.panelBGColor);
		} else
		{
			sprite->setTextColor(g_GlobalTheme.textColor, g_GlobalTheme.panelBGColor);
		}
		sprite->drawString(options[i].name, TEXT_HORIZONTAL_BUFFER * 2, options[i].coordinates.y + TEXT_VERTICAL_BUFFER);
	}
	sprite->pushSprite(coordinates.x, coordinates.y, TFT_TRANSPARENT);

	menuIsOpen = true;
}

void IconPopUpMenu::Close()
{
	/*
	if (clean)
	{
		return;
	}
	TFT->pushRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, screenReadBuffer);
	*/
	CleanUp();
}

void IconPopUpMenu::DrawPillBoxWithDropShadow(StarsideCoordinates coordinates)
{
	int textAreaWidth = (coordinates.w + (TEXT_HORIZONTAL_BUFFER * 2));
	int textAreaHeight = coordinates.h + (TEXT_VERTICAL_BUFFER * 2);

	sprite->fillRoundRect(coordinates.x, coordinates.y, textAreaWidth, textAreaHeight, textAreaHeight / 2, g_GlobalTheme.panelBGColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, textAreaWidth, textAreaHeight, textAreaHeight / 2, g_GlobalTheme.panelDropShadowColor);
	sprite->drawRoundRect(coordinates.x + 2, coordinates.y + 2, textAreaWidth - 3, textAreaHeight - 4, textAreaHeight / 2, g_GlobalTheme.panelDropShadowColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, textAreaWidth - 1, textAreaHeight - 1, textAreaHeight / 2, g_GlobalTheme.panelBorderColor);
	sprite->drawRoundRect(coordinates.x + 1, coordinates.y + 1, textAreaWidth - 3, textAreaHeight - 3, textAreaHeight / 2, g_GlobalTheme.panelBorderColor);
}

void IconPopUpMenu::DrawRoundedBoxWithDropShadow(StarsideCoordinates coordinates, int radius)
{

	sprite->fillRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, g_GlobalTheme.panelHeaderColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, coordinates.w, coordinates.h, radius, g_GlobalTheme.panelDropShadowColor);
	sprite->drawRoundRect(coordinates.x + 2, coordinates.y + 2, coordinates.w - 3, coordinates.h - 4, radius, g_GlobalTheme.panelDropShadowColor);

	sprite->drawRoundRect(coordinates.x, coordinates.y, coordinates.w - 1, coordinates.h - 1, radius, g_GlobalTheme.panelBorderColor);
	sprite->drawRoundRect(coordinates.x + 1, coordinates.y + 1, coordinates.w - 3, coordinates.h - 3, radius, g_GlobalTheme.panelBorderColor);
}

String IconPopUpMenu::Touched(int x, int y)
{
	for (int i = 0; i < numOptions; i++)
	{
		int textAreaWidth = (options[i].coordinates.w + (TEXT_HORIZONTAL_BUFFER * 2));
		int textAreaHeight = options[i].coordinates.h + (TEXT_VERTICAL_BUFFER * 2);
		
		if (x >= options[i].coordinates.x + coordinates.x &&
			x <= options[i].coordinates.x + coordinates.x + textAreaWidth &&
			y >= options[i].coordinates.y + coordinates.y &&
			y <= options[i].coordinates.y + coordinates.y + textAreaHeight)
		{
			return options[i].name;
		}
	}
	return "";
}