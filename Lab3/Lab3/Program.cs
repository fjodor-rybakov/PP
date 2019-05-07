using System;
using System.Collections.Generic;
using System.Threading;
using Lab3.Assets;

namespace Lab3
{
    internal static class Program
    {
        private static double _pi;
        private const int CountThreads = 4;

        private static CriticalSection.CriticalSection _criticalSection;
        private static int _timeout;

        public static void Main(string[] args)
        {
            if (args[0] == null && args[1] == null && args[2] == null)
            {
                Console.WriteLine("Invalid number of arguments. Must: Lab3.exe <count steps> <timeout> <count try cs>");
                return;
            }

            if (!int.TryParse(args[0], out var countSteps))
            {
                Console.WriteLine("Incorrect value steps. Value must be a number");
                return;
            }
            
            if (!int.TryParse(args[1], out _timeout))
            {
                Console.WriteLine("Incorrect value timeout. Value must be a number");
                return;
            }
            
            if (!uint.TryParse(args[2], out var countTry))
            {
                Console.WriteLine("Incorrect value count try. Value must be a number");
                return;
            }
            
            _criticalSection = new CriticalSection.CriticalSection(countTry);

            var watch = System.Diagnostics.Stopwatch.StartNew();

            var handles = CreateThreads(countSteps, TryType.TryEnter);
            foreach (var tr in handles) tr.Join();

            watch.Stop();
            var elapsedMs = watch.ElapsedMilliseconds;

            Console.WriteLine("Pi: " + _pi + ", Work time: " + elapsedMs + "ms");
            _pi = 0;

            watch = System.Diagnostics.Stopwatch.StartNew();

            handles = CreateThreads(countSteps, TryType.Enter);
            foreach (var tr in handles) tr.Join();

            watch.Stop();
            elapsedMs = watch.ElapsedMilliseconds;

            Console.WriteLine("Pi: " + _pi + ", Work time: " + elapsedMs + "ms");
        }

        private static IEnumerable<Thread> CreateThreads(int countSteps, string tryType)
        {
            var waitHandles = new List<Thread>();
            var countStepsPerThread = countSteps / CountThreads;
            for (var i = 0; i < CountThreads; i++)
            {
                var newThread = new Thread(CalcPi);
                newThread.Start(new ThreadContext
                {
                    Left = i * countStepsPerThread,
                    Right = (i + 1) * countStepsPerThread,
                    Step = 1.0 / countSteps,
                    Name = $"Thread: {i}",
                    TryType = tryType
                });
                waitHandles.Add(newThread);
            }

            return waitHandles;
        }

        private static void CalcPi(object param)
        {
            var threadContext = (ThreadContext) param;
            for (var i = threadContext.Left; i < threadContext.Right; i++)
            {
                var x = (i + 0.5) * threadContext.Step;
                if (threadContext.TryType == TryType.Enter) _criticalSection.Enter();
                else
                {
                    while (!_criticalSection.TryEnter(_timeout))
                    {
                    }
                }

                _pi += 4.0 / (1.0 + x * x) * threadContext.Step;
                _criticalSection.Leave();
            }
        }
    }
}