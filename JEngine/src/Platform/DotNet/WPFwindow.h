#pragma once
#include "JEngine/Core/Window.h"

#include "Platform/OpenGL/WGLContext.h"

namespace JEngine {
	class WPFwindow : public Window {
	public:
		WPFwindow(const WindowProps& props);
		virtual ~WPFwindow();

		void OnUpdate() override;
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override {
			m_Data.EventCallback = callback;
		}

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return hwnd; }
		
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	public:
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
	private:
		HWND hwnd;
		WGLContext* m_Context;

		WindowData m_Data;

	public:
		inline WindowData& GetWindowData() { return m_Data; }
	};
}