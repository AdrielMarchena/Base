/* From The Cherno video */
#pragma once
#include "entt/entt.hpp"
#include "args/UpdateArgs.h"
#include "render/gl/Framebuffer.h"
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

		void SetFrameBuff(unsigned int w, unsigned int h, float scale_factor = 1.0f);

		//entt::registry& Reg() { return m_Registry; }
		void OnUpdate(const UpdateArgs& args);
	private:
		void DrawScene(float dt);

		Scope<Framebuffer> m_FrameBuffer;
		SceneCamera m_Camera;
		
		entt::registry m_Registry;
		friend class Entity;
	};
}


