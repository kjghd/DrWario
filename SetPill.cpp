#include "SetPill.h"

SetPill::SetPill(ID2D1Bitmap* pBitmap, int color_A, int color_B, int loc_A, int loc_B)
	:
	sideA(new PillDot(pBitmap,  7.f, loc_A, color_A)),
	sideB(new PillDot(pBitmap, 14.f, loc_B, color_B))
{
}
