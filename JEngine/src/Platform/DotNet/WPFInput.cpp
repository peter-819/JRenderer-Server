#include "JEpch.h"

#include "WPFInput.h"
#include "JEngine/Application.h"

namespace JEngine {
	Input* Input::s_Instance = new WPFInput();

	bool WPFInput::IsKeyPressedImpl(int keycode) {
		return keyboard.KeyState[keycode];
	}

	bool WPFInput::IsMouseButtonPressedImpl(int button) {
		return mouse.IsPressed[button];
	}

	std::pair<float, float> WPFInput::GetMousePosImpl() {
		return std::make_pair(mouse.x, mouse.y);
	}

	float WPFInput::GetMouseXImpl() {
		auto [x, y] = GetMousePosImpl();
		return x;
	}

	float WPFInput::GetMouseYImpl() {
		auto [x, y] = GetMousePosImpl();
		return y;
	}
}