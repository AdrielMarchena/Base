#include "pch.h"
#include "Components.h"

namespace Base {
	void Init::InitComponents()
	{
		IDComponent();
		TagComponent();
		TransformComponent();
		TextureComponent();
		AnimateComponent();
		SubTextureComponent();
		SpriteComponent();
		ScriptComponent();
		CircleComponent();
		CameraComponent();
		NativeScriptComponent();
		RigidBody2DComponent();
		BoxColider2DComponent();
		CircleColider2DComponent();
		Text2DComponent();
		Perlin2dComponent();
	}
}