#pragma once
#include <Arduino.h>

enum DISPLAY_COMMANDS {
	DISPLAY_COMMAND_NO_COMMAND = -1,
	DISPLAY_COMMAND_CHANGE_SCREEN,
	DISPLAY_COMMAND_UPDATE_VALUE,
};