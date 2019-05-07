namespace Lab3.CriticalSection
{
    public interface ICriticalSection
    {
        void Enter();
        bool TryEnter(int timeout);
        void SetSpinCount(uint count);
        void Leave();
    }
}