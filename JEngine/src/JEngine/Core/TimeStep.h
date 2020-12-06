#pragma once

namespace JEngine {
	class TimeStep {
	public:
		virtual void GetNewFrame() = 0;
		virtual void Init() = 0;

		static TimeStep* Create();
		float GetDeltaTime() const { return m_DeltaTime; }
		inline void SetTickTime(double t) { m_TickTime = t; }
	protected:
		float m_DeltaTime;
		float m_TickTime = 150.0f;
	};
}