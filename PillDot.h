#pragma once
#include <d2d1.h>
#include <vector>

class PillDot
{
public:
	ID2D1Bitmap* m_pBitmap;
	D2D1_RECT_F m_bmpTile;
	PillDot* m_friend;
	int m_bmpLoc;
	int m_color;
	bool m_still;

	bool CheckDownHit(std::vector <PillDot*> vRange);
	bool CheckDownBounds();
	bool CheckRightBounds();

	void MoveDown();

	PillDot(ID2D1Bitmap* pBitmap, float tileX, int loc, int color, bool still = false);
	~PillDot();
};

