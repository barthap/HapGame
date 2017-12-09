using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using Hapex.HapGame.EditorWrapper;

namespace Hapex.HapGame.Editor.WPFRenderer
{
    /// <summary>
    /// Interaction logic for MainControl.xaml
    /// </summary>
    public partial class MainControl : UserControl
    {
        EngineWrapper _engine;
        Dryas3DEngineHost _host;
        public MainControl()
        {
            InitializeComponent();


        }

        public void setEsenthelDrawing()
        {
            _engine = new EngineWrapper();


            _host = new Dryas3DEngineHost(_engine, (int)this.ActualWidth, (int)this.ActualHeight);

            PART_3DEngine.Child = _host;  
        }

        public void OnLoaded()
        {
            unsafe
            {
                IntPtr hhandle = Marshal.GetHINSTANCE(typeof(EngineWrapper).Module);

                _engine.Init(hhandle.ToPointer(), _host.ChildHwnd.ToPointer(), (int)this.ActualWidth, (int)this.ActualHeight);


            }
        }

        private void PART_3DEngine_Unloaded(object sender, RoutedEventArgs e)
        {
            this.Kill();
        }

        public void Kill()
        {
            _engine.Kill();
        }

        private void Grid_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            PART_3DEngine.Width = this.Width-2;
            PART_3DEngine.Height = this.Height - 2;
            PART_3DEngine.RenderSize = new Size(Width, Height);
        }
    }
}
