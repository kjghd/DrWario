#pragma once
#include <Windows.h>

struct ButtonState
{
	bool down = false;
	bool changed = false;
};

enum Button
{
	BUTTON_A,
	BUTTON_D,
	BUTTON_E,
	BUTTON_F,
	BUTTON_Q,
	BUTTON_S,

	BUTTON_SPACE,
	BUTTON_ENTER,

	BUTTON_COUNT
};

class Input
{
public:

	ButtonState m_button[BUTTON_COUNT];
	float m_timeHeld[BUTTON_COUNT];
	
	Input();
	~Input();

	void SetButtonDown(WPARAM wParam, int vk_code, int button);
	void SetButtonUp(WPARAM wParam, int vk_code, int button);
	bool ButtonPressed(int button);
	bool ButtonHeld(int button);
	float ButtonHeldTime(int button);

	bool debugSetters;
	bool debugInput;
	void DebugMessage(bool condition, int button, LPCWSTR message);
};

static Input* input = new Input();