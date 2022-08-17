#include "ManualHeatIcon.h"

ManualHeatIcon::ManualHeatIcon()
	: IconBase()
{

}

ManualHeatIcon::ManualHeatIcon(IconBaseDto config, TFT_eSPI* tft)
	: IconBase(config, tft)
{

}

void ManualHeatIcon::Draw(TFT_eSprite* sprite)
{
	IconBase::Draw(sprite);
	int columnX[2] = { _config.coordinates.x + 16,
						_config.coordinates.x + _config.coordinates.w - 16
	};

	int rowY[3] = { _config.coordinates.y + _config.coordinates.h - 16,
					_config.coordinates.y + _config.coordinates.h - 21,
					_config.coordinates.y + _config.coordinates.h - 26 };

	uint16_t color = _config.theme.panelDarkColor;

	sprite->drawFastHLine(_config.coordinates.x + 6, _config.coordinates.y + _config.coordinates.h - 10, _config.coordinates.w - 12, color);
	sprite->drawFastHLine(_config.coordinates.x + 6, _config.coordinates.y + _config.coordinates.h - 9, _config.coordinates.w - 12, color);
	sprite->drawFastHLine(_config.coordinates.x + 8, _config.coordinates.y + _config.coordinates.h - 8, _config.coordinates.w - 16, color);
	sprite->drawFastHLine(_config.coordinates.x + 8, _config.coordinates.y + _config.coordinates.h - 7, _config.coordinates.w - 16, color);
	sprite->drawFastHLine(_config.coordinates.x + 10, _config.coordinates.y + _config.coordinates.h - 6, _config.coordinates.w - 20, color);
	sprite->drawFastHLine(_config.coordinates.x + 10, _config.coordinates.y + _config.coordinates.h - 5, _config.coordinates.w - 20, color);

	for (int i = 0; i < 2; i++) {
		sprite->drawCircleHelper(columnX[i], rowY[0], 5, 0x1, color);
		sprite->drawCircleHelper(columnX[i], rowY[0], 6, 0x1, color);
		sprite->drawCircleHelper(columnX[i], rowY[0]-1, 5, 0x8, color);
		sprite->drawCircleHelper(columnX[i], rowY[0]-1, 6, 0x8, color);

		sprite->drawCircleHelper(columnX[i] - 1, rowY[1], 5, 0x2, color);
		sprite->drawCircleHelper(columnX[i] - 1, rowY[1], 6, 0x2, color);
		sprite->drawCircleHelper(columnX[i] - 1, rowY[1]-1, 5, 0x4, color);
		sprite->drawCircleHelper(columnX[i] - 1, rowY[1]-1, 6, 0x4, color);

		sprite->drawCircleHelper(columnX[i], rowY[2], 5, 0x1, color);
		sprite->drawCircleHelper(columnX[i], rowY[2], 6, 0x1, color);
		sprite->drawCircleHelper(columnX[i], rowY[2]-1, 5, 0x8, color);
		sprite->drawCircleHelper(columnX[i], rowY[2]-1, 6, 0x8, color);
	}


	sprite->drawFastHLine(_config.coordinates.x + 6, _config.coordinates.y + 10, _config.coordinates.w - 12, color);
	sprite->drawFastHLine(_config.coordinates.x + 6, _config.coordinates.y + 9, _config.coordinates.w - 12, color);
	sprite->drawFastHLine(_config.coordinates.x + 8, _config.coordinates.y + 8, _config.coordinates.w - 16, color);
	sprite->drawFastHLine(_config.coordinates.x + 8, _config.coordinates.y + 7, _config.coordinates.w - 16, color);
	sprite->drawFastHLine(_config.coordinates.x + 10, _config.coordinates.y + 6, _config.coordinates.w - 20, color);
	sprite->drawFastHLine(_config.coordinates.x + 10, _config.coordinates.y + 5, _config.coordinates.w - 20, color);

	if (!_enabled) {
		UpdateAsDisabled(sprite);
	}
}