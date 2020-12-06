using System;
using System.Runtime.InteropServices;
using System.Threading;
using JRenderer_Server.src;

namespace Jerenderer_Server
{
    class Program
    {
        static void Main(string[] args)
        {
            Thread NetWorkThread = new Thread(() =>
            {
                Server.Start(50, 8080);
                while (true)
                {
                    ThreadManager.UpdateMain();
                    //Thread.Sleep(10);
                }
            });
            NetWorkThread.Start();
        }
    }
}
