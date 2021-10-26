/*
* 	@file threading.cpp
* 	@author Adriel Marchena Santos
*
* 	A resource load for multi thread things
*
*/

#pragma once
#include <future>
namespace Base
{
namespace utils
{
	template<typename _Key, typename _Res>
	struct ResourceLoads
	{
	public:
		std::unordered_map<_Key, _Res> resources;
		std::unordered_map<_Key, std::future<void>> futures;
		std::mutex mutex;

		ResourceLoads() = default;
		ResourceLoads(ResourceLoads&) = delete;
		ResourceLoads(ResourceLoads&&) = delete;
		ResourceLoads& operator=(const ResourceLoads&) = delete;

		bool isAllLoad() const
		{
			for (auto& f : futures)
				if (!f.second._Is_ready())
					return false;
			return true;
		}

		void waitAll() const
		{
			while (!isAllLoad()) {}
		}
	};
}
}