#include <JEpch.h>
#include <JEngine.h>
#include "Platform/DotNet/WPFwindow.h"
#include "JEngine/Events/Event.h"
#include "Platform/DotNet/WPFInput.h"

#define JAPI extern "C" __declspec(dllexport) 

JAPI void OnWindowSize(JEngine::Application* app, int width, int height) {
	JEngine::WPFwindow& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();
	data.Width = width;
	data.Height = height;

	JEngine::WindowResizeEvent e(width, height);
	data.EventCallback(e);
}

JAPI void OnWindowClose(JEngine::Application* app) {
	auto& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();
	JEngine::WindowCloseEvent e;
	data.EventCallback(e);
}

JAPI void OnKey(JEngine::Application* app,int key,int scancode,int action,int mods) {
	auto& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();

	switch (action) {
		case 1: { //press
			JEngine::KeyPressedEvent event(key, 0);
			data.EventCallback(event);
			break;
		}
		case 2: { //release
			JEngine::KeyReleasedEvent event(key);
			data.EventCallback(event);
			break;
		}
		case 3: { //repeat
			JEngine::KeyPressedEvent event(key, 1);
			data.EventCallback(event);
			break;
		}
	}
}

JAPI void OnChar(JEngine::Application* app, unsigned int keycode) {
	auto& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();
	JEngine::KeyTypedEvent e(keycode);
	data.EventCallback(e);
}

JAPI void OnMouseButton(JEngine::Application* app, int button, int action) {
	auto& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();
	switch (action) {
		case 0: //release
		{
			JEngine::MouseButtonReleasedEvent e(button);
			data.EventCallback(e);
			break;
		}
		case 1: //press
		{
			JEngine::MouseButtonPressedEvent e(button);
			data.EventCallback(e);
			break;
		}
	}
	auto* input = JEngine::Input::s_Instance;
	dynamic_cast<JEngine::WPFInput*>(input)->mouse.IsPressed[button] = action;
}

JAPI void OnScroll(JEngine::Application* app, double xOffset, double yOffset) {
	auto& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();
	JEngine::MouseScrolledEvent e((float)xOffset, (float)yOffset);
	data.EventCallback(e);
}

JAPI void OnCursor(JEngine::Application* app, double xPos, double yPos) {
	auto& handle = dynamic_cast<JEngine::WPFwindow&>(app->GetWindow());
	auto& data = handle.GetWindowData();
	JEngine::MouseMovedEvent e((float)xPos, (float)yPos);
	data.EventCallback(e);

	auto* input = JEngine::Input::s_Instance;
	dynamic_cast<JEngine::WPFInput*>(input)->mouse.x = xPos;
	dynamic_cast<JEngine::WPFInput*>(input)->mouse.y = yPos;

}