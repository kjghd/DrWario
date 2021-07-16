#pragma once
#include <d2d1.h>
#include "PillDot.h"

class SetPill
{
public:
	PillDot* sideA;
	PillDot* sideB;

	SetPill(ID2D1Bitmap* pBitmap, int color_A, int color_B, int loc_A, int loc_B);

};

