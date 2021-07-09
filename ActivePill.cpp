#include "ActivePill.h"
#include "SafeRelease.h"

ActivePill::ActivePill()
	:
	m_pBitmap(NULL),
	m_bmpTileA(D2D1::RectF( 7.f, 0,   14.f,  7.f)),
	m_bmpTileB(D2D1::RectF(14.f, 7.f, 21.f, 14.f)),
	m_bmpLocA(3),
	m_bmpLocB(4),
	m_colorA(PILLCOLOR_BLUE),
	m_colorB(PILLCOLOR_RED),
	m_rotation(PILL_RIGHT)
{
}

ActivePill::~ActivePill()
{
	SafeRelease(&m_pBitmap);
}

void ActivePill::Rotate()
{

}