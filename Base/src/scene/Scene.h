#pragma once

#include "entt/entt.hpp"
#include "args/UpdateArgs.h"
namespace en
{
	class Scene
	{
	public:

		Scene();
		~Scene();

		entt::entity CreateEntity();

		void OnUpdate(const UpdateArgs& args);
	private:
		entt::registry m_Registry;
	};
}


