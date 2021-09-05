/* From The Cherno video */
#pragma once
#include "entt/entt.hpp"
#include "args/UpdateArgs.h"
#include "Components.h"

namespace en
{
	class Entity;
	class Scene
	{
	public:

		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		//entt::registry& Reg() { return m_Registry; }
		void OnUpdate(const UpdateArgs& args);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}


