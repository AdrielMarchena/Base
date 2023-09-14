namespace Base
{
    public class RigidBody2DComponent : Component
    {
        public void ApplyLinearImpulse(Vector2 impulse, Vector2 point, bool wake)
        {
            InternalCalls.RigidBody2DComponent_SetImpulse(Entity.Id, ref impulse, ref point, wake);
        }

        public void ApplyLinearImpulseToCenter(Vector2 impulse, bool wake)
        {
            InternalCalls.RigidBody2DComponent_SetImpulseToCenter(Entity.Id, ref impulse, wake);
        }
    }
}
