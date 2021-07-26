#include "Input.h"
#include <sstream>

Input::Input()
	:
	debugSetters(true),
	debugInput(true),
	m_timeHeld()
{
}
Input::~Input()
{
}

void Input::SetButtonDown(WPARAM wParam, int vk_code, int button)
{
	if (wParam == vk_code && !m_button[button].down)
	{
		DebugMessage(debugSetters, button, L"button set down!\n");
		m_button[button].down = true;
		m_button[button].changed = true;
	}
}

void Input::SetButtonUp(WPARAM wParam, int vk_code, int button)
{
	if (wParam == vk_code)
	{
		DebugMessage(debugSetters, button, L"button set up!\n");
		m_timeHeld[button] = 0;
		m_button[button].down = false;
		m_button[button].changed = true;
	}
}

bool Input::ButtonPressed(int button)
{
	if (m_button[button].down && m_button[button].changed)
	{
		DebugMessage(debugInput, button, L"button pessed!\n");
		return true;
	}
	else
	{
		return false;
	}
}

bool Input::ButtonHeld(int button)
{
	if (m_button[button].down)
	{
		DebugMessage(debugInput, button, L"button held!\n");
		return true;
	}
	else
	{
		return false;
	}
}

float Input::ButtonHeldTime(int button, float deltaTime)
{
	if (m_button[button].down)
	{
		m_timeHeld[button] += deltaTime;

		DebugMessage(debugInput, button, L"button held for ", m_timeHeld[button], L" s\n");

		return m_timeHeld[button];

	}
	return 0;
}

void Input::DebugMessage(bool condition, int button, LPCWSTR message, float timing, LPCWSTR timingMessage)
{
	if (condition)
	{
		LPCWSTR btn_wstr = L"null ";
		switch (button)
		{
		case BUTTON_A:
			btn_wstr = L"'A' ";
			break;
		case BUTTON_D:
			btn_wstr = L"'D' ";
			break;
		case BUTTON_E:
			btn_wstr = L"'E' ";
			break;
		case BUTTON_F:
			btn_wstr = L"'F' ";
			break;
		case BUTTON_Q:
			btn_wstr = L"'Q' ";
			break;
		case BUTTON_S:
			btn_wstr = L"'S' ";
			break;

		default:
			break;
		}
		OutputDebugString(btn_wstr);
		OutputDebugString(message);
		if (timing)
		{
			OutputDebugString(std::to_wstring(timing).c_str());
			OutputDebugString(timingMessage);
		}
		
	}
}