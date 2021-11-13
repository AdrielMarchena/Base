#include "Window.h"
#include "WindowsWindow.h"
namespace Base
{
	Scope<Window> Base::Window::Create(const WindowSpecifications& specs)
	{
		return MakeScope<WindowsWindow>(specs);
	}
}