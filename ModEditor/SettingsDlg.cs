using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;

namespace Hapex.HapGame.Editor
{
    public partial class SettingsDlg : DevComponents.DotNetBar.Office2007Form
    {
        bool gamePathChanged;
        public SettingsDlg()
        {
            InitializeComponent();
            gamePathChanged = false;
            gamePath.Text = frmMain.LoadGamePath();
        }

        private void cancelBtn_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void okBtn_Click(object sender, EventArgs e)
        {
            if (gamePathChanged) MessageBoxEx.Show("Game path was changed. You have to restart editor to apply changes.", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
            frmMain.SaveGamePath(gamePath.Text);
            this.Close();
        }

        private void gamePathBrowseBtn_Click(object sender, EventArgs e)
        {
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                gamePath.Text = folderBrowserDialog1.SelectedPath;
                gamePathChanged = true;
            }
        }

        private void gamePath_TextChanged(object sender, EventArgs e)
        {
            if (!gamePathChanged) gamePathChanged = true;
        }
    }
}
