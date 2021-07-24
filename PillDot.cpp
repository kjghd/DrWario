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

// collision detection
bool PillDot::CheckDownHit(std::vector <PillDot*> vRange)
{
	if (m_friend)
	{
		for (size_t i = 0; i < vRange.size(); i++)
		{
			if (vRange[i]->m_bmpLoc != m_friend->m_bmpLoc &&
				m_bmpLoc + 8 == vRange[i]->m_bmpLoc)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		for (size_t i = 0; i < vRange.size(); i++)
		{
			if (m_bmpLoc + 8 == vRange[i]->m_bmpLoc)
			{
				return true;
			}
		}
		return false;
	}
}
bool PillDot::CheckDownBounds()
{
	if (m_bmpLoc >= 96)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// movement
void PillDot::MoveDown()
{
	m_bmpLoc += 8;
}
