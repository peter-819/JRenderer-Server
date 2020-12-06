using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.Diagnostics;

namespace JRenderer_Server.src
{
    class Renderer
    {
        static public Renderer instance = null;
        public int ClientID { get; set; }
        static public void Create(int clientID, int width,int height)
        {
            if (instance != null)
            {
                Debug.Assert(false);
            }
            else
            {
                instance = new Renderer(clientID, width, height);
            }
        }
        public delegate void SendDataCallback(IntPtr buffer, ulong size);
        private IntPtr hwnd = IntPtr.Zero;
        private Thread renderThread;
        public IntPtr RendererHandle;

        public int mWidth { get; set; }
        public int mHeight { get; set; }

        #region Win32_Dll_Import

        [DllImport("user32.dll", EntryPoint = "CreateWindowEx", CharSet = CharSet.Unicode)]
        internal static extern IntPtr CreateWindowEx(
            int dwExStyle,
            string lpszClassName,
            string lpszWindowName,
            int style,
            int x, int y,
            int width, int height,
            IntPtr hwndParent,
            IntPtr hMenu,
            IntPtr hInst,
            [MarshalAs(UnmanagedType.AsAny)] object pvParam
        );
        [DllImport("user32.dll", EntryPoint = "DestroyWindow", CharSet = CharSet.Unicode)]
        internal static extern bool DestroyWindow(IntPtr hwnd);
        internal const int WS_CHILD = 0x40000000, WS_VISIBLE = 0x10000000;

        #endregion

        #region JEngine_Dll_Import
        private const string JEngineDLL = "JEngine.dll";
        [DllImport(JEngineDLL)]
        public static extern IntPtr CreateJEngine(IntPtr hwnd, int width, int height);
        [DllImport(JEngineDLL)]
        public static extern void JEngineRun(IntPtr app);
        [DllImport(JEngineDLL, CallingConvention = CallingConvention.StdCall)]
        public static extern void SetSendDataCallback(SendDataCallback callback);

        [DllImport(JEngineDLL)]
        public static extern void OnWindowSize(IntPtr app, int width, int height);
        [DllImport(JEngineDLL)]
        public static extern void OnWindowClose(IntPtr app);
        [DllImport(JEngineDLL)]
        public static extern void OnKey(IntPtr app, int key, int scancode, int action, int mods);
        [DllImport(JEngineDLL)]
        public static extern void OnChar(IntPtr app, uint keycode);
        [DllImport(JEngineDLL)]
        public static extern void OnMouseButton(IntPtr app, int button, int action);
        [DllImport(JEngineDLL)]
        public static extern void OnScroll(IntPtr app, double xOffset, double yOffset);
        [DllImport(JEngineDLL)]
        public static extern void OnCursor(IntPtr app, double xPos, double yPos);
        [DllImport(JEngineDLL)]
        public static extern int OnCreate(IntPtr app, int shape,double scale,double x,double y,double z);
        [DllImport(JEngineDLL)]
        public static extern int OnCreateLight(IntPtr app, double als, double dls, double slr, double slp, double x, double y, double z);

        #endregion
        void SendDataCallbackFunc(IntPtr buffer, ulong size)
        {
            byte[] ImageBuffer = new byte[size];
            Marshal.Copy(buffer, ImageBuffer, 0, Convert.ToInt32(size));
            ServerSend.sendJPEGBuffer(ClientID,ImageBuffer);
        }
        public Renderer(int clientID, int width,int height)
        {
            this.ClientID = clientID;
            mWidth = width;
            mHeight = height;
        }
        private void Render(object data)
        {
            SetSendDataCallback(SendDataCallbackFunc);
            RendererHandle = CreateJEngine(hwnd, mWidth, mHeight);
            JEngineRun(RendererHandle);
        }
        public void Run()
        {
            renderThread = new Thread(new ParameterizedThreadStart(Render));
            renderThread.Start();
        }
        public void Shutdown()
        {
            renderThread.Join();
            DestroyWindow(hwnd);
        }
    }
}
