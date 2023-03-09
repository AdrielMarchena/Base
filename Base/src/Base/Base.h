#ifndef BASE_API
#include <memory>

#define BASE_API

#define BASE_XPOS(x) x[3][0]
#define BASE_YPOS(x) x[3][1]
#define BASE_ZPOS(x) x[3][2]

#if defined BASE_DEBUG || BASE_RELEASE
	#define BASE_PROFILING //Temp
#endif

#ifdef _DEBUG
	#define BASE_DEBUG_CALL(x) x 
#else
	#define BASE_DEBUG_CALL(x)
#endif

namespace Base
{
	template<typename T>
	using KeyboardPressedCallback = std::function<void(T&)>;

	typedef int Key_T;
	//From The Cherno video
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T,class... _Args>
	Ref<T> MakeRef(_Args&&... args)
	{
		return std::make_shared<T>(std::forward<_Args>(args)...);
	}

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T, class... _Args>
	Scope<T> MakeScope(_Args&&... args)
	{
		return std::make_unique<T>(std::forward<_Args>(args)...);
	}

	struct WindowsProps
	{
		static int width;
		static int height;
		static double aspect_ratio;
		static bool minimized;
	};
	static WindowsProps& WindowProps()
	{
		static WindowsProps p;
		return p;
	}
	static double B_GetRatio()
	{
		return (double)WindowProps().width / (double)WindowProps().height;
	}

}
#endif