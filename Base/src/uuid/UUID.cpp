#include "pch.h"
#include "UUID.h"

#include <random>
#include <unordered_map>
#include "entt/entt.hpp"
namespace Base {
	static bool s_MetaInit = false;
	static std::random_device s_RandomDevice;
#ifdef BASE_USING_UUID_64_T
	static std::mt19937_64 s_Engine(s_RandomDevice());
#else
	static std::mt19937 s_Engine(s_RandomDevice);
#endif
	static std::uniform_int_distribution<UUID_T> s_UniformDistribution;

	UUID::UUID()
		:m_UUID(s_UniformDistribution(s_Engine))
	{
	}

	UUID::UUID(UUID_T uuid)
		:m_UUID(uuid)
	{
	}
	bool UUID::MetaInit()
	{
		if (!s_MetaInit)
		{
			std::hash<std::string_view> hash{};
			{
				//UUID
				auto factory = entt::meta<UUID>().type(hash("UUID"));
				factory.
					data<&UUID::m_UUID>(hash("m_UUID"));
				factory.ctor<>();
				factory.ctor<const UUID&>();
			}

			s_MetaInit = true;
		}
		return s_MetaInit;
	}
}