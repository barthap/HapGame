using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Hapex.HapGame.Editor
{
    public partial class frmRenderer : Form
    {
        public frmRenderer()
        {
            InitializeComponent();
        }

        private void frmRenderer_Load(object sender, EventArgs e)
        {
            Hapex.HapGame.Editor.WPFRenderer.MainControl ctrl = elementHost1.Child as Hapex.HapGame.Editor.WPFRenderer.MainControl;
            ctrl.setEsenthelDrawing();
        }

        public void LoadEE() { Hapex.HapGame.Editor.WPFRenderer.MainControl ctrl = elementHost1.Child as Hapex.HapGame.Editor.WPFRenderer.MainControl; ctrl.OnLoaded(); }

        public void frmRenderer_FormClosing(object sender, FormClosingEventArgs e)
        {
            Hapex.HapGame.Editor.WPFRenderer.MainControl ctrl = elementHost1.Child as Hapex.HapGame.Editor.WPFRenderer.MainControl;
            ctrl.Kill();
        }
    }
}
