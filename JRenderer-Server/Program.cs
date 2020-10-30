using System;
using System.Threading;

namespace Jerenderer_Server
{
    class Program
    {
        static void Main(string[] args)
        {
            Server.Start(50, 8080);
            while (true)
            {
                ThreadManager.UpdateMain();
                Thread.Sleep(10);
            }
        }
    }
}
