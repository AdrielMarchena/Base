using Base;
using System;

namespace Sandbox
{
    public class Player : Entity
    {
        private TransformComponent m_Transform;
        private RigidBody2DComponent m_Body;
        void OnCreate()
        {
            m_Transform = GetComponent<TransformComponent>();
            m_Body = GetComponent<RigidBody2DComponent>();
            bool hasC = HasComponent<TransformComponent>();

            Console.WriteLine($"{hasC}");
            // m_Transform.Translation = Vector3.Zero;
        }

        void OnUpdate(float ts)
        {
            float speed = 25.0f;
            Vector2 velocity = Vector2.Zero;

            if (Input.IsPressed(KeyCode.Up))
            {
                velocity.Y = 1;
            }
            else if (Input.IsPressed(KeyCode.Down))
            {
                velocity.Y = -1;
            }

            if (Input.IsPressed(KeyCode.Left))
            {
                velocity.X = -1;
            }
            else if (Input.IsPressed(KeyCode.Right))
            {
                velocity.X = 1;
            }

            m_Body.ApplyLinearImpulseToCenter(velocity * speed * ts, true);

            // Vector3 translation = Translation;
            // translation += new Vector3(velocity * speed * ts, 0);
            // Translation = translation;
        }
    }
}
