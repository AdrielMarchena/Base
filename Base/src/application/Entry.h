#pragma once
#include <iostream>
#include "Log.h"
#include "application/Application.h"
#include "utils/Instrumentor.h"
extern Base::Application* Base::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	try
	{
		Base::Log::Init();

		BASE_PROFILE_BEGIN_SESSION("Initialization", "Profiling_Init.json");
		auto app = Base::CreateApplication(argc, argv);
		Base::Application::m_AppInstance = app;
		BASE_PROFILE_END_SESSION();

		app->Run();

		BASE_PROFILE_BEGIN_SESSION("Shutdown", "Profiling_Shutdown.json");
		delete app;
		BASE_PROFILE_END_SESSION();
	}
	catch (const std::exception& e)
	{
		std::cout << "Unhandled exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}