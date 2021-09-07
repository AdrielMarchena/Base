#pragma once
#include "Entity.h"
namespace en
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
		virtual void OnUpdate(const UpdateArgs& args){}
		virtual void OnDestroy(){}

	private:
		Entity m_Entity;
		friend class Scene;
	};
}

