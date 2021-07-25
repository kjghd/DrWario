#pragma once
#include "Graphics.h"
#include "PillDot.h"
#include "PillPlayer.h"
#include "DIPs.h"
#include "SafeRelease.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>

enum PillColor
{
	PILL_COLOR_NULL,
	PILL_COLOR_BLU,
	PILL_COLOR_RED,
	PILL_COLOR_YEL
};
enum PlayState
{
	PLAY_SERVE,
	PLAY_AIM,
	PLAY_CHECK
};
enum GameState
{
	GAME_PLAY_INIT,
	GAME_PLAY,
	GAME_WIN,
	GAME_LOSE,
	GAME_PAUSE,
	GAME_MENU
};
//
int pauseOption;

// game objects and variables
int game_state;
std::vector <PillDot*> vPillDot;
PillPlayer* oPillPlayer;
ID2D1Bitmap* pBmp_bg;
ID2D1Bitmap* pBmp_bottle;
ID2D1Bitmap* pBmp_bottleWon;
ID2D1Bitmap* pBmp_grid;
ID2D1Bitmap* pBmp_gridItem;
ID2D1Bitmap* pBmp_pause;
ID2D1Bitmap* pBmp_pauseCircle;
float time_ms;
float timeElapsed_ms;
int play_state;
D2D1_RECT_F grid[104];

std::vector<int> vToFall;
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
	oPillPlayer = new PillPlayer(pBmp_gridItem, rand() % 3 + 1, rand() % 3 + 1);

	OutputDebugString(L"Serving\n");
}
bool Aim()
{
	if (oPillPlayer->IncrementY(vPillDot))
	{
		return true;
	}
	else
	{
		return false;
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
	/*
	OutputDebugString(L"Order:\n");
	for (size_t i = 0; i < vPillDot.size(); i++)
	{
		OutputDebugString(std::to_wstring(vPillDot[i]->m_bmpLoc).c_str());
		OutputDebugString(L", ");
	}
	OutputDebugString(L"\n");
	*/

	// check
	int current;
	int count;
	bool onEdge;
	matches.clear();
	for (size_t i = 0; i < vPillDot.size(); i++)
	{
		onEdge = false;
		for (size_t ii = 0; ii < 13; ii++)
		{
			if (vPillDot[i]->m_bmpLoc == ii * 8 + 7)
			{
				onEdge = true;
			}
		}
		if (!onEdge)
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
		}

		onEdge = false;
		if (i >= 96)
		{
			onEdge = true;
		}
		if (!onEdge)
		{
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

	// sort matches big to small for destroying list without reshuffling idexes
	std::sort(matches.begin(), matches.end(), SortMatches);

	// debug matches
	/*
	OutputDebugString(L"Matches:\n");
	for (size_t i = 0; i < matches.size(); i++)
	{
		OutputDebugString(std::to_wstring(matches[i]).c_str());
		OutputDebugString(L", ");
	}
	OutputDebugString(L"\n");
	*/

	// set match image
	for (size_t i = 0; i < matches.size(); i++)
	{
		vPillDot[matches[i]]->m_bmpTile.left = 35.f;
		vPillDot[matches[i]]->m_bmpTile.right = 42.f;
	}

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
	matches.clear();
}
int CheckVirusAmount()
{
	int amount{ 0 };
	for (size_t i = 0; i < vPillDot.size(); i++)
	{
		if (vPillDot[i]->m_still == true)
		{
			amount++;
		}
	}
	return amount;
}
bool CanFall()
{
	vToFall.clear();
	bool canFall = false;
	if (!vPillDot.empty())
	{
		for (size_t i = 0; i < vPillDot.size(); i++)
		{
			if (vPillDot[i]->m_friend)
			{
				if (!vPillDot[i]->m_still &&
					!vPillDot[i]->CheckDownBounds() && !vPillDot[i]->CheckDownHit(vPillDot) &&
					!vPillDot[i]->m_friend->CheckDownBounds() && !vPillDot[i]->m_friend->CheckDownHit(vPillDot))
				{
					vToFall.push_back(i);
					canFall = true;
				}
			}
			else
			{
				if (!vPillDot[i]->m_still &&
					!vPillDot[i]->CheckDownBounds() && !vPillDot[i]->CheckDownHit(vPillDot))
				{
					vToFall.push_back(i);
					canFall = true;
				}
			}
		}
	}
	return canFall;
}
void Fall()
{
	if (!vPillDot.empty())
	{
		for (size_t i = 0; i < vToFall.size(); i++)
		{
			vPillDot[vToFall[i]]->MoveDown();
		}
	}
}
void PlaceViruses(int amount)
{

	// assign location
	std::vector<int> takenSquares;
	while (takenSquares.size() != amount)
	{
		bool canAdd = true;
		int x = rand() % 80 + 24;
		for (size_t i = 0; i < takenSquares.size(); i++)
		{
			if (x == takenSquares[i])
			{
				canAdd = false;
			}
		}
		if (canAdd)
		{
			takenSquares.push_back(x);
		}
	}

	bool goodPlacement{ false };
	while (!goodPlacement)
	{
		// assign colour & fill array
		for (size_t i = 0; i < amount; i++)
		{
			vPillDot.push_back(new PillDot(pBmp_gridItem, 42.f, takenSquares[i], rand() % 3 + 1, true));
		}

		// determine if good placment
		if (CheckForMatches())
		{
			vPillDot.clear();
			goodPlacement = false;
		}
		else
		{
			goodPlacement = true;
		}
	}

}

// Main Game functions
void GameInit(HWND* phWnd)
{
	game_state = GAME_PLAY_INIT;
	graphics->Init(phWnd);
	// create bitmaps
	graphics->CreateBitmap(L"textures/test/checkers.png", &pBmp_bg);
	graphics->CreateBitmap(L"textures/test/grid_square.png", &pBmp_grid);
	graphics->CreateBitmap(L"textures/test/bottle.png", &pBmp_bottle);
	graphics->CreateBitmap(L"textures/test/bottle_won.png", &pBmp_bottleWon);
	graphics->CreateBitmap(L"textures/test/pills.png", &pBmp_gridItem);
	graphics->CreateBitmap(L"textures/test/pause.png", &pBmp_pause);
	graphics->CreateBitmap(L"textures/test/pause_circle.png", &pBmp_pauseCircle);
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
	timeElapsed_ms = 0;



	pauseOption = 0;
}

void GameUpdate(float deltaTime)
{
	switch (game_state)
	{
	case GAME_PLAY_INIT:
	{
		vPillDot.clear();

		srand((int)(deltaTime * 1000.f));
		PlaceViruses(1); // 80 at most
		game_state = GAME_PLAY;
		play_state = PLAY_SERVE;
		break;
	}
	case GAME_PLAY:
	{
		// pause
		if (input->ButtonPressed(BUTTON_SPACE))
			{
				game_state = GAME_PAUSE;
			}
		// logic
		timeElapsed_ms += deltaTime;
		time_ms += deltaTime;
		if (play_state == PLAY_SERVE && time_ms > 1.f)
			{
				time_ms = 0;
				Serve();
				play_state = PLAY_AIM;
			}
		if (play_state == PLAY_AIM)
			{
				oPillPlayer->HandleInput(vPillDot, timeElapsed_ms);
				if (time_ms > 1.f)
				{
					time_ms = 0;
					if (!Aim())
					{
						Land();
						play_state = PLAY_CHECK;
					}
				}

			}
		if (play_state == PLAY_CHECK && time_ms > 0.5f)
			{
				time_ms = 0;
				if (matches.empty())
				{
					if (CanFall())
					{
						Fall();
					}
					else if (!CheckForMatches())
					{
						play_state = PLAY_SERVE;
					}
				}
				else
				{
					DestroyMatches();
				}
				if (CheckVirusAmount() == 0)
				{
					game_state = GAME_WIN;
				}
			}
	}
	case GAME_WIN:
	{
		if (input->ButtonPressed(BUTTON_ENTER))
		{
			game_state = GAME_PLAY_INIT;
		}
		break;
	}
	case GAME_PAUSE:
	{
		// change selection
		if (pauseOption == 0 && input->ButtonPressed(BUTTON_D)) // select exit
			{
				pauseOption = 1;
			}
		else if (pauseOption == 1 && input->ButtonPressed(BUTTON_A)) // select continue
			{
				pauseOption = 0;
			}

		// activate selection
		if (pauseOption == 0 && input->ButtonPressed(BUTTON_ENTER)) // activate continue
			{
				game_state = GAME_PLAY;
			}
		else if (pauseOption == 1 && input->ButtonPressed(BUTTON_ENTER)) // activate exit
			{
				PostQuitMessage(0);
			}

		// un-pause
		if (input->ButtonPressed(BUTTON_SPACE))
			{
				game_state = GAME_PLAY;
			}
	}
	}

}

void GameRender(HWND* pHwnd)
{
	graphics->BeginDraw();
	graphics->ClearScreen();
	switch (game_state)
	{
	case GAME_PLAY:
	{
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
				grid[oPillPlayer->m_pillDotA->m_bmpLoc].top + 1.f,
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
		break;
	}
	case GAME_WIN:
	{
		graphics->DrawBitmap(&pBmp_bg, D2D1::RectF(0, 0, 240.f, 160.f));
		graphics->DrawBitmap(&pBmp_bottleWon, D2D1::RectF(80.f, 8.f, 160.f, 160.f));
		break;
	}
	case GAME_LOSE:
	{
		break;
	}
	case GAME_PAUSE:
	{
		graphics->DrawBitmap(&pBmp_pause, D2D1::RectF(0, 0, 240.f, 160.f));
		if (pauseOption == 0)
		{
			graphics->DrawBitmap(&pBmp_pauseCircle, D2D1::RectF(45.f, 76.f, 45.f + 69.f, 76.f + 39.f));
		}
		else if (pauseOption == 1)
		{
			graphics->DrawBitmap(&pBmp_pauseCircle, D2D1::RectF(118.f, 76.f, 118.f + 69.f, 76.f + 39.f));
		}
		break;
	}
	}


	graphics->EndDraw();
}

void GameRelease()
{
	SafeRelease(&pBmp_grid);
	SafeRelease(&pBmp_bg);
	SafeRelease(&pBmp_bottle);
}