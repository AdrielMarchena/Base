using System;

namespace Base
{
    public class Main
    {
        public float FloatVar { get; set; }
        public Main()
        {
            Console.WriteLine("Main constructor!");
        }

        public void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
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
    }
}