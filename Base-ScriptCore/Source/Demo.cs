using System;

namespace Base
{
    public class Demo
    {
        public float FloatVar { get; set; }
        public Demo()
        {
            Console.WriteLine("Main constructor!");

            var vec = new Vector3(7, 3, 1);
            var res = Log(ref vec);

            Log(res.str(), 0);
            InternalCalls.NativeLogVector3Dot(ref vec, out Vector3 result);
            Console.WriteLine(result.str());
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
            InternalCalls.NativeLog(text, parameter);
        }

        public Vector3 Log(ref Vector3 parameter)
        {
            InternalCalls.NativeLogVector3(ref parameter, out Vector3 vec);
            return vec;
        }
    }
}
