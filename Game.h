#pragma once
#include "Graphics.h"
#include "Input.h"
#include "ActivePill.h"
#include "SetPill.h"
#include "DIPs.h"
#include "SafeRelease.h"
#include <sstream>
#include <vector>
#include <algorithm>

// game objects and variables
ActivePill* o_ActivePill;
std::vector <SetPill*> o_SetPill;
ID2D1Bitmap* pBmp_bg;
ID2D1Bitmap* pBmp_bottle;
ID2D1Bitmap* pBmp_grid;
ID2D1Bitmap* pBmp_gridItem;
float time_ms;
D2D1_RECT_F grid[104];

struct PillSquare
{
	int m_pillIndex;
	int m_pillSide;
	int m_pillLoc;
	int m_pillColor;

	PillSquare(int pillIndex, int pillSide, int pillLoc, int pillColor)
		:
		m_pillIndex(pillIndex),
		m_pillSide(pillSide),
		m_pillLoc(pillLoc),
		m_pillColor(pillColor)
	{
	};
};
std::vector <PillSquare> ps;
bool SortPillSquare(PillSquare i, PillSquare j)
{
	return i.m_pillLoc < j.m_pillLoc;
}
std::vector <int> matches;

// Game functions

void DestroyMatches()
{
	for (size_t i = 0; i < matches.size(); i++)
	{
		o_SetPill.erase(o_SetPill.begin() + ps[matches[i]].m_pillIndex);
	}
}

void CheckRow()
{
	// sort
	ps.clear();
	for (size_t i = 0; i < o_SetPill.size(); i++)
	{
		ps.push_back(PillSquare(i, 0, o_SetPill[i]->sideA->m_bmpLoc, o_SetPill[i]->sideA->m_color));
		ps.push_back(PillSquare(i, 1, o_SetPill[i]->sideB->m_bmpLoc, o_SetPill[i]->sideB->m_color));
	}
	std::sort(ps.begin(), ps.end(), SortPillSquare);

	// check for matches
	matches.clear();
	int current;
	int count;
	bool inMatches;
	OutputDebugString(L"Match debug\n");
	// X
	for (size_t i = 0; i < ps.size(); i++) // enumerate through every pill side
	{
		// check if pill side is in matches already
		inMatches = false;
		for (size_t ii = 0; ii < matches.size(); ii++)
		{
			if (i == matches[ii])
			{
				OutputDebugString(std::to_wstring(ps[i].m_pillLoc).c_str());
				OutputDebugString(L" already in matches\n");
				inMatches = true;
			}
		}
		// if not add pill side to matches
		if (!inMatches)
		{
			current = i;
			matches.push_back(current);
			count = 1;
			OutputDebugString(L"	Starting row with ");
			OutputDebugString(std::to_wstring(ps[current].m_pillLoc).c_str());
			OutputDebugString(L"\n");
			for (size_t ii = 0; ii < ps.size(); ii++) // check current against all other pill sides
			{
				// if right of current is a match, add it to matches
				if (ps[current].m_pillLoc + 1 == ps[ii].m_pillLoc &&
					ps[current].m_pillColor == ps[ii].m_pillColor)
				{
					OutputDebugString(L"match in square: ");
					OutputDebugString(std::to_wstring(ps[current].m_pillLoc).c_str());
					OutputDebugString(L" & ");
					OutputDebugString(std::to_wstring(ps[ii].m_pillLoc).c_str());
					OutputDebugString(L"\n");
					current = ii;		   // update current
					matches.push_back(ii); // add match
					ii = 0;				   // reset loop for updated current
					count++;			   // increment match count
				}
			}
			// if the total matches just added is < 4, remove them from matches
			if (count < 4)
			{
				OutputDebugString(L"Count: ");
				OutputDebugString(std::to_wstring(count).c_str());
				OutputDebugString(L"\n");
				for (int ii = 0; ii < count; ii++)
				{
					OutputDebugString(L"Discarded ");
					OutputDebugString(std::to_wstring(ps[matches.back()].m_pillLoc).c_str());
					OutputDebugString(L"\n");
					matches.pop_back();
				}
			}
			else
			{
				OutputDebugString(L"Count: ");
				OutputDebugString(std::to_wstring(count).c_str());
				OutputDebugString(L"\n");
			}
		}
		OutputDebugString(L"\n");
	}
	// Y
	for (size_t i = 0; i < ps.size(); i++) // enumerate through every pill side
	{
		// check if pill side is in matches already
		inMatches = false;
		for (size_t ii = 0; ii < matches.size(); ii++)
		{
			if (i == matches[ii])
			{
				OutputDebugString(std::to_wstring(ps[i].m_pillLoc).c_str());
				OutputDebugString(L" already in matches\n");
				inMatches = true;
			}
		}
		// if not add pill side to matches
		if (!inMatches)
		{
			current = i;
			matches.push_back(current);
			count = 1;
			OutputDebugString(L"	Starting row with ");
			OutputDebugString(std::to_wstring(ps[current].m_pillLoc).c_str());
			OutputDebugString(L"\n");
			for (size_t ii = 0; ii < ps.size(); ii++) // check current against all other pill sides
			{
				// if right of current is a match, add it to matches
				if (ps[current].m_pillLoc + 8 == ps[ii].m_pillLoc &&
					ps[current].m_pillColor == ps[ii].m_pillColor)
				{
					OutputDebugString(L"match in square: ");
					OutputDebugString(std::to_wstring(ps[current].m_pillLoc).c_str());
					OutputDebugString(L" & ");
					OutputDebugString(std::to_wstring(ps[ii].m_pillLoc).c_str());
					OutputDebugString(L"\n");
					current = ii;		   // update current
					matches.push_back(ii); // add match
					ii = 0;				   // reset loop for updated current
					count++;			   // increment match count
				}
			}
			// if the total matches just added is < 4, remove them from matches
			if (count < 4)
			{
				OutputDebugString(L"Count: ");
				OutputDebugString(std::to_wstring(count).c_str());
				OutputDebugString(L"\n");
				for (int ii = 0; ii < count; ii++)
				{
					OutputDebugString(L"Discarded ");
					OutputDebugString(std::to_wstring(ps[matches.back()].m_pillLoc).c_str());
					OutputDebugString(L"\n");
					matches.pop_back();
				}
			}
			else
			{
				OutputDebugString(L"Count: ");
				OutputDebugString(std::to_wstring(count).c_str());
				OutputDebugString(L"\n");
			}
		}
		OutputDebugString(L"\n");
	}

	// debug matches
	OutputDebugString(L"Match locations\n");
	for (size_t i = 0; i < matches.size(); i++)
	{
		OutputDebugString(std::to_wstring(ps[matches[i]].m_pillLoc).c_str());
		OutputDebugString(L", ");
	}
	OutputDebugString(L"\n");
	OutputDebugString(L"\n");

	// set bitmap
	for (size_t i = 0; i < matches.size(); i++)
	{
		if (ps[matches[i]].m_pillSide == 0) // if side 0
		{
			o_SetPill[ps[matches[i]].m_pillIndex]->sideA->m_bmpTile = {
				35,
				(float)(7 * ps[matches[i]].m_pillColor - 7),
				42,
				(float)(7 * ps[matches[i]].m_pillColor)
			};
		}
		else if (ps[matches[i]].m_pillSide == 1) // if side 1
		{
			o_SetPill[ps[matches[i]].m_pillIndex]->sideB->m_bmpTile = {
				35,
				(float)(7 * ps[matches[i]].m_pillColor - 7),
				42,
				(float)(7 * ps[matches[i]].m_pillColor)
			};
		}
	}
}


void Serve()
{
	o_ActivePill = new ActivePill(pBmp_gridItem, PILLCOLOR_BLUE, PILLCOLOR_RED);
	OutputDebugString(L"Serving\n");
}

void Land()
{
	o_SetPill.resize(o_SetPill.size() + 1);
	o_SetPill[o_SetPill.size() - 1] = new SetPill(
		pBmp_gridItem,
		o_ActivePill->sideA->m_color,
		o_ActivePill->sideB->m_color,
		o_ActivePill->sideA->m_bmpLoc,
		o_ActivePill->sideB->m_bmpLoc
	);

	// delete active pill
	o_ActivePill = NULL;
	OutputDebugString(L"Landed\n");
	CheckRow();
	if (matches.size() > 0)
	{
		DestroyMatches();
	}
}

void Aim()
{
	// idle
	if (time_ms > 1.f)
	{
		time_ms = 0;

		// check collision
		for (size_t i = 0; i < o_SetPill.size(); i++)
		{
			if (o_ActivePill->sideA->m_bmpLoc + 8 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideA->m_bmpLoc + 8 == o_SetPill[i]->sideB->m_bmpLoc ||
				o_ActivePill->sideB->m_bmpLoc + 8 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideB->m_bmpLoc + 8 == o_SetPill[i]->sideB->m_bmpLoc)
			{
				Land();
				return;
			}
		}
		// touching ground
		if (o_ActivePill->m_base >= 96)
		{
			Land();
		}
		// move down
		else
		{
			o_ActivePill->m_base += 8;
			o_ActivePill->sideA->m_bmpLoc += 8;
			o_ActivePill->sideB->m_bmpLoc += 8;
		}
	}

	// manual
		// clockwise
	if (input->ButtonPressed(BUTTON_E))
	{
		switch (o_ActivePill->m_rotation)
		{
		case PILL_RIGHT:
			o_ActivePill->SetToDown();
			break;
			
		case PILL_DOWN:
			o_ActivePill->SetToLeft();
			break;

		case PILL_LEFT:
			o_ActivePill->SetToUp();
			break;
			
		case PILL_UP:
			o_ActivePill->SetToRight();
			break;

		default:
			break;
		}
	}
	// anti clockwise
	if (input->ButtonPressed(BUTTON_Q))
	{
		switch (o_ActivePill->m_rotation)
		{
		case PILL_RIGHT:
			o_ActivePill->SetToUp();
			break;
			
		case PILL_UP:
			o_ActivePill->SetToLeft();
			break;
			
		case PILL_LEFT:
			o_ActivePill->SetToDown();
			break;
			
		case PILL_DOWN:
			o_ActivePill->SetToRight();
			break;
			
		default:
			break;
		}
	}
	// move left
	if (input->ButtonPressed(BUTTON_A))
	{
		// if against left side return
		for (size_t i = 0; i < 13; i++)
		{
			if (o_ActivePill->sideA->m_bmpLoc == i * 8 || o_ActivePill->sideB->m_bmpLoc == i * 8)
			{
				return;
			}
		}
		// if pill to left return
		for (size_t i = 0; i < o_SetPill.size(); i++)
		{
			if (o_ActivePill->sideA->m_bmpLoc - 1 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideA->m_bmpLoc - 1 == o_SetPill[i]->sideB->m_bmpLoc || 
				o_ActivePill->sideB->m_bmpLoc - 1 == o_SetPill[i]->sideA->m_bmpLoc || 
				o_ActivePill->sideB->m_bmpLoc - 1 == o_SetPill[i]->sideB->m_bmpLoc)
			{
				return;
			}
		}
		// else move to left
		o_ActivePill->m_base -= 1;
		o_ActivePill->sideA->m_bmpLoc -= 1;
		o_ActivePill->sideB->m_bmpLoc -= 1;
	}
	// move right
	if (input->ButtonPressed(BUTTON_D))
	{
		// if against right side return
		for (size_t i = 0; i < 13; i++)
		{
			if (o_ActivePill->sideA->m_bmpLoc == i * 8 + 7 || o_ActivePill->sideB->m_bmpLoc == i * 8 + 7)
			{
				return;
			}
		}
		// if pill to right return
		for (size_t i = 0; i < o_SetPill.size(); i++)
		{
			if (o_ActivePill->sideA->m_bmpLoc + 1 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideA->m_bmpLoc + 1 == o_SetPill[i]->sideB->m_bmpLoc ||
				o_ActivePill->sideB->m_bmpLoc + 1 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideB->m_bmpLoc + 1 == o_SetPill[i]->sideB->m_bmpLoc)
			{
				return;
			}
		}
		// else move to right
		o_ActivePill->m_base += 1;
		o_ActivePill->sideA->m_bmpLoc += 1;
		o_ActivePill->sideB->m_bmpLoc += 1;
	}
	// move down
	if (input->ButtonPressed(BUTTON_S))
	{
		// if pill underneath return
		for (size_t i = 0; i < o_SetPill.size(); i++)
		{
			if (o_ActivePill->sideA->m_bmpLoc + 8 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideA->m_bmpLoc + 8 == o_SetPill[i]->sideB->m_bmpLoc ||
				o_ActivePill->sideB->m_bmpLoc + 8 == o_SetPill[i]->sideA->m_bmpLoc ||
				o_ActivePill->sideB->m_bmpLoc + 8 == o_SetPill[i]->sideB->m_bmpLoc)
			{
				return;
			}
		}
		// if against bottom return
		if (o_ActivePill->sideA->m_bmpLoc >= 96 || o_ActivePill->sideB->m_bmpLoc >= 96)
		{
			return;
		}
		// else move down
		o_ActivePill->m_base += 8;
		o_ActivePill->sideA->m_bmpLoc += 8;
		o_ActivePill->sideB->m_bmpLoc += 8;
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
	o_ActivePill = new ActivePill(pBmp_gridItem, PILLCOLOR_BLUE, PILLCOLOR_RED);

	time_ms = 0;
}

void GameUpdate(float deltaTime)
{
	time_ms += deltaTime;
	if (o_ActivePill)
	{
		Aim();
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

	// bottle
	graphics->DrawBitmap(&pBmp_bottle, D2D1::RectF(80.f, 8.f, 160.f, 160.f)); 
	// grid debug
	for (size_t i = 0; i < 104; i++)
	{
		if (true)
		{
			graphics->DrawBitmap(&pBmp_grid, grid[i]);
		}
	}
	// set pills
	for (size_t i = 0; i < o_SetPill.size(); i++)
	{
		D2D1_RECT_F locA = {
			grid[o_SetPill[i]->sideA->m_bmpLoc].left        ,
			grid[o_SetPill[i]->sideA->m_bmpLoc].top         ,
			grid[o_SetPill[i]->sideA->m_bmpLoc].right  - 1.f,
			grid[o_SetPill[i]->sideA->m_bmpLoc].bottom - 1.f
		};
		D2D1_RECT_F locB = {
			grid[o_SetPill[i]->sideB->m_bmpLoc].left        ,
			grid[o_SetPill[i]->sideB->m_bmpLoc].top         ,
			grid[o_SetPill[i]->sideB->m_bmpLoc].right  - 1.f,
			grid[o_SetPill[i]->sideB->m_bmpLoc].bottom - 1.f
		};
		graphics->DrawBitmapTile(&pBmp_gridItem, locA, o_SetPill[i]->sideA->m_bmpTile);
		graphics->DrawBitmapTile(&pBmp_gridItem, locB, o_SetPill[i]->sideB->m_bmpTile);
	}
	// active pill
	if (o_ActivePill)
	{
		D2D1_RECT_F locA = {
			grid[o_ActivePill->sideA->m_bmpLoc].left       ,
			grid[o_ActivePill->sideA->m_bmpLoc].top   + 1.f,
			grid[o_ActivePill->sideA->m_bmpLoc].right - 1.f,
			grid[o_ActivePill->sideA->m_bmpLoc].bottom
		};
		D2D1_RECT_F locB = {
			grid[o_ActivePill->sideB->m_bmpLoc].left       ,
			grid[o_ActivePill->sideB->m_bmpLoc].top   + 1.f,
			grid[o_ActivePill->sideB->m_bmpLoc].right - 1.f,
			grid[o_ActivePill->sideB->m_bmpLoc].bottom
		};
		graphics->DrawBitmapTile(&o_ActivePill->sideA->m_pBitmap, locA, o_ActivePill->sideA->m_bmpTile);
		graphics->DrawBitmapTile(&o_ActivePill->sideB->m_pBitmap, locB, o_ActivePill->sideB->m_bmpTile);
	}

	graphics->EndDraw();
}

void GameRelease()
{
	SafeRelease(&pBmp_grid);
	SafeRelease(&pBmp_bg);
	SafeRelease(&pBmp_bottle);
}