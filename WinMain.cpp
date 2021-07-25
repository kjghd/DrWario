#include "Globals.h"
#include "DIPs.h"
#include "Game.h"
#include "Input.h"
#include <Windows.h>
#include <sstream>

// Window Procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:

		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}

// Main
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Main Window Class
	LPCWSTR wcName = L"WndClass";
	WNDCLASS wc = { NULL };
	wc.hInstance	 = hInstance;
	wc.lpfnWndProc	 = WndProc;
	wc.lpszClassName = wcName;
	RegisterClass(&wc);

	// Calculate desired window size
	RECT rc = { 0, 0, 240 * (int)screenScale, 160 * (int)screenScale};
	AdjustWindowRect(&rc, WS_CAPTION, FALSE);

	// Main Window
	HWND hWnd = CreateWindow(
		wcName,
		L"Dr Wario",
		WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);

	RECT rcc;
	GetClientRect(hWnd, &rcc);

	OutputDebugString(std::to_wstring(rcc.left).c_str());
	OutputDebugString(L"\n");
	OutputDebugString(std::to_wstring(rcc.right).c_str());
	OutputDebugString(L"\n");
	OutputDebugString(std::to_wstring(rcc.top).c_str());
	OutputDebugString(L"\n");
	OutputDebugString(std::to_wstring(rcc.bottom).c_str());
	OutputDebugString(L"\n");

	// Initialize Game
	GameInit(&hWnd);

	// start timer
	LARGE_INTEGER timerFreq  = {NULL};
	LARGE_INTEGER timerStart = {NULL};
	LARGE_INTEGER timerEnd	 = {NULL};
	LARGE_INTEGER timerDiff  = {NULL};
	float deltaTime = 0;
	float timeSinceFrame = 0;
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&timerStart);

	// Message Loop
	MSG msg = { NULL };
	while (msg.message != WM_QUIT)
	{
		for (size_t i = 0; i < BUTTON_COUNT; i++)
		{
			input->m_button[i].changed = false;
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			switch (msg.message)
			{
			case WM_KEYDOWN:
			{
				input->SetButtonDown(msg.wParam, 0x41, BUTTON_A);
				input->SetButtonDown(msg.wParam, 0x44, BUTTON_D);
				input->SetButtonDown(msg.wParam, 0x45, BUTTON_E);
				input->SetButtonDown(msg.wParam, 0x46, BUTTON_F);
				input->SetButtonDown(msg.wParam, 0x51, BUTTON_Q);
				input->SetButtonDown(msg.wParam, 0x53, BUTTON_S);
				input->SetButtonDown(msg.wParam, VK_SPACE, BUTTON_SPACE);
				input->SetButtonDown(msg.wParam, VK_RETURN, BUTTON_ENTER);
				break;
			}
			case WM_KEYUP:
			{
				input->SetButtonUp(msg.wParam, 0x41, BUTTON_A);
				input->SetButtonUp(msg.wParam, 0x44, BUTTON_D);
				input->SetButtonUp(msg.wParam, 0x45, BUTTON_E);
				input->SetButtonUp(msg.wParam, 0x46, BUTTON_F);
				input->SetButtonUp(msg.wParam, 0x51, BUTTON_Q);
				input->SetButtonUp(msg.wParam, 0x53, BUTTON_S);
				input->SetButtonUp(msg.wParam, VK_SPACE, BUTTON_SPACE);
				input->SetButtonUp(msg.wParam, VK_RETURN, BUTTON_ENTER);
				break;
			}
			default:
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// update and render game
			timeSinceFrame = 0;
			GameUpdate(deltaTime);
			GameRender(&hWnd);

		// calculate deltatime and reset timer
		QueryPerformanceCounter(&timerEnd);
		timerDiff.QuadPart = timerEnd.QuadPart - timerStart.QuadPart;
		deltaTime = (float)timerDiff.QuadPart / (float)timerFreq.QuadPart;
		QueryPerformanceCounter(&timerStart);
	}
	GameRelease();
	return 0;
}

