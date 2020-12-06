#pragma once

#include "JEngine/Core/core.h"
#include "JEngine/Core/Input.h"
#include <cstring>

namespace JEngine {
	class WPFInput : public Input {
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePosImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;

	public:
		struct Mouse {
			int x, y;
			int IsPressed[10];
			Mouse() {
				memset(IsPressed, 0, sizeof IsPressed);
			}
		};
		struct Keyboard {
			int KeyState[350];
			Keyboard() {
				memset(KeyState, 0, sizeof KeyState);
			}
		};
		
		Mouse mouse;
		Keyboard keyboard;
	};
}