#ifndef BASE_API
#include <memory>

#define BASE_API

namespace Base
{
	//From The Cherno video
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template<typename T>
	using Scope = std::unique_ptr<T>;
}

#include "render/render2D.h"
#include "Log.h"

#endif