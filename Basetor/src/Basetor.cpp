#include "pch.h"
#include "Base/Base.h"
#include "Base.h"
#include "application/Entry.h"
#include "Editor.h"

class Basetor :public Base::Application
{
private:
public:
	Basetor(int argc, char** argv)
		:Base::Application(argc,argv)
	{
		PushLayer(new Base::Editor());
	}

	~Basetor()
	{
	}

};

Base::Application* Base::CreateApplication(int argc, char** argv)
{
	return new Basetor(argc, argv);
}

