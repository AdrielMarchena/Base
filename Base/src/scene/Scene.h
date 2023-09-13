/* From The Cherno video */
#pragma once
#include "args/UpdateArgs.h"
#include "render/gl/FramebufferRender.h"
#include "SceneCamera.h"
#include "entt/entt.hpp"
#include "uuid/UUID.h"
#include "EditorCamera.h"
#include "Components.h"
#include <concepts>
class b2World;
namespace Base {
	class Entity;
	class Scene
	{
	public:

		Scene();
		~Scene();

		void SceneBegin();
		void SceneEnd();

		//Entity Stuff
		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWhithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity ent);

		void StartNativeScript(Entity& ent);
		void DestroyNativeScript(Entity& ent);
		void AwakeNativeScript(Entity& ent);
		/* Return the fist Primary camera it finds */
		Entity GetPrimaryCamera();
		Entity GetEntityByUUID(UUID uuid);

		//Runtime
		void RuntimeInit();
		void RuntimeStop();

		//Updates
		void OnUpdateEditor(const UpdateArgs& args, EditorCamera& camera);
		void OnUpdateRuntime(const UpdateArgs& args);

		//ViewPort
		void OnViewPortResize(uint32_t w, uint32_t h);

		template<typename U, Derived<Component<U>> T>
		void OnComponentAdded(Entity ent, T& component);
	private:

		uint32_t m_ViewPortWidth;
		uint32_t m_ViewPortHeight;
		std::unordered_map<UUID, entt::entity> m_EntityMap;

		b2World* m_PhysicWorld = nullptr;

		entt::registry m_Registry;
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
		friend class PropertiesPanel;
	};
}


