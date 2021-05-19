#pragma once
#include <future>
namespace en
{
namespace utils
{

	template<typename T, typename F>
	struct ResourceLoads
	{
	public:
		std::unordered_map<T, F> resources;
		std::unordered_map<T, std::future<void>> futures;
		std::mutex mutex;

		ResourceLoads() = default;

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