#pragma once
#include <d2d1.h>

class PillDot
{
public:
	ID2D1Bitmap* m_pBitmap;
	D2D1_RECT_F m_bmpTile;
	int m_bmpLoc;
	int m_color;

	PillDot(ID2D1Bitmap* pBitmap, float tileX, int loc, int color);
	~PillDot();
};

