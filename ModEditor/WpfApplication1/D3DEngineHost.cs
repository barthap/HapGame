using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Interop;
using System.Runtime.InteropServices;
using Win32;
using Hapex.HapGame.EditorWrapper;

namespace Hapex.HapGame.Editor.WPFRenderer
{
    public class Dryas3DEngineHost : HwndHost
    {


        #region ---------------- DATA MEMBERS ----------------



        /// <summary>
        /// W32 child window
        /// </summary>
        private IntPtr _childHwnd;

        /// <summary>
        /// 3D engine
        /// </summary>
        public EngineWrapper _engine;


        public System.Windows.Rect WndBoundingBox;
        int _width, _height;

        // This member is required to prevent the MyWndProc() method from being 
        // disposed by the Garbage Collector
        
        private Win32.User32.WndProc _CallbackOnCollectedDelegateWindowProc;


        #endregion


        #region ---------------- INITIALISATION --------------

        /// <summary>
        /// Ctr
        /// </summary>
        public Dryas3DEngineHost(EngineWrapper engine, int width, int height)
        {
            _engine = engine;
            _width = width;
            _height = height;
        }





        #endregion


        #region ---------------- OPERATIONS --------------



        /// <summary>
        /// Creates child window and D3D device
        /// </summary>
        /// <param name="hwndParent"></param>
        /// <returns></returns>
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {

            IntPtr hInstance = Marshal.GetHINSTANCE(typeof(System.Windows.Application).Module);
            string wndClassName = "NetDCWin323DEngineClass";
            string wndName = "WinRender";

            _CallbackOnCollectedDelegateWindowProc = new User32.WndProc(MyWndProc);

            User32.WNDCLASSEX wcex = new User32.WNDCLASSEX()
            {
                cbSize = Marshal.SizeOf(typeof(User32.WNDCLASSEX)),
                style = User32.CS.CS_HREDRAW | User32.CS.CS_VREDRAW,
                lpfnWndProc = _CallbackOnCollectedDelegateWindowProc,
                cbClsExtra = 0,
                cbWndExtra = 0,
                hInstance = hInstance,
                hIcon = IntPtr.Zero,
                hbrBackground = (IntPtr)(GDI32.COLOR.COLOR_WINDOW + 1),
                hCursor = User32.LoadCursor(IntPtr.Zero, User32.IDC.IDC_ARROW),
                lpszClassName = wndClassName,
                lpszMenuName = null,
                hIconSm = IntPtr.Zero
            };

            short atom = User32.RegisterClassEx(ref wcex);

            if (atom == 0)
                throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error());

            IntPtr hWnd = User32.CreateWindowEx(
              User32.WS_EX.WS_EX_NONE,
              wndClassName,
              wndName,
              User32.WS.WS_CHILD | User32.WS.WS_VISIBLE,
              User32.CW_USEDEFAULT, User32.CW_USEDEFAULT,
              640,480,
              hwndParent.Handle,
              IntPtr.Zero,
              hInstance,
              IntPtr.Zero);

            if (hWnd == IntPtr.Zero)
                throw new System.ComponentModel.Win32Exception(Marshal.GetLastWin32Error()); //Cannot find window class???

            _childHwnd = hWnd;

          



            return new HandleRef(this, _childHwnd);
        }

     

        int lastX = 0;
        int lastY = 0;
        private int MyWndProc(IntPtr hWnd, uint msg, uint wParam, uint lParam)
        {

            switch (msg)
            {
                case (int)User32.WM.WM_SIZE:
                    {
                //        _engine.Resize(User32.LoWord((int)lParam), User32.HiWord((int)lParam));
                    } break;
                case (int)User32.WM.WM_MOUSEMOVE:
                    if (User32.LoWord((int)wParam) == User32.MK_LBUTTON)
                    {
                        int x = User32.LoWord((int)lParam);
                        int y = User32.HiWord((int)lParam);
                        int deltaX = x - lastX;
                        int deltaY = y - lastY;

                     //   _engine.InputManager.Mouse(deltaX, deltaY, 0, true, false);
                        lastX = x;
                        lastY = y;
                    }

                    break;
                case (int)User32.WM.WM_LBUTTONDOWN:
                    {
                        int x = User32.LoWord((int)lParam);
                        int y = User32.HiWord((int)lParam);
                        lastX = x;
                        lastY = y;
                        User32.SetFocus(_childHwnd);
                    } break;
                case (int)User32.WM.WM_KEYDOWN:
                    //  Debug.Print("KEY" + wParam);
                   //engine.InputManager.Key((int)wParam);


                    break;


                //Debug.Print("WM_LBUTTONDOWN"); break;
                case (int)User32.WM.WM_MOUSEWHEEL:
                    //Debug.Print("WM_MOUSEWHEEL");
                    /*GDI32.RECT rect = new GDI32.RECT();
                    User32.GetClientRect(hWnd, out rect);

                    GDI32.PAINTSTRUCT ps = new GDI32.PAINTSTRUCT();
                    IntPtr hdc = User32.BeginPaint(hWnd, out ps);

                    User32.DrawText(hdc, "Hello, World!", -1, ref rect,
                        User32.DT.DT_CENTER | User32.DT.DT_VCENTER | User32.DT.DT_SINGLELINE);

                    User32.EndPaint(hWnd, out ps);
                    */
                    break;

                /* case (int)User32.WM.WM_DESTROY:
                     User32.PostQuitMessage(0);
                     break;*/

                default:
                    return User32.DefWindowProc(hWnd, msg, wParam, lParam);
            }

            return 0;
        }

        protected override void DestroyWindowCore(HandleRef hwnd)
        {
       //     _engine.ShutdownEngine();
            // HwndHost will dispose the _childHwnd for us
        }

        #endregion


        #region ---------------- EVENTS ----------------------
        #endregion


        #region ---------------- PROPERTIES ------------------


        /// <summary>
        /// W32 child window
        /// </summary>
        public IntPtr ChildHwnd
        {
            get { return _childHwnd; }
        }



        #endregion

    }


}
