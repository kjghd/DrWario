#include "PillPlayer.h"

PillPlayer::PillPlayer(ID2D1Bitmap* pBmp, int colorA, int colorB)
	:
	m_pillDotA(new PillDot(pBmp, 7.f, 3, colorA)),
	m_pillDotB(new PillDot(pBmp, 14.f, 4, colorB)),
	m_baseLocation(3),
	m_rotation(PILL_ROT_R),
	debugMove(false),
	debugHit(false),
	debugState(false)
{
	DebugMessage(debugState, L"Created PillPlayer.\n");
}
PillPlayer::~PillPlayer()
{
	DebugMessage(debugState, L"Destroyed PillPlayer.\n");
}

// collision detection
bool PillPlayer::CheckLeftHit(std::vector <PillDot*> vRange)
{
	for (size_t i = 0; i < vRange.size(); i++)
	{
		if (m_pillDotA->m_bmpLoc - 1 == vRange[i]->m_bmpLoc ||
			m_pillDotB->m_bmpLoc - 1 == vRange[i]->m_bmpLoc)
		{
			DebugMessage(debugHit, L"PillPlayer hit left.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckRightHit(std::vector <PillDot*> vRange)
{
	for (size_t i = 0; i < vRange.size(); i++)
	{
		if (m_pillDotA->m_bmpLoc + 1 == vRange[i]->m_bmpLoc ||
			m_pillDotB->m_bmpLoc + 1 == vRange[i]->m_bmpLoc)
		{
			DebugMessage(debugHit, L"PillPlayer hit right.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckRightBaseHit(std::vector <PillDot*> vRange)
{
	for (size_t i = 0; i < vRange.size(); i++)
	{
		if (m_baseLocation + 1 == vRange[i]->m_bmpLoc)
		{
			DebugMessage(debugHit, L"PillPlayer base location hit right.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckDownHit(std::vector <PillDot*> vRange)
{
	for (size_t i = 0; i < vRange.size(); i++)
	{
		if (m_pillDotA->m_bmpLoc + 8 == vRange[i]->m_bmpLoc ||
			m_pillDotB->m_bmpLoc + 8 == vRange[i]->m_bmpLoc)
		{
			DebugMessage(debugHit, L"PillPlayer hit below.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckUpBaseHit(std::vector<PillDot*> vRange)
{
	for (size_t i = 0; i < vRange.size(); i++)
	{
		if (m_baseLocation - 8 == vRange[i]->m_bmpLoc)
		{
			DebugMessage(debugHit, L"PillPlayer base hit above.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckLeftBounds()
{
	for (size_t i = 0; i < 13; i++)
	{
		if (m_pillDotA->m_bmpLoc == i * 8 || m_pillDotB->m_bmpLoc == i * 8)
		{
			DebugMessage(debugHit, L"PillPlayer hit left bounds.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckRightBounds()
{
	for (size_t i = 0; i < 13; i++)
	{
		if (m_pillDotA->m_bmpLoc == i * 8 + 7 || m_pillDotB->m_bmpLoc == i * 8 + 7)
		{
			DebugMessage(debugHit, L"PillPlayer hit right bounds.\n");
			return true;
		}
	}
	return false;
}
bool PillPlayer::CheckDownBounds()
{
	if (m_pillDotA->m_bmpLoc >= 96 || m_pillDotB->m_bmpLoc >= 96)
	{
		DebugMessage(debugHit, L"PillPlayer hit bottom bounds.\n");
		return true;
	}
	else
	{
		return false;
	}
}

// movement
void PillPlayer::MoveLeft()
{
	DebugMessage(debugMove, L"PillPlayer moved left.\n");
	m_pillDotA->m_bmpLoc -= 1;
	m_pillDotB->m_bmpLoc -= 1;
	m_baseLocation -= 1;
}
void PillPlayer::MoveRight()
{
	DebugMessage(debugMove, L"PillPlayer moved right.\n");
	m_pillDotA->m_bmpLoc += 1;
	m_pillDotB->m_bmpLoc += 1;
	m_baseLocation += 1;
}
void PillPlayer::MoveDown()
{
	DebugMessage(debugMove, L"PillPlayer moved down.\n");
	m_pillDotA->m_bmpLoc += 8;
	m_pillDotB->m_bmpLoc += 8;
	m_baseLocation += 8;
}
void PillPlayer::SetToUp()
{
	DebugMessage(debugMove, L"PillPlayer is pointing up.\n");
	m_rotation = PILL_ROT_U;
	m_pillDotA->m_bmpLoc = m_baseLocation;
	m_pillDotA->m_bmpTile.left = 21.f;
	m_pillDotA->m_bmpTile.right = 28.f;
	m_pillDotB->m_bmpLoc = m_baseLocation - 8;
	m_pillDotB->m_bmpTile.left = 28.f;
	m_pillDotB->m_bmpTile.right = 35.f;
}
void PillPlayer::SetToDown()
{
	DebugMessage(debugMove, L"PillPlayer is pointing down.\n");
	m_rotation = PILL_ROT_D;
	m_pillDotA->m_bmpLoc = m_baseLocation - 8;
	m_pillDotA->m_bmpTile.left = 28.f;
	m_pillDotA->m_bmpTile.right = 35.f;
	m_pillDotB->m_bmpLoc = m_baseLocation;
	m_pillDotB->m_bmpTile.left = 21.f;
	m_pillDotB->m_bmpTile.right = 28.f;
}
void PillPlayer::SetToLeft()
{
	DebugMessage(debugMove, L"PillPlayer is pointing left.\n");
	m_rotation = PILL_ROT_L;
	m_pillDotA->m_bmpLoc = m_baseLocation + 1;
	m_pillDotA->m_bmpTile.left = 14.f;
	m_pillDotA->m_bmpTile.right = 21.f;
	m_pillDotB->m_bmpLoc = m_baseLocation;
	m_pillDotB->m_bmpTile.left = 7.f;
	m_pillDotB->m_bmpTile.right = 14.f;
}
void PillPlayer::SetToRight()
{
	DebugMessage(debugMove, L"PillPlayer is pointing right.\n");
	m_rotation = PILL_ROT_R;
	m_pillDotA->m_bmpLoc = m_baseLocation;
	m_pillDotA->m_bmpTile.left = 7.f;
	m_pillDotA->m_bmpTile.right = 14.f;
	m_pillDotB->m_bmpLoc = m_baseLocation + 1;
	m_pillDotB->m_bmpTile.left = 14.f;
	m_pillDotB->m_bmpTile.right = 21.f;
}


bool PillPlayer::IncrementY(std::vector <PillDot*> vRange)
{
	DebugMessage(debugState, L"PillPlayer Incrementing Y axis.\n");
	if (CheckDownBounds() || CheckDownHit(vRange))
	{
		return false;
	}
	else
	{
		MoveDown();
		return true;
	}
}

// debugging
void PillPlayer::DebugMessage(bool condition, LPCWSTR message)
{
	if (condition)
	{
		OutputDebugString(message);
	}
}