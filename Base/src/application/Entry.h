#pragma once
#include "Log.h"
#include "application/Application.h"
extern Base::Application* Base::CreateApplication(int argc, char** argv);

int main(int argc, char** argv)
{
	Base::Log::Init();
	auto app = Base::CreateApplication(argc, argv);
	app->Run();
	delete app;

}