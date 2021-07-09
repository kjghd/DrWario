#pragma once
#include <d2d1.h>

class SetPill
{
public:
	ID2D1Bitmap* m_pBitmap;
	D2D1_RECT_F m_bmpTileA;
	D2D1_RECT_F m_bmpTileB;
	int m_bmpLocA;
	int m_bmpLocB;
	int m_colorA;
	int m_colorB;

	SetPill();

};

