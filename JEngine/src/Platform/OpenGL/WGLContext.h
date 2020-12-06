#pragma once 

#include "JEngine/Renderer/GraphicContext.h"

namespace JEngine {

	class WGLContext : public GraphicContext {

	public:
		WGLContext(HWND windowhandle);
		virtual void Init() override;
		virtual void SwapBuffer() override;

	private:
		HWND hwnd;
		HDC mHDC;
		HGLRC mHGLRC;
		int mPixelFormat;
		PIXELFORMATDESCRIPTOR mPFD;
	};
}