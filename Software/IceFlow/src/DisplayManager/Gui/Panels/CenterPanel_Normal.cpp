#include "CenterPanel_Normal.h"

CenterPanel_Normal::~CenterPanel_Normal()
{
	if (_gridSprite != nullptr) {
		delete(_gridSprite);
		_gridSprite = nullptr;
	}
}

void CenterPanel_Normal::Init(TFT_eSPI* newTFT, Profile currentProfile)
{
	PanelBase::Init(newTFT);
	_profile = currentProfile;
	_gridSprite = new TFT_eSprite(TFT);
	_gridSprite->createSprite(GRID_SPRITE_WIDTH, GRID_SPRITE_HEIGHT);

	configure();


}

void CenterPanel_Normal::configure()
{
	_maxTemp = _profile.reflow_max_temperature == 0 ? 250 : _profile.reflow_max_temperature + BG_MAXTEMP_ADD;
	_minTemp = 10;
}

void CenterPanel_Normal::ChangeProfile(Profile newProfile)
{
	_profile = newProfile;
	configure();
}

void CenterPanel_Normal::Draw()
{
	drawBackground();

}

void CenterPanel_Normal::drawBackground()
{
	TFT_eSprite sprite = TFT_eSprite(TFT);
	sprite.createSprite(CP_NORMAL_WIDTH, CP_NORMAL_HEIGHT);
	//sprite.fillRect(0, 0, CP_NORMAL_WIDTH, CP_NORMAL_HEIGHT, 0xBDF7);
	sprite.fillRect(0, 0, CP_NORMAL_WIDTH, CP_NORMAL_HEIGHT, TFT_BLACK);

	DrawRoundedBox(&sprite,
		StarsideCoordinates(CP_NORMAL_X, CP_NORMAL_Y, CP_NORMAL_X, CP_NORMAL_Y),
		g_GlobalTheme,
		CP_NORMAL_RADIUS,
		CP_NORMAL_BORDER);

	sprite.loadFont(AA_FONT_14PT);
	sprite.setTextColor(g_GlobalTheme.textColor);

	String str(_maxTemp);
	int maxTextWidth = sprite.textWidth(str) + 2;
	Serial.print("TEMPERATURE WIDTH: ");
	Serial.println(maxTextWidth);
	Serial.print("TEMPERATURE : ");
	Serial.println(_maxTemp);

	sprite.setTextDatum(TR_DATUM);
	sprite.drawNumber(_maxTemp, maxTextWidth, 2);

	sprite.setTextDatum(TL_DATUM);
	sprite.drawNumber(_maxTemp, GRID_SPRITE_WIDTH - maxTextWidth, 2);

	/*
	sprite.setTextDatum(BR_DATUM);
	sprite.drawNumber(_minTemp, maxTextWidth, CP_NORMAL_HEIGHT - 2);

	sprite.setTextDatum(BR_DATUM);
	sprite.drawNumber(_minTemp, maxTextWidth, CP_NORMAL_HEIGHT - 2);

	int midXValue = ((_maxTemp - _minTemp) / 2) + _minTemp;
	sprite.setTextDatum(MR_DATUM);
	sprite.drawNumber(midXValue, maxTextWidth, CP_NORMAL_HEIGHT / 2);

	int quarterXValue = midXValue / 2;
	int quarterX = (CP_NORMAL_HEIGHT / 2) / 2;
	sprite.setTextDatum(MR_DATUM);
	sprite.drawNumber(quarterXValue, maxTextWidth, quarterX);

	quarterXValue = quarterXValue + (midXValue / 2);
	quarterX = quarterX + (CP_NORMAL_HEIGHT / 2);
	sprite.drawNumber(quarterXValue, maxTextWidth, quarterX);

	*/
	sprite.pushSprite(CP_NORMAL_X, CP_NORMAL_Y, TFT_TRANSPARENT);
	sprite.deleteSprite();
}