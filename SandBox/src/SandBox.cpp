#pragma once

#include "Base/Base.h"
#include "Base.h"
#include "application/Entry.h"

#include "TestLayer.h"

#include <unordered_map>
#include <vector>

class SandBox :public Base::Application
{
private:
public:
	SandBox(int argc, char** argv)
		:Base::Application(argc,argv)
	{
		PushLayer(new TestLayer());
	}

	~SandBox()
	{

	}

};

Base::Application* Base::CreateApplication(int argc, char** argv)
{
	return new SandBox(argc, argv);
}

