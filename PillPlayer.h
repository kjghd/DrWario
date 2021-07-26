#pragma once
#include "PillDot.h"
#include "Input.h"
#include <vector>

enum PillRotation
{
	PILL_ROT_L,
	PILL_ROT_U,
	PILL_ROT_R,
	PILL_ROT_D,
};

class PillPlayer
{
public:
	PillDot* m_pillDotA;
	PillDot* m_pillDotB;
	int m_baseLocation;
	int m_rotation;

private:
	// collision detection
	bool CheckLeftHit(std::vector <PillDot*> vRange);
	bool CheckRightHit(std::vector <PillDot*> vRange);
	bool CheckRightBaseHit(std::vector <PillDot*> vRange);
	bool CheckDownHit(std::vector <PillDot*> vRange);
	bool CheckUpBaseHit(std::vector<PillDot*> vRange);
	bool CheckLeftBounds();
	bool CheckRightBounds();
	bool CheckDownBounds();

	// movement
	void MoveLeft();
	void MoveRight();
	void MoveDown();
	void SetToUp();
	void SetToDown();
	void SetToLeft();
	void SetToRight();
	
public:
	void HandleInput(std::vector <PillDot*> vRange)
	{
		// clockwise
		if (input->ButtonPressed(BUTTON_E))
		{
			switch (m_rotation)
			{
			case PILL_ROT_L:
				if (!CheckUpBaseHit(vRange))
				{
					SetToUp();
				}
				break;

			case PILL_ROT_U: // if up set to right
				if (CheckRightBounds() || CheckRightBaseHit(vRange))
				{
					if (!CheckLeftBounds() && !CheckLeftHit(vRange))
					{
						MoveLeft();
						SetToRight();
					}
				}
				else
				{
					SetToRight();
				}
				break;

			case PILL_ROT_R:
				if (!CheckUpBaseHit(vRange))
				{
					SetToDown();
				}
				break;

			case PILL_ROT_D:
				if (CheckRightBounds() || CheckRightBaseHit(vRange))
				{
					if (!CheckLeftBounds() && !CheckLeftHit(vRange))
					{
						MoveLeft();
						SetToLeft();
					}
				}
				else
				{
					SetToLeft();
				}
				break;
			}
		}
		// anti clockwise
		if (input->ButtonPressed(BUTTON_Q))
		{
			switch (m_rotation)
			{
			case PILL_ROT_L:
				if (!CheckUpBaseHit(vRange))
				{
					SetToDown();
				}
				break;

			case PILL_ROT_U:
				if (CheckRightBounds() || CheckRightBaseHit(vRange))
				{
					if (!CheckLeftBounds() && !CheckLeftHit(vRange))
					{
						MoveLeft();
						SetToLeft();
					}
				}
				else
				{
					SetToLeft();
				}
				break;

			case PILL_ROT_R:
				if (!CheckUpBaseHit(vRange))
				{
					SetToUp();
				}
				break;

			case PILL_ROT_D:
				if (CheckRightBounds() || CheckRightBaseHit(vRange))
				{
					if (!CheckLeftBounds() && !CheckLeftHit(vRange))
					{
						MoveLeft();
						SetToRight();
					}
				}
				else
				{
					SetToRight();
				}
				break; 
			}
		}
		// move left
		if (input->ButtonPressed(BUTTON_A) && !CheckLeftBounds() && !CheckLeftHit(vRange))
		{
			MoveLeft();
		}
		// move right
		if (input->ButtonPressed(BUTTON_D) && !CheckRightBounds() && !CheckRightHit(vRange))
		{
			MoveRight();
		}
	}
	bool IncrementY(std::vector <PillDot*> vRange);

	PillPlayer(ID2D1Bitmap* pBmp, int colorA, int colorB);
	~PillPlayer();

private:
	// debugging
	bool debugMove;
	bool debugHit;
	bool debugState;
	void DebugMessage(bool condition, LPCWSTR message);
};
