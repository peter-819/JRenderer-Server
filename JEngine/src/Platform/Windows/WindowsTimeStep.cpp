#include "JEpch.h"
#include "WindowsTimeStep.h"

namespace JEngine {
	void WindowsTimeStep::Init() {
		bool errorCode = QueryPerformanceFrequency(&PerformanceFrequency);
		JE_CORE_ASSERT(errorCode, "Can't Query Performace Frequency !");
		m_DeltaTime = 0;
		LastFrameCounter.QuadPart = 0;
	}

	void WindowsTimeStep::GetNewFrame() {
		LARGE_INTEGER nowFrameCounter;
		bool errorCode = QueryPerformanceCounter(&nowFrameCounter);
		JE_CORE_ASSERT(errorCode, "Can't Query Performace Counter !");

		LARGE_INTEGER deltaLI;
		double deltaTime = 0;
		deltaLI.QuadPart = nowFrameCounter.QuadPart - LastFrameCounter.QuadPart;
		if (LastFrameCounter.QuadPart != 0)
			deltaTime = ((float)deltaLI.QuadPart) / PerformanceFrequency.QuadPart;

		Sleep(m_TickTime - deltaTime);

		DeltaCounter.QuadPart = nowFrameCounter.QuadPart - LastFrameCounter.QuadPart;
		if (LastFrameCounter.QuadPart != 0)
			m_DeltaTime = ((float)DeltaCounter.QuadPart) / PerformanceFrequency.QuadPart;

		LastFrameCounter.QuadPart = nowFrameCounter.QuadPart;
	}
}