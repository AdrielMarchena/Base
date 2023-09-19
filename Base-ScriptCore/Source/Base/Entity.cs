using System;

namespace Base
{
    public class Entity
    {
        protected Entity() { Id = 0; }
        public readonly ulong Id;
        internal Entity(ulong id)
        {
            Id = id;
        }

        public Vector3 Translation
        {
            get
            {
                InternalCalls.TransformComponent_GetTranslation(Id, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.TransformComponent_SetTranslation(Id, ref value);
            }
        }

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.Entity_HasComponent(Id, componentType);
        }

        public void AddComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            InternalCalls.Entity_HasComponent(Id, componentType);
        }

        public void RemoveComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            InternalCalls.Entity_RemoveComponent(Id, componentType);
        }

        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { Entity = this };
            return component;
        }
    }
}
