using System.Runtime.CompilerServices;

namespace Base
{
    public static class InternalCalls
    {
        #region Demo
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void NativeLogVector3(ref Vector3 parameter, out Vector3 result);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static float NativeLogVector3Dot(ref Vector3 parameter, out Vector3 result);
        #endregion

        #region Entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Entity_GetTranslation(ulong entityId, out Vector3 translation);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Entity_SetTranslation(ulong entityId, ref Vector3 translation);
        #endregion

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool Input_IsKeyDown(KeyCode key);
    }
}