#pragma once

#ifdef JE_PLATFORM_WINDOWS

extern JEngine::Application* JEngine::createApplication(JEngine::AppProps props);

int main(int argc, char** argv) {
	JEngine::Log::Init();
	JE_CORE_WARN("Initialized Log!");
	auto p = JEngine::createApplication(JEngine::AppProps());
	p->AppRun();
	delete p;
	return 0;
}

#endif