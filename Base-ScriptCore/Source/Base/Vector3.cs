namespace Base
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0, 0, 0);

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public Vector3(Vector2 vec2, float z)
        {
            X = vec2.X;
            Y = vec2.Y;
            Z = z;
        }
        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator +(Vector3 vector, Vector3 scalar)
        {
            return new Vector3(vector.X + scalar.X, vector.Y + scalar.Y, vector.Z + scalar.Z);
        }

        public string str()
        {
            return X + " " + Y + " " + Z;
        }
    }

    public struct Vector2
    {
        public float X, Y;

        public static Vector2 Zero => new Vector2(0, 0);

        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public static Vector2 operator *(Vector2 vector, float scalar)
        {
            return new Vector2(vector.X * scalar, vector.Y * scalar);
        }

        public static Vector2 operator +(Vector2 vector, Vector2 scalar)
        {
            return new Vector2(vector.X + scalar.X, vector.Y + scalar.Y);
        }

        public string str()
        {
            return X + " " + Y;
        }
    }
}
