#pragma once
#include "Graphics.h"
#include "PillDot.h"
#include "PillPlayer.h"
#include "DIPs.h"
#include "SafeRelease.h"
#include "TimingValues.h"
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
int menuOption;
int menuVirus;
int menuSpeed;
int virusCount;

// game objects and variables
int game_state;
std::vector <PillDot*> vPillDot;
PillPlayer* oPillPlayer;
ID2D1Bitmap* pBmp_bg;
ID2D1Bitmap* pBmp_bottle;
ID2D1Bitmap* pBmp_bottleWon;
ID2D1Bitmap* pBmp_bottleLost;
ID2D1Bitmap* pBmp_grid;
ID2D1Bitmap* pBmp_gridItem;
ID2D1Bitmap* pBmp_pause;
ID2D1Bitmap* pBmp_pauseCircle;
ID2D1Bitmap* pBmp_menu;
ID2D1Bitmap* pBmp_arrowVirus;
ID2D1Bitmap* pBmp_arrowSpeed;
ID2D1Bitmap* pBmp_statCard;
ID2D1Bitmap* pBmp_numbers;

float time_s;
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
bool Serve()
{
	// check nothing is in the way
	for (size_t i = 0; i < vPillDot.size(); i++)
	{
		if (vPillDot[i]->m_bmpLoc == 3 ||
			vPillDot[i]->m_bmpLoc == 4)
		{
			return false;
		}
	}

	// drop a new pill
	oPillPlayer = new PillPlayer(pBmp_gridItem, rand() % 3 + 1, rand() % 3 + 1);
	OutputDebugString(L"Serving\n");
	return true;
}
bool Drop()
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
	// sort pill dots
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

	// check for matches
	int current;
	int count;
	bool onEdge;
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
				!vPillDot[current]->CheckRightBounds() &&
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

	// remove double matches
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

	// sort matches big to small for erasing a member without reshuffling the others
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
			// if the pilldot has a friend, check below both both it and it's friend
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
			// else only check below the pill
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
	game_state = GAME_MENU;
	graphics->Init(phWnd);
	// create bitmaps
	graphics->CreateBitmap(L"textures/test/checkers.png", &pBmp_bg);
	graphics->CreateBitmap(L"textures/test/grid_square.png", &pBmp_grid);
	graphics->CreateBitmap(L"textures/test/bottle.png", &pBmp_bottle);
	graphics->CreateBitmap(L"textures/test/bottle_won.png", &pBmp_bottleWon);
	graphics->CreateBitmap(L"textures/test/bottle_lose.png", &pBmp_bottleLost);
	graphics->CreateBitmap(L"textures/test/pills.png", &pBmp_gridItem);
	graphics->CreateBitmap(L"textures/test/pause.png", &pBmp_pause);
	graphics->CreateBitmap(L"textures/test/pause_circle.png", &pBmp_pauseCircle);
	graphics->CreateBitmap(L"textures/test/menu.png", &pBmp_menu);
	graphics->CreateBitmap(L"textures/test/arrow_virus.png", &pBmp_arrowVirus);
	graphics->CreateBitmap(L"textures/test/arrow_speed.png", &pBmp_arrowSpeed);
	graphics->CreateBitmap(L"textures/test/stat_card.png", &pBmp_statCard);
	graphics->CreateBitmap(L"textures/test/numbers.png", &pBmp_numbers);
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
	time_s = 0;

	pauseOption = 0;

	menuOption = 0;
	menuVirus = 0;
	menuSpeed = 0;

	virusCount = 0;
}

void GameUpdate(float deltaTime)
{
	time_s += deltaTime;

	switch (game_state)
	{
	case GAME_PLAY_INIT:
	{
		vPillDot.clear();

		srand((int)(time_s * 100.f));

		if (menuVirus * 4 + 4 <= 80)
		{
			virusCount = menuVirus * 4 + 4;
		}
		else
		{
			virusCount = 80;
		}
		PlaceViruses(virusCount);

		switch (menuSpeed)
		{
		case 0:
			timing_pillDrop = 0.6667f;
			break;
		case 1:
			timing_pillDrop = 0.4167f;
			break;
		case 2:
			timing_pillDrop = 0.2834;
			break;
		}

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
		if (play_state == PLAY_SERVE)
			{
				if (time_s > timing_pillServe)
				{
					Serve() ? play_state = PLAY_AIM : game_state = GAME_LOSE;
				}
			}
		if (play_state == PLAY_AIM)
			{
				oPillPlayer->HandleInput(vPillDot);
				float dropSpeed = input->ButtonHeld(BUTTON_S) ? timing_pillDropM : timing_pillDrop;

				if (time_s > dropSpeed)
				{
					time_s = 0;
					if (!Drop())
					{
						Land();
						play_state = PLAY_CHECK;
					}
				}

			}
		if (play_state == PLAY_CHECK)
			{
			if (matches.empty())
			{
				if (CanFall())
				{
					if (time_s > timing_pillFall)
					{
						time_s = 0;
						Fall();
						// render pills that just fell
					}
				}
				else if (!CheckForMatches())
				{
					play_state = PLAY_SERVE;
					// render matches
				}
			}
			else if (!matches.empty() && time_s > timing_pillMatch)
			{
				DestroyMatches();
				virusCount = CheckVirusAmount();
				if (virusCount <= 0)
				{
					game_state = GAME_WIN;
					// render destroyed pills
				}
			}
			}
		break;
	}
	case GAME_WIN:
	{
		if (input->ButtonPressed(BUTTON_ENTER))
		{
			game_state = GAME_PLAY_INIT;
			menuVirus++;
		}
		break;
	}
	case GAME_LOSE:
	{
		if (input->ButtonPressed(BUTTON_ENTER))
		{
			game_state = GAME_MENU;
		}
		break;
	}
	case GAME_PAUSE:
	{
		// un-pause
		if (input->ButtonPressed(BUTTON_SPACE))
		{
			game_state = GAME_PLAY;
			break;
		}

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
				game_state = GAME_MENU;
				//PostQuitMessage(0);
			}

		break;
	}
	case GAME_MENU:
	{
		// play
		if (input->ButtonPressed(BUTTON_ENTER))
		{
			game_state = GAME_PLAY_INIT;
			break;
		}

		// change option
		if (input->ButtonPressed(BUTTON_S) && menuOption == 0) // if pressed down while "viruses" is highlighted.
		{
			menuOption = 1; // "speed" now selected.
		}
		else if (input->ButtonPressed(BUTTON_W) && menuOption == 1) // if pressed up while "speed" is selected.
		{
			menuOption = 0; // "viruses" now selected.
		}

		// lessen value
		if (input->ButtonPressed(BUTTON_A))
		{
			if (menuOption == 0 && menuVirus > 0)
			{
				menuVirus--;
			}
			else if (menuOption == 1 && menuSpeed > 0)
			{
				menuSpeed--;
			}
		}
		// increase value
		else if (input->ButtonPressed(BUTTON_D))
		{
			if (menuOption == 0 && menuVirus < 9)
			{
				menuVirus++;
			}
			else if (menuOption == 1 && menuSpeed < 2)
			{
				menuSpeed++;
			}
		}

		break;
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
		graphics->DrawBitmap(&pBmp_statCard, D2D1::RectF(168.f, 88.f, 236.f, 160.f));
		graphics->DrawBitmap(&pBmp_bottle, D2D1::RectF(80.f, 8.f, 160.f, 160.f));

		// level count
		int a;
		int b;
		if (menuVirus >= 10)
		{
			a = menuVirus / 10;
			b = menuVirus - a * 10;
		}
		else
		{
			a = 0;
			b = menuVirus;
		}
		graphics->DrawBitmapTile(&pBmp_numbers, D2D1::RectF(213.f, 96.f, 218.f, 103.f), D2D1::RectF(5.f * a, 0, 5.f * (a + 1), 7.f)); // level digit 1
		graphics->DrawBitmapTile(&pBmp_numbers, D2D1::RectF(220.f, 96.f, 225.f, 103.f), D2D1::RectF(5.f * b, 0, 5.f * (b + 1), 7.f)); // level digit 2

		// virus count
		if (virusCount >= 10)
		{
			a = virusCount / 10;
			b = virusCount - a * 10;
		}
		else
		{
			a = 0;
			b = virusCount;
		}
		graphics->DrawBitmapTile(&pBmp_numbers, D2D1::RectF(213.f, 115.f, 218.f, 122.f), D2D1::RectF(5.f * a, 0, 5.f * (a + 1), 7.f)); // virus count digit 1
		graphics->DrawBitmapTile(&pBmp_numbers, D2D1::RectF(220.f, 115.f, 225.f, 122.f), D2D1::RectF(5.f * b, 0, 5.f * (b + 1), 7.f)); // virus count digit 2

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
		graphics->DrawBitmap(&pBmp_statCard, D2D1::RectF(168.f, 88.f, 236.f, 160.f));
		graphics->DrawBitmap(&pBmp_bottleWon, D2D1::RectF(80.f, 8.f, 160.f, 160.f));
		break;
	}
	case GAME_LOSE:
	{
		graphics->DrawBitmap(&pBmp_bg, D2D1::RectF(0, 0, 240.f, 160.f));
		graphics->DrawBitmap(&pBmp_statCard, D2D1::RectF(168.f, 88.f, 236.f, 160.f));
		graphics->DrawBitmap(&pBmp_bottleLost, D2D1::RectF(80.f, 8.f, 160.f, 160.f));
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
	case GAME_MENU:
	{
		graphics->DrawBitmap(&pBmp_menu, D2D1::RectF(0, 0, 240.f, 160.f)); // background
		graphics->DrawBitmap(&pBmp_pauseCircle, D2D1::RectF(82.f, 2.f + 70.f * menuOption, 151.f, 40.f + 70.f * menuOption)); // circle
		graphics->DrawBitmap(&pBmp_arrowVirus, D2D1::RectF(68.f + 10.f * menuVirus, 45.f, 77.f + 10 * menuVirus, 55.f)); // virus arrow
		graphics->DrawBitmap(&pBmp_arrowVirus, D2D1::RectF(46.f + 60.f * menuSpeed, 116.f, 76.f + 60 * menuSpeed, 124.f)); // speed arrow
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