#include "Input.h"

Input::Input()
{
}
Input::~Input()
{
}

void Input::SetButtonDown(WPARAM wParam, int vk_code, int button)
{
	if (wParam == vk_code && !m_button[button].down)
	{
		m_button[button].down = true;
		m_button[button].changed = true;
	}
}

void Input::SetButtonUp(WPARAM wParam, int vk_code, int button)
{
	if (wParam == vk_code)
	{
		m_timeHeld[button] = 0;
		m_button[button].down = false;
		m_button[button].changed = true;
	}
}

bool Input::ButtonPressed(int button)
{
	if (m_button[button].down && m_button[button].changed)
	{
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
		return true;
	}
	else
	{
		return false;
	}
}

float Input::ButtonHeldTime(int button)
{
	if (m_button[button].down)
	{
		return m_timeHeld[button];
	}
	else
	{
		return false;
	}
}