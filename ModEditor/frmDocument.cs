using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Hapex.HapGame.Editor
{
    public partial class frmDocument : Form
    {
        public string FileName = "";
        public bool DocumentChanged = false;
        private int linesPrinted;
        private string[] lines;

        private void DocumentChange(bool changed)
        {
            this.DocumentChanged = changed;
            if (changed && this.Text.Substring(this.Text.Length - 1) != "*")
                this.Text += "*";
            else if (!changed && this.Text.Substring(this.Text.Length - 1) == "*")
                this.Text = this.Text.Substring(0, this.Text.Length - 1);
        }

        public frmDocument()
        {
            InitializeComponent();
            this.codeTextBox.CursorPositionChanged += new System.EventHandler(this.codeTextBox1_CursorPositionChanged);
        }
        public void OpenFile(string file_name)
        {
            this.codeTextBox.LoadFile(file_name, RichTextBoxStreamType.PlainText);
            this.codeTextBox.UpdateSyntaxHightlight();
            this.FileName = file_name;
            DocumentChange(false);
        }
        public void SaveFile()
        {
            this.codeTextBox.SaveFile(this.FileName, RichTextBoxStreamType.PlainText);
            DocumentChange(false);
        }

        private void commandUndo_Executed(object sender, EventArgs e)
        {
            codeTextBox.Undo();
        }

        private void commandRedo_Executed(object sender, EventArgs e)
        {
            codeTextBox.Redo();
        }

        private void commandCut_Executed(object sender, EventArgs e)
        {
            codeTextBox.Cut();
        }

        private void commandCopy_Executed(object sender, EventArgs e)
        {
            codeTextBox.Copy();
        }

        private void commandPaste_Executed(object sender, EventArgs e)
        {
            codeTextBox.Paste();
        }

        private void OnTextChanged(object sender, EventArgs e)
        {
            DocumentChange(true);
        }

        private void UpdateStatusPosition(int line, int col, int pos)
        {

            frmMain form = this.MdiParent as frmMain;

            form.positionLabel.Text = "Ln " +
                        line + ", Col " + col + ", Pos " + pos;
        }

        private void commandShowIntellisense_Executed(object sender, EventArgs e)
        {
            codeTextBox.ShowIntellisense();
        }

        private void codeTextBox1_CursorPositionChanged(object sender, EventArgs e)
        {
            int line = codeTextBox.CurrentLine;
            int col = codeTextBox.CurrentColumn;
            int pos = codeTextBox.CurrentPosition;
            UpdateStatusPosition(line, col, pos);
        }

        private void frmDocument_Load(object sender, EventArgs e)
        {
            frmMain form = this.MdiParent as frmMain;
            form.positionLabel.Text = "Ln 0, Col 0, Pos 0";
        }

       
    }
}
