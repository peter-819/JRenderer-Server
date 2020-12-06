using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace JRenderer_Server.src
{
    class Input
    {
        public class Mouse
        {
            public int X { get; set; }
            public int Y { get; set; }
            public byte[] IsPressed;
            public Mouse()
            {
                IsPressed = new byte[2];
            }
        }
        public class Keyboard
        {
            public byte[] KeyState;
            public Keyboard()
            {
                KeyState = new byte[350];
            }
        }

        static public Mouse mouse = new Mouse();
        static public Keyboard keyboard = new Keyboard();
        
    }
}
