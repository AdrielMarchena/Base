#pragma once

#include "Components.h"
#include "ScriptableEntity.h"
namespace Base
{
	/* Simple control script for orthographic cameras */
	class OrthoCameraScript : public ScriptableEntity
	{
	public:
		float Velocity = 5.5f;
	protected:
		virtual void OnUpdate(const UpdateArgs& args) override
		{
			auto& T = GetComponent<TransformComponent>().Translation;
			using kb = Base::input::Keyboard;

			if (kb::isPress(BASE_KEY_D))
				T.x += Velocity * args.dt;
			if (kb::isPress(BASE_KEY_A))
				T.x -= Velocity * args.dt;
			if (kb::isPress(BASE_KEY_W))
				T.y += Velocity * args.dt;
			if (kb::isPress(BASE_KEY_S))
				T.y -= Velocity * args.dt;
		}
	};
}