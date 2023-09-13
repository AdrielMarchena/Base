using Base;

namespace Sandbox
{
    public class Player : Entity
    {
        void OnCreate()
        {
        }

        void OnUpdate(float ts)
        {
            float speed = 25.0f;
            Vector3 velocity = Vector3.Zero;

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

            Vector3 translation = Translation;
            translation += (velocity * speed * ts);
            Translation = translation;
        }
    }
}
