namespace Base
{
    public class Entity
    {
        protected Entity() { Id = 0; }
        public readonly ulong Id;
        internal Entity(ulong id)
        {
            Id = id;
        }

        public Vector3 Translation
        {
            get
            {
                InternalCalls.Entity_GetTranslation(Id, out Vector3 translation);
                return translation;
            }
            set
            {
                InternalCalls.Entity_SetTranslation(Id, ref value);
            }
        }
    }
}
