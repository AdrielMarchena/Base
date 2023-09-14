namespace Base
{
    public class Input
    {
        public Input() { }
        public static bool IsPressed(KeyCode code)
        {
            return InternalCalls.Input_IsKeyDown(code);
        }
    }
}
