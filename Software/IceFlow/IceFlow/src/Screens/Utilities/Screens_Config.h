#pragma once

//Default DMTheme settings				Bluish		Darkish
#define DM_THEME_TEXT_COLOR				0xFFFF		//0xCE59
#define DM_THEME_PANEL_DARK_COLOR	0x528a//		0x3BB6		//0x5AEB
#define DM_THEME_PANEL_LIGHT_COLOR	0xa534//	 0x7D19		//0x738E
#define DM_THEME_BOX_BORDER_COLOR	0xe73c//	0x9DDB		//0x9CD3
#define DM_THEME_BOX_DROP_SHADOW		0x63F2		//0x4208

//Small font
#include "../DMFonts/segoeui5pt7b.h"
//Medium font
#include "../DMFonts/segoeui7pt7b.h"
//Large Font
#include "../DMFonts/segoeui9pt7b.h"

#define SMALL_FONT &segoeui5pt7b
#define MEDIUM_FONT &segoeui7pt7b
#define LARGE_FONT &segoeui9pt7b

#define SMALL_FONT_TEXT_H 11
#define MEDIUM_FONT_TEXT_H 16
#define LARGE_FONT_TEXT_H 20

#define SMALL_FONT_TEXT_BOX_H 19
#define MEDIUM_FONT_TEXT_BOX_H 23
#define LARGE_FONT_TEXT_BOX_H 29

#define BUTTON_COLOR 0x1A95

//This is a negative offset
#define TEXT_Y_OFFSET 2

#define ICON_BASE_ICON_RADIUS 6

//Used when 2 sprites are needed to fill the screen
#define TOP_SIDE 0
#define BOTTOM_SIDE 1
