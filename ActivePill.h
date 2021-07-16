#pragma once
#include <d2d1.h>
#include "PillDot.h"

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
	PillDot* sideA;
	PillDot* sideB;
	int m_rotation;
	int m_base;

	void SetToUp();
	void SetToDown();
	void SetToLeft();
	void SetToRight();

	ActivePill(ID2D1Bitmap* pBitmap, int color_A, int color_B);
	~ActivePill();
};

