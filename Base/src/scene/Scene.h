/* From The Cherno video */
#pragma once
#include "args/UpdateArgs.h"
#include "render/gl/FramebufferRender.h"
#include "SceneCamera.h"
#include "entt/entt.hpp"
#include "uuid/UUID.h"
class b2World;
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
		Entity CreateEntityWhithUUID(UUID uuid,const std::string& name = std::string());
		void StartNativeScript(Entity& ent);
		void DestroyNativeScript(Entity& ent);
		void AwakeNativeScript(Entity& ent);

		void SetFrameBuff();
		const std::unordered_map<std::string, FramebufferPostEffect>& GetPostEffects() const;
		void SetPostEffect(const std::string& name);

		void RuntimeInit();
		void RuntimeStop();

		//entt::registry& Reg() { return m_Registry; }
		void OnUpdate(const UpdateArgs& args);

		void OnViewPortResize(uint32_t w, uint32_t h);
		void SetFramebufferScaler(float scaler);
		float GetFramebufferScaler() const { return m_FramebufferScaler; }
	private:

		Scope<FramebufferRender> m_FrameBufferRender;
		SceneCamera m_FramebufferCamera;
		
		float m_FramebufferScaler = 1.0f;
		uint32_t m_ViewPortWidth;
		uint32_t m_ViewPortHeight;

		b2World* m_PhysicWorld = nullptr;

		entt::registry m_Registry;
		friend class Entity;
		friend class SceneSerializer;
	};
}


