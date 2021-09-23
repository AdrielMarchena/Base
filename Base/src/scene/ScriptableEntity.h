#pragma once
#include "Entity.h"
namespace Base
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity(){}
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate(){}
		virtual void OnAwake(){}
		virtual void OnUpdate(const UpdateArgs& args){}
		//Render extra things here without a entity to it
		virtual void ExtraRender(){}
		virtual void OnDestroy(){}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}

