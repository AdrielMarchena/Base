#pragma once
#include "Base/Base.h"
#include "event/Events.h"

#include <stdint.h>
#include <functional>
#include <string>
namespace Base
{

	struct WindowSpecifications
	{
		std::string Title = "Base";
		uint32_t Width = 800;
		uint32_t Height = 600;
		bool VSync_On = true;
		bool Fullscreen = false;
		bool Decorated = true;
		bool Resizeble = true;
	};

class Window
{
private:
public:
	using EventCallbackFn = std::function<void(Event&)>;
	Window() = default;

	virtual ~Window() {}
	virtual void OnUpdate() = 0;

	virtual uint32_t GetWidth() const = 0;
	virtual uint32_t GetHeight() const = 0;

	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetVSync(bool enabled) = 0;
	virtual bool GetVSync() const = 0;

	virtual void* GetNative() const = 0;

	static Scope<Window> Create(const WindowSpecifications& specs = WindowSpecifications());
};

}

