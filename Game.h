#pragma once
#include "Graphics.h"
#include "PillDot.h"
#include "PillPlayer.h"
#include "DIPs.h"
#include "SafeRelease.h"
#include <sstream>
#include <vector>
#include <algorithm>

enum PillColor
{
	PILL_COLOR_NULL,
	PILL_COLOR_BLU,
	PILL_COLOR_RED,
	PILL_COLOR_YEL
};

// game objects and variables
std::vector <PillDot*> vPillDot;
PillPlayer* oPillPlayer;
ID2D1Bitmap* pBmp_bg;
ID2D1Bitmap* pBmp_bottle;
ID2D1Bitmap* pBmp_grid;
ID2D1Bitmap* pBmp_gridItem;
float time_ms;
D2D1_RECT_F grid[104];

std::vector <int> matches; // matching pill indexes
bool SortPillDots(PillDot* i, PillDot* j)
{
	return i->m_bmpLoc < j->m_bmpLoc;
}
bool SortMatches(int i, int j)
{
	return i > j;
}

// Game functions
void Serve()
{
	oPillPlayer = new PillPlayer(pBmp_gridItem, PILL_COLOR_BLU, PILL_COLOR_RED);

	OutputDebugString(L"Serving\n");
}
bool Aim()
{
	oPillPlayer->HandleInput(vPillDot); // when function definition is in cpp, sets input->m_button[3] to (false, false)
	if (time_ms > 1.f) // input->m_button[3] reverts back to (true, true).
	{
		time_ms = 0;
		if (!oPillPlayer->IncrementY(vPillDot))
		{
			return false;
		}
		return true;
	}
}
void Land()
{
	// add to list
	vPillDot.push_back(oPillPlayer->m_pillDotA);
	vPillDot.push_back(oPillPlayer->m_pillDotB);
	// second last (A) friend = last (B)
	vPillDot[vPillDot.size() - 2]->m_friend = vPillDot[vPillDot.size() - 1];
	// last (B) friend = second last (A)
	vPillDot[vPillDot.size() - 1]->m_friend = vPillDot[vPillDot.size() - 2];

	delete oPillPlayer;
	oPillPlayer = nullptr;

	OutputDebugString(L"Landed\n");
}
bool CheckForMatches()
{
	// sort
	std::sort(vPillDot.begin(), vPillDot.end(), SortPillDots);

	// debug total pill dots
	OutputDebugString(L"Order:\n");
	for (size_t i = 0; i < vPillDot.size(); i++)
	{
		OutputDebugString(std::to_wstring(vPillDot[i]->m_bmpLoc).c_str());
		OutputDebugString(L", ");
	}
	OutputDebugString(L"\n");

	// check
	int current;
	int count;
	matches.clear();
	for (size_t i = 0; i < vPillDot.size(); i++)
	{
		// X
		matches.push_back(i);
		current = i;
		count = 1;
		for (size_t ii = 0; ii < vPillDot.size(); ii++)
		{
			if (i != ii &&
				vPillDot[current]->m_bmpLoc + 1 == vPillDot[ii]->m_bmpLoc &&
				vPillDot[current]->m_color == vPillDot[ii]->m_color) // check to right
			{
				matches.push_back(ii); // store
				current = ii; // retarget
				ii = 0; // reset
				count++;
			}
		}
		if (count < 4)
		{
			for (size_t i = 0; i < count; i++)
			{
				matches.pop_back();
			}
		}

		// Y
		matches.push_back(i);
		current = i;
		count = 1;
		for (size_t ii = 0; ii < vPillDot.size(); ii++)
		{
			if (i != ii &&
				vPillDot[current]->m_bmpLoc + 8 == vPillDot[ii]->m_bmpLoc &&
				vPillDot[current]->m_color == vPillDot[ii]->m_color) // check to right
			{
				matches.push_back(ii); // store
				current = ii; // retarget
				ii = 0; // reset
				count++;
			}
		}
		if (count < 4)
		{
			for (size_t i = 0; i < count; i++)
			{
				matches.pop_back();
			}
		}
	}

	// remove doubles
	for (size_t i = 0; i < matches.size(); i++)
	{
		for (size_t ii = 0; ii < matches.size(); ii++)
		{
			if (i != ii && matches[i] == matches[ii])
			{
				matches.erase(matches.begin() + ii);
				i = 0;
				ii = 0;
			}
		}
	}

	std::sort(matches.begin(), matches.end(), SortMatches); // sort matches big to small for destroying list without reshuffling idexes

	// debug matches
	OutputDebugString(L"Matches:\n");
	for (size_t i = 0; i < matches.size(); i++)
	{
		OutputDebugString(std::to_wstring(matches[i]).c_str());
		OutputDebugString(L", ");
	}
	OutputDebugString(L"\n");

	if (matches.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}
void DestroyMatches()
{
	for (size_t i = 0; i < matches.size(); i++)
	{
		if (vPillDot[matches[i]]->m_friend)
		{
			vPillDot[matches[i]]->m_friend->m_friend = nullptr;
			vPillDot[matches[i]]->m_friend->m_bmpTile.left = 0;
			vPillDot[matches[i]]->m_friend->m_bmpTile.right = 7.f;
		}
		vPillDot.erase(vPillDot.begin() + matches[i]);
	}
}
void Fall()
{
	// maybe not detecting friends when falling
	if (!vPillDot.empty())
	{
		bool stillRoom{ true };
		while (stillRoom)
		{
			stillRoom = false;
			for (size_t i = 0; i < vPillDot.size(); i++)
			{
				if (vPillDot[i]->m_friend)
				{
					if (!vPillDot[i]->CheckDownBounds() && !vPillDot[i]->CheckDownHit(vPillDot) ||
						!vPillDot[i]->m_friend->CheckDownBounds() && !vPillDot[i]->m_friend->CheckDownHit(vPillDot))
					{
						vPillDot[i]->MoveDown();
						vPillDot[i]->m_friend->MoveDown();
						stillRoom = true;
					}
				}
				else
				{
					if (!vPillDot[i]->CheckDownBounds() && !vPillDot[i]->CheckDownHit(vPillDot))
					{
						vPillDot[i]->MoveDown();
						stillRoom = true;
					}
				}
			}
		}
	}
}

// Main Game functions
void GameInit(HWND* phWnd)
{
	graphics->Init(phWnd);
	// create bitmaps
	graphics->CreateBitmap(L"textures/test/checkers.png", &pBmp_bg);
	graphics->CreateBitmap(L"textures/test/grid_square.png", &pBmp_grid);
	graphics->CreateBitmap(L"textures/test/bottle.png", &pBmp_bottle);
	graphics->CreateBitmap(L"textures/test/pills.png", &pBmp_gridItem);
	// calculate grid
	for (size_t i = 0; i < 104; i++)
	{
		float top    = 48.f +  (i      / 8 * 8.f);
		float bottom = 48.f + ((i + 8) / 8 * 8.f);
		float left   = 88.f +  (i          * 8.f) - i / 8 * 64.f;
		float right  = 88.f + ((i + 1)     * 8.f) - i / 8 * 64.f;
					 
		grid[i] = D2D1::RectF(
			left,
			top,
			right,
			bottom
		);
	}
	time_ms = 0;
}

void GameUpdate(float deltaTime)
{
	time_ms += deltaTime;

	if (oPillPlayer)
	{
		if (!Aim())
		{
			Land();
			bool possibleMatches{ true };
			while (possibleMatches)
			{
				if (CheckForMatches())
				{
					DestroyMatches();
					Fall();
				}
				else
				{
					possibleMatches = false;
				}
			}

		}
	}
	else
	{
		Serve();
	}
}

void GameRender(HWND* pHwnd)
{
	graphics->BeginDraw();
	graphics->ClearScreen();

	graphics->DrawBitmap(&pBmp_bg, D2D1::RectF(0, 0, 240.f, 160.f));
	graphics->DrawBitmap(&pBmp_bottle, D2D1::RectF(80.f, 8.f, 160.f, 160.f));

	// grid debug
	for (size_t i = 0; i < 104; i++)
	{
		if (true)
		{
			graphics->DrawBitmap(&pBmp_grid, grid[i]);
		}
	}
	// PillDots
	if (!vPillDot.empty())
	{
		for (size_t i = 0; i < vPillDot.size(); i++)
		{
			D2D1_RECT_F rcf = {
				grid[vPillDot[i]->m_bmpLoc].left        ,
				grid[vPillDot[i]->m_bmpLoc].top         ,
				grid[vPillDot[i]->m_bmpLoc].right - 1.f,
				grid[vPillDot[i]->m_bmpLoc].bottom - 1.f
			};
			graphics->DrawBitmapTile(&pBmp_gridItem, rcf, vPillDot[i]->m_bmpTile);
			graphics->DrawBitmapTile(&pBmp_gridItem, rcf, vPillDot[i]->m_bmpTile);
		}
	}
	// PillPlayer
	if (oPillPlayer)
	{
		D2D1_RECT_F rcfA = {
			grid[oPillPlayer->m_pillDotA->m_bmpLoc].left       ,
			grid[oPillPlayer->m_pillDotA->m_bmpLoc].top   + 1.f,
			grid[oPillPlayer->m_pillDotA->m_bmpLoc].right - 1.f,
			grid[oPillPlayer->m_pillDotA->m_bmpLoc].bottom
		};
		D2D1_RECT_F rcfB = {
			grid[oPillPlayer->m_pillDotB->m_bmpLoc].left       ,
			grid[oPillPlayer->m_pillDotB->m_bmpLoc].top + 1.f,
			grid[oPillPlayer->m_pillDotB->m_bmpLoc].right - 1.f,
			grid[oPillPlayer->m_pillDotB->m_bmpLoc].bottom
		};
		graphics->DrawBitmapTile(&oPillPlayer->m_pillDotA->m_pBitmap, rcfA, oPillPlayer->m_pillDotA->m_bmpTile);
		graphics->DrawBitmapTile(&oPillPlayer->m_pillDotB->m_pBitmap, rcfB, oPillPlayer->m_pillDotB->m_bmpTile);
	}

	graphics->EndDraw();
}

void GameRelease()
{
	SafeRelease(&pBmp_grid);
	SafeRelease(&pBmp_bg);
	SafeRelease(&pBmp_bottle);
}