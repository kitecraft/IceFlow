#include "IconBase.h"

void IconBase::Init(TFT_eSPI* inTFT, StarsideCoordinates inCoordinates)
{
	TFT = inTFT;
	coordinates = inCoordinates;
}

bool IconBase::Touched(int x, int y)
{
	if (x >= coordinates.x &&
		x <= coordinates.x + coordinates.w &&
		y >= coordinates.y &&
		y <= coordinates.y + coordinates.h)
	{
		return true;
	}
	return false;
}