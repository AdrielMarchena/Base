/* From The Cherno video */
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

		entt::registry& Reg() { return m_Registry; }
		void OnUpdate(const UpdateArgs& args);
	private:
		entt::registry m_Registry;
	};
}


