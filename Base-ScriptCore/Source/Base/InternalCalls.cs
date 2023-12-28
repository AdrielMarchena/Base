using System;
using System.Runtime.CompilerServices;

namespace Base
{
    internal static class InternalCalls
    {
        #region Demo
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void NativeLogVector3(ref Vector3 parameter, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float NativeLogVector3Dot(ref Vector3 parameter, out Vector3 result);
        #endregion

        #region TransformComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_GetTranslation(ulong entityId, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void TransformComponent_SetTranslation(ulong entityId, ref Vector3 translation);
        #endregion

        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Entity_HasComponent(ulong entityId, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_GetComponent(ulong entityId, Type componentType);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Entity_RemoveComponent(ulong entityId, Type componentType);
        #endregion

        #region RigidBody2DComponent
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetImpulse(ulong entityId, ref Vector2 impulse, ref Vector2 point, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBody2DComponent_SetImpulseToCenter(ulong entityId, ref Vector2 impulse, bool wake);
        #endregion

        #region Scene
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong Scene_CreateEntityWithUUID(string entityName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void Scene_CreateEntityWithUUID(ulong entityId, string entityName);
        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool Input_IsKeyDown(KeyCode key);
        #endregion
    }
}