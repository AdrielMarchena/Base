#include "pch.h"
#include "Base/Base.h"

namespace Base
{

	int WindowsProps::width = 800;
	int WindowsProps::height = 800;
	double WindowsProps::aspect_ratio = B_GetRatio();
	bool WindowsProps::minimized = false;
}