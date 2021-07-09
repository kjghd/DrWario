#pragma once
#include <d2d1.h>

enum PillColor
{
	PILLCOLOR_EMPTY,
	PILLCOLOR_BLUE,
	PILLCOLOR_RED,
	PILLCOLOR_YELLOW
};

enum PillState
{
	PILL_RIGHT,
	PILL_DOWN,
	PILL_LEFT,
	PILL_UP
};

class ActivePill
{
public:
	ID2D1Bitmap* m_pBitmap;
	D2D1_RECT_F m_bmpTileA;
	D2D1_RECT_F m_bmpTileB;
	int m_bmpLocA;
	int m_bmpLocB;
	int m_rotation;
	int m_colorA;
	int m_colorB;

	void Rotate();

	ActivePill();
	~ActivePill();

};

