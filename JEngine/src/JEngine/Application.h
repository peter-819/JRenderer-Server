#pragma once
#include "JEngine/Core/core.h"
#include "JEngine/Core/Window.h"
#include "JEngine/Events/Event.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "JEngine/Core/LayerStack.h"

#include "JEngine/ImGui/ImGuiLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Buffer.h"
#include "Renderer/VertexArray.h"

#include "Core/TimeStep.h"

namespace JEngine {
	struct AppProps {
		unsigned int width;
		unsigned int height;
		HWND hwnd;
		AppProps():width(1024),height(768),hwnd(nullptr){}
	};
	class JE_API Application
	{
	public:
		Application(AppProps props);
		virtual ~Application();
		virtual void run() {}

		void glTriangleTest();
		void AppRun();

		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverLayer(Layer* layer);
		Layer* GetMainRenderLayer() { return *m_LayerStack.begin(); }
		inline void SetTickTime(double t) { ts->SetTickTime(t); }

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool m_Running = true;
		
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
	
	private:
		TimeStep *ts;
	private:
		static Application* s_Instance;
	};
	Application* createApplication(AppProps props = AppProps());
}
