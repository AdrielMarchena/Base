/* From The Cherno video */
#pragma once
#include "entt/entt.hpp"
#include "args/UpdateArgs.h"
#include "render/gl/FramebufferRender.h"
#include "SceneCamera.h"
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
		void AwakeNativeScript(Entity& ent);

		void SetFrameBuff(unsigned int w, unsigned int h, float scale_factor = 1.0f,bool using_lut = true);
		const std::unordered_map<std::string, FramebufferPostEffect>& GetPostEffects() const;
		void SetPostEffect(const std::string& name);

		//entt::registry& Reg() { return m_Registry; }
		void OnUpdate(const UpdateArgs& args);
	private:
		void DrawScene(float dt);

		Scope<FramebufferRender> m_FrameBufferRender;
		SceneCamera m_FramebufferCamera;
		
		entt::registry m_Registry;
		friend class Entity;
	};
}


