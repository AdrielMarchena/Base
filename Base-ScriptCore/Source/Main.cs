using System;
using System.Runtime.CompilerServices;

namespace Base
{

    public struct Vector3
    {
        public float X, Y, Z;

        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public string str()
        {
            return X + " " + Y + " " + Z;
        }
    }

    public class Main
    {
        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("Main constructor!");

            var vec = new Vector3(7, 3, 1);
            var res = Log(ref vec);

            Log(res.str(), 0);
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");

            Log("AdrielMarchena", 9742);
        }

        public void PrintInts(int a, int b)
        {
            Console.WriteLine($"C# says int number: {a} and {b}");
        }

        public void PrintInt(int message)
        {
            Console.WriteLine($"C# says int number: {message}");
        }

        public void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

        public void Log(string text, int parameter)
        {
            NativeLog(text, parameter);
        }

        public Vector3 Log(ref Vector3 parameter)
        {
            NativeLogVector3(ref parameter, out Vector3 vec);
            return vec;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLog(string text, int parameter);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void NativeLogVector3(ref Vector3 parameter, out Vector3 result);
    }
}