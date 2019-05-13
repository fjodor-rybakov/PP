using System;
using System.Threading;

namespace Lab3.CriticalSection
{
    public class CriticalSection : ICriticalSection
    {
        private static Mutex Mut { get; } = new Mutex();
        private static uint _spinCount;

        public CriticalSection(uint count)
        {
            _spinCount = count;
        }
        
        public void Enter()
        {
            var numAttempt = 0;
            if (Mut.WaitOne(0, false)) return;
            while (!Mut.WaitOne(0, false))
            {
                numAttempt++;
                if (numAttempt != _spinCount) continue;
                numAttempt = 0;
                Thread.Sleep(10);
            }
        }

        public bool TryEnter(int timeout)
        {
            var numAttempt = 0;
            var startTime = DateTime.Now.Millisecond;
            if (Mut.WaitOne(0, false)) return true;
            while (!Mut.WaitOne(0, false))
            {
                numAttempt++;
                if (DateTime.Now.Millisecond >= startTime + timeout) return false;
                if (numAttempt != _spinCount) continue;
                numAttempt = 0;
                Thread.Sleep(10);
            }

            return true;
        }

        public void SetSpinCount(uint count)
        {
            _spinCount = count;
        }

        public void Leave()
        {
            Mut.ReleaseMutex();
        }
    }
}