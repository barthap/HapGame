using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DevComponents.DotNetBar;
using System.IO;
using System.Diagnostics;
using DevComponents.AdvTree;
using System.Runtime.InteropServices;
using System.Windows.Interop;
using Win32;
using System.Windows;
using Hapex.HapGame.Editor;

namespace Hapex.HapGame.Editor
{
    public partial class frmMain : DevComponents.DotNetBar.Office2007Form
    {
        public string gamePath;

        public DirectoryInfo actualModDir;

        public frmMain()
        {

            gamePath = frmMain.LoadGamePath();

            InitializeComponent();

            fileExplorerTree1.GamePath = gamePath;

            fileExplorerTree1.FileExplorerTree_Load(null, null);

           
        }


        //aktualizacja widoczności tego, co nie odwołuje sie do Document bezpośrednio przez Command
        private void EnableFileItems()
        {
            // Accessing items through the Items collection and setting the properties on them
            // will propagate certain properties to all items with the same name...
            bool enabled = true;
            if (this.ActiveMdiChild == null)
            {
                enabled = false;
            }


            saveMenuItem.Enabled = enabled;
            saveAsMenuItem.Enabled = enabled;
            closeMenuItem.Enabled = enabled;
        }

        //aktualizacja odwołań Command do Document
        private void BindDocumentCommands()
        {
            frmDocument document = this.ActiveMdiChild as frmDocument;
            if (document == null)
            {
                // Note that when Command is set to null the button will be automatically 
                // disabled if it had command associated previously
                cutMenuItem.Command = null;
                cutMenuItem.Enabled = false;
                copyMenuItem.Command = null;
                copyMenuItem.Enabled = false;
                pasteMenuItem.Command = null;
                pasteMenuItem.Enabled = false;
                undoMenuItem.Command = null;
                undoMenuItem.Enabled = false;
                redoMenuItem.Command = null;
                redoMenuItem.Enabled = false;

                positionLabel.Text = null;
            }
            else
            {

                cutMenuItem.Command = document.commandCut;
                cutMenuItem.Enabled = true;
                copyMenuItem.Command = document.commandCopy;
                copyMenuItem.Enabled = true;
                pasteMenuItem.Command = document.commandPaste;
                pasteMenuItem.Enabled = true;
                undoMenuItem.Command = document.commandUndo;
                undoMenuItem.Enabled = true;
                redoMenuItem.Command = document.commandRedo;
                redoMenuItem.Enabled = true;

            }
        }

        private void OnMdiChildActivate(object sender, EventArgs e)
        {
            EnableFileItems();
            BindDocumentCommands();
        }

        private void cmdNew_executed(object sender, EventArgs e)
        {
            CreateNewDocument();
        }

        private void CreateNewDocument()
        {
            frmDocument doc = new frmDocument();
            doc.MdiParent = this;
            doc.WindowState = FormWindowState.Maximized;
            doc.Show();
            doc.Update();
            doc.Text = "Document " + this.MdiChildren.Length.ToString();
        }

        private void OpenDocument()
        {
            openFileDialog1.FileName = "";
            openFileDialog1.Filter = "Wszystkie skrypty (*.lua, *.gs, *.qst, *.dlg, *.mod)|*.lua;*.gs;*.qst;*.dlg;*.mod|"
                + "Pliki LUA (*.lua)|*.lua|"
                + "Pliki GameScript (*.gs)|*.gs|"
                + "Pliki Questów (*.qst)|*.qst|"
                + "Pliki Dialogów (*.dlg)|*.dlg|"
                + "Pliki Modów (*.mod)|*.mod|"
                + "Wszystkie pliki (*.*)|*.*";
            openFileDialog1.Title = "Otwórz skrypt gry";
            openFileDialog1.ShowDialog();
            if (openFileDialog1.FileName == "")
                return;
            frmDocument doc = new frmDocument();
            doc.Text = Path.GetFileName(openFileDialog1.FileName);
            doc.MdiParent = this;
            doc.WindowState = FormWindowState.Maximized;
            doc.Show();
            doc.OpenFile(openFileDialog1.FileName);
         }

        internal void SaveDocument(frmDocument doc)
        {
            saveFileDialog1.Filter = "Wszystkie pliki (*.*)|*.*"
    + "Pliki LUA (*.lua)|*.lua|"
    + "Pliki GameScript (*.gs)|*.gs|"
    + "Pliki Questów (*.qst)|*.qst|"
    + "Pliki Modów (*.mod)|*.mod|"
    + "Pliki Dialogów (*.dlg)|*.dlg|";
            saveFileDialog1.Title = "Zapisz skrypt gry";

            if (doc.FileName == "")
            {
                if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                    return;
                if (saveFileDialog1.FileName == "")
                    return;
                doc.FileName = saveFileDialog1.FileName;

                doc.Text = Path.GetFileName(saveFileDialog1.FileName);
            }
            if (doc.FileName != "")
                doc.SaveFile();

        }

        private void commandSave_Executed(object sender, EventArgs e)
        {
            frmDocument doc = this.ActiveMdiChild as frmDocument;
            if (doc == null || !doc.DocumentChanged)
                return;
            SaveDocument(doc);
        }

        private void commandSaveAs_Executed(object sender, EventArgs e)
        {
            if (this.ActiveMdiChild == null)
                return;

            // Close menu popup if needed since File Dialogs can interfer with it
            if (sender is BaseItem)
                BaseItem.CollapseAll(sender as BaseItem);

            frmDocument doc = this.ActiveMdiChild as frmDocument;
            if (doc == null || !doc.DocumentChanged)
                return;
            if (doc.FileName == "")
            {
                SaveDocument(doc);
                return;
            }

            saveFileDialog1.ShowDialog();
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            if (saveFileDialog1.FileName == "")
                return;
            doc.FileName = saveFileDialog1.FileName;
            doc.SaveFile();
            doc.Text = Path.GetFileName(saveFileDialog1.FileName);
        }

        private void openMenuItem_Click(object sender, EventArgs e)
        {
            OpenDocument();
        }

        private void frmMain_Load(object sender, EventArgs e)
        {
            CreateNewDocument();
            EnableFileItems();           

        }

        private void cascadeMenuItem_click(object sender, EventArgs e)
        {
            this.LayoutMdi(MdiLayout.Cascade);
        }

        private void tileVerticalMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(MdiLayout.TileVertical);
        }

        private void tileHorizontalMenuItem_Click(object sender, EventArgs e)
        {
            this.LayoutMdi(MdiLayout.TileHorizontal);
        }

        private void exitBtn_Click(object sender, EventArgs e)
        {
            this.Close();
            System.Windows.Forms.Application.Exit();
        }

        private void fileExplorerTree1_OnBrowserNodeDoubleClick(object sender, EventArgs e)
        {
            if (!(fileExplorerTree1.ExplorerTree.SelectedNode.Tag is FileInfo)) return;

            FileInfo dir = fileExplorerTree1.ExplorerTree.SelectedNode.Tag as FileInfo;

            frmDocument doc = new frmDocument();
            doc.Text = dir.Name;
            doc.MdiParent = this;
            doc.WindowState = FormWindowState.Maximized;
            doc.Show();
            doc.Update();
            doc.OpenFile(dir.FullName);
        }
        private void fileExplorer1_OnBrowserNodeDoubleClick(object sender, EventArgs e)
        {
            if (!(fileExplorer1.ExplorerTree.SelectedNode.Tag is FileInfo)) return;

            FileInfo dir = fileExplorer1.ExplorerTree.SelectedNode.Tag as FileInfo;

            frmDocument doc = new frmDocument();
            doc.Text = dir.Name;
            doc.MdiParent = this;
            doc.WindowState = FormWindowState.Maximized;
            doc.Show();
            doc.Update();
            doc.OpenFile(dir.FullName);
        }

        public void Compile(string fname)
        {
            string fileNameCompiled = Path.GetDirectoryName(fname) + "\\" + Path.GetFileNameWithoutExtension(fname)
                + "_" + Path.GetExtension(fname);
            try
            {
                Process.Start("Tools\\luac.exe", "-o " + fileNameCompiled + " " + fname);
            }
            catch (System.IO.FileNotFoundException ex)
            {
                MessageBoxEx.Show("Nie znaleziono pliku Tools\\Luac.exe. Ponowne zainstalowanie aplikacji" +
                "powinno rozwiązać ten problem\n" +
                "Oryginalna treść błędu: " + ex.Message,
                "Błąd! Nie znaleziono pliku", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void buttonItem7_Click(object sender, EventArgs e)
        {
            frmDocument doc = this.ActiveMdiChild as frmDocument;
            if(doc == null)return;
            if (doc.FileName == "") MessageBoxEx.Show("Before compilation script must be saved", "Information",
                MessageBoxButtons.OK, MessageBoxIcon.Information);
            else
                Compile(doc.FileName);
        }

        public static void SaveGamePath(string GamePath)
        {
            StreamWriter sw = new StreamWriter(@"game_path.dat");
            sw.WriteLine(GamePath);
            sw.Close();
        }
        public static string LoadGamePath()
        {
            StreamReader reader;
            string path;
            try
            {
                reader = new StreamReader(@"game_path.dat");
                path = reader.ReadLine();
                if (path == "")
                {
                    MessageBoxEx.Show("Bad game path. You have to set correct path now!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                }
                reader.Close();
            }
            catch(FileNotFoundException)
            {
                MessageBoxEx.Show("File game_path.dat not found! You have to set it now!", "Błąd", MessageBoxButtons.OK, MessageBoxIcon.Error);
                path = "";
            }

            if(path == "")
            {
                FolderBrowserDialog dlg = new FolderBrowserDialog();

                while (true)
                {
                    if (dlg.ShowDialog() == DialogResult.OK) break;
                    else if(dlg.ShowDialog() == DialogResult.Cancel)
                        MessageBoxEx.Show("You have to set corret path");
                   
                    
                }

                path = dlg.SelectedPath;
                SaveGamePath(path);
            }
            
            return path;
        }

        private void settingsMenuItem_Click(object sender, EventArgs e)
        {
            using(SettingsDlg dlg = new SettingsDlg())
            {
                dlg.ShowDialog(this);
            }
        }

        private void gameRun_Click(object sender, EventArgs e)
        {
            Process.Start(gamePath + @"\HapGame.exe");
        }

        private void fileExplorerTree1_BrowserNodeClick(object sender, EventArgs e)
        {
            TreeNodeMouseEventArgs ea = e as TreeNodeMouseEventArgs;
            if (ea.Node.Checked == true)
            {
                actualModDir = ea.Node.Tag as DirectoryInfo; //pobiera informacje o directory moda, można pobrać pliki (np info.dat itp.)
                statusLabel.Text = "Selected Mod: " + actualModDir.Name;

                modPopup.Popup(frmMain.MousePosition);
            }

        }

        private void popModInfoDat_Click(object sender, EventArgs e)
        {
            frmDocument doc = new frmDocument();
            doc.Text = Path.GetFileName(actualModDir.Name + " info.dat");
            doc.MdiParent = this;
            doc.WindowState = FormWindowState.Maximized;
            doc.Show();
            doc.OpenFile(actualModDir.FullName + @"\info.dat");
        }

        private void newModBtn_Click(object sender, EventArgs e)
        {
            NewModName dlg = new NewModName();
            if(dlg.ShowDialog(this)==DialogResult.OK)
            {
                DirectoryInfo modsDir = new DirectoryInfo(gamePath + @"\Mods");
                DirectoryInfo newModDir = modsDir.CreateSubdirectory(dlg.mName.Text);

                StreamWriter infoDat = new StreamWriter(File.Create(newModDir.FullName + @"\info.dat"));
                infoDat.Write(global::Hapex.HapGame.Editor.Properties.Resources.InfoDatDefVal);
                infoDat.Close();

                StreamWriter mainMod = new StreamWriter(File.Create(newModDir.FullName + @"\main.mod"));
                mainMod.Write(global::Hapex.HapGame.Editor.Properties.Resources.MainModDefVal);
                mainMod.Close();
            }

            

            fileExplorerTree1.Refresh();
        }

        private void popDeleteMod_Click(object sender, EventArgs e)
        {
            if(actualModDir.Exists)
            {
                actualModDir.Delete(true);
                actualModDir = null;
                statusLabel.Text = "Mo mod selected";
            }
           

            fileExplorerTree1.Refresh();
        }

        private void popRunGameMod_Click(object sender, EventArgs e)
        {
            Process.Start(gamePath + @"\HapGame.exe", "-" + actualModDir.Name);
        }

        private void gameCmd_Click(object sender, EventArgs e)
        {
            GameCmd dlg = new GameCmd();
            if(dlg.ShowDialog(this)==DialogResult.OK)
            {
                Process.Start(gamePath + @"\HapGame.exe", dlg.mName.Text);
            }
        }

        private void runRenderer_Click(object sender, EventArgs e)
        {
       /*     frmRenderer rndr = new frmRenderer();
            rndr.MdiParent = this;
            rndr.WindowState = FormWindowState.Maximized;
            rndr.Show();
            rndr.Update();
            rndr.LoadEE();*/
        }

        private void dockSite1_Click(object sender, EventArgs e)
        {

        }

    }
}
