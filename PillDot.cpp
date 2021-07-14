#include "PillDot.h"
#include "SafeRelease.h"

PillDot::PillDot(ID2D1Bitmap* pBitmap, float tileX, int loc, int color)
	:
	m_pBitmap(pBitmap),
	m_bmpTile({tileX, (float)((color - 1) * 7), tileX + 7.f, (float)(color * 7) }),
	m_bmpLoc(loc),
	m_color(color)
{
}

PillDot::~PillDot()
{
	SafeRelease(&m_pBitmap);
}