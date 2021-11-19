#pragma once
#include "Entity.h"
#include "event/Events.h"
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
		virtual void OnDestroy(){}
		virtual void OnEvent(Event& e){}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}

