#include "ActivePill.h"

ActivePill::ActivePill(ID2D1Bitmap* pBitmap, int color_A, int color_B)
	:
	sideA(new PillDot(pBitmap,  7.f, 0, color_A)),
	sideB(new PillDot(pBitmap, 14.f, 1, color_B)),
	m_rotation(PILL_RIGHT),
	m_base(0)
{
}

ActivePill::~ActivePill()
{
}

void ActivePill::SetToUp()
{
	m_rotation = PILL_UP;
	sideA->m_bmpLoc = m_base;
	sideA->m_bmpTile.left  = 21.f;
	sideA->m_bmpTile.right = 28.f;
	sideB->m_bmpLoc = m_base - 8;
	sideB->m_bmpTile.left  = 28.f;
	sideB->m_bmpTile.right = 35.f;
}

void ActivePill::SetToDown()
{
	m_rotation = PILL_DOWN;
	sideA->m_bmpLoc = m_base - 8;
	sideA->m_bmpTile.left  = 28.f;
	sideA->m_bmpTile.right = 35.f;
	sideB->m_bmpLoc = m_base;
	sideB->m_bmpTile.left  = 21.f;
	sideB->m_bmpTile.right = 28.f;
}

void ActivePill::SetToLeft()
{
	for (size_t i = 0; i < 14; i++) // check each row of grid
	{
		if (m_base == i * 8 + 7)
		{
			m_base -= 1;
			sideA->m_bmpLoc -= 1;
			sideB->m_bmpLoc -= 1;
		}
	}
	m_rotation = PILL_LEFT;
	sideA->m_bmpLoc = m_base + 1;
	sideA->m_bmpTile.left  = 14.f;
	sideA->m_bmpTile.right = 21.f;
	sideB->m_bmpLoc = m_base;
	sideB->m_bmpTile.left  = 7.f;
	sideB->m_bmpTile.right = 14.f;
}

void ActivePill::SetToRight()
{
	for (size_t i = 0; i < 14; i++) // check each row of grid
	{
		if (m_base == i * 8 + 7)
		{
			m_base -= 1;
			sideA->m_bmpLoc -= 1;
			sideB->m_bmpLoc -= 1;
		}
	}
	m_rotation = PILL_RIGHT;
	sideA->m_bmpLoc = m_base;
	sideA->m_bmpTile.left  = 7.f;
	sideA->m_bmpTile.right = 14.f;
	sideB->m_bmpLoc = m_base + 1;
	sideB->m_bmpTile.left  = 14.f;
	sideB->m_bmpTile.right = 21.f;
}