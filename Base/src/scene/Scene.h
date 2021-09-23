/* From The Cherno video */
#pragma once
#include "entt/entt.hpp"
#include "args/UpdateArgs.h"

namespace Base
{
	class Entity;
	class Scene
	{
	public:

		Scene();
		~Scene();

		void SceneBegin();
		void SceneEnd();

		Entity CreateEntity(const std::string& name = std::string());
		void StartNativeScript(Entity& ent);
		void DestroyNativeScript(Entity& ent);

		//entt::registry& Reg() { return m_Registry; }
		void OnUpdate(const UpdateArgs& args);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}


