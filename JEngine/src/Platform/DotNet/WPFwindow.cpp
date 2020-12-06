#include "JEpch.h"
#include "WPFwindow.h"

namespace JEngine {
	Window* Window::Create(const WindowProps& props) {
		return new WPFwindow(props);
	}

	WPFwindow::WPFwindow(const WindowProps& props) {
		Init(props);
	}
	WPFwindow::~WPFwindow() {
		Shutdown();
	}
	void WPFwindow::Init(const WindowProps& props) {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        
        HWND handle = (HWND)props.hwnd;

        if (handle == nullptr) {
            WNDCLASSEX wndClass; 
            memset(&wndClass, 0, sizeof wndClass);
            wndClass.cbSize = sizeof(WNDCLASSEX);
            wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
            wndClass.lpfnWndProc = &DefWindowProc;
            wndClass.cbClsExtra = 0;
            wndClass.cbWndExtra = 0;
            wndClass.hInstance = 0;
            wndClass.hIcon = 0;
            wndClass.hCursor = LoadCursor(0, IDC_ARROW);
            wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
            wndClass.lpszMenuName = 0;
            wndClass.lpszClassName = L"WndClass";
            wndClass.hIconSm = 0;
            RegisterClassEx(&wndClass);
            // style the window and remove the caption bar (WS_POPUP)
            DWORD style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
            // Create the window. Position and size it.
            handle = CreateWindowEx(0,
                L"WndClass",
                L"",
                style,
                CW_USEDEFAULT, CW_USEDEFAULT, m_Data.Width, m_Data.Height,
                0, 0, 0, 0);
        }
		m_Context = new WGLContext(handle);
		m_Context->Init();

	}
	void WPFwindow::Shutdown() {

	}
	void WPFwindow::OnUpdate() {
		m_Context->SwapBuffer();
	}
	void WPFwindow::SetVSync(bool enabled) {

	}
	bool WPFwindow::IsVSync() const {
		return m_Data.VSync;
	}
}