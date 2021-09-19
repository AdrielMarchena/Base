#ifndef BASE_API
#include <memory>

#define BASE_API

#define BASE_XPOS(x) x[3][0]
#define BASE_YPOS(x) x[3][1]
#define BASE_ZPOS(x) x[3][2]

namespace Base
{
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

	namespace WindowProps
	{
		static int width;
		static int height;
		static bool minimized;
	};
}
#endif