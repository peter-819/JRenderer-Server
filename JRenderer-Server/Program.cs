using System;
using System.Runtime.InteropServices;
using System.Threading;

namespace Jerenderer_Server
{
    class Program
    {
        [DllImport("JEngine-Sandbox.dll")]
        public static extern void JEngineMain();
        static void Main(string[] args)
        {
            Thread thread = new Thread(() =>
            {
                JEngineMain();
            });
            thread.Start();
            Server.Start(50, 8080);
            while (true)
            {
                ThreadManager.UpdateMain();
                Thread.Sleep(10);
            }
        }
    }
}
