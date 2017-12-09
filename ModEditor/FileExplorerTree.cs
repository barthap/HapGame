using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
using DevComponents.AdvTree;
using DevComponents.DotNetBar;

namespace Hapex.HapGame.Editor
{

    public partial class FileExplorerTree : UserControl
    {
        [Description("Event wywoływany podczas podwójnego kliknięcia myszy na Node")]
        public event EventHandler BrowserNodeDoubleClick;

        [Description("Event wywoływany podczas podwójnego kliknięcia myszy na Node")]
        public event EventHandler BrowserNodeClick;

        private ElementStyle _RightAlignFileSizeStyle = null;

        [Category("Explorer Tree")]
        [Description("Odwołanie do samego drzewa.")]
        [Browsable(true)]
        public DevComponents.AdvTree.AdvTree ExplorerTree
        {
            get { return explorerTree; }
        }

        public string GamePath;

        bool toRefresh;

        public FileExplorerTree()
        {
            toRefresh = false;
            InitializeComponent();
        }

        public void FileExplorerTree_Load(object sender, EventArgs e)
        {
            DInfo d = new DInfo();
            DInfo d2 = new DInfo();
            
            explorerTree.BeginUpdate();

            {
                DirectoryInfo driveInfo = new DirectoryInfo(GamePath + @"\Mods");
                d.dir = driveInfo;
                d.isRoot = true;

                Node node = new Node();
                node.Tag = d;
                node.Text = driveInfo.Name.Replace(@"\", "");
                node.Image = global::Hapex.HapGame.Editor.Properties.Resources.Harddrive;
                node.Cells.Add(new Cell("Mody"));
                node.Cells.Add(new Cell());
                explorerTree.Nodes.Add(node);
                // We will load drive content on demand
                node.ExpandVisibility = eNodeExpandVisibility.Visible;
            }
            {
                DirectoryInfo driveInfo2 = new DirectoryInfo(GamePath + @"\Scripts");
                d2.dir = driveInfo2;
                d2.isRoot = true;

                Node node = new Node();
                node.Tag = d2;
                node.Text = driveInfo2.Name.Replace(@"\", "");
                node.Image = global::Hapex.HapGame.Editor.Properties.Resources.Harddrive;
                node.Cells.Add(new Cell("Skrypty"));
                node.Cells.Add(new Cell());
                explorerTree.Nodes.Add(node);
                // We will load drive content on demand
                node.ExpandVisibility = eNodeExpandVisibility.Visible;
            }
            // Enable tree layout and display updates, performs any pending layout and display updates
            explorerTree.EndUpdate();

            _RightAlignFileSizeStyle = new ElementStyle();
            _RightAlignFileSizeStyle.TextAlignment = DevComponents.DotNetBar.eStyleTextAlignment.Far;
        }

        private void fileExplorer_BeforeExpand(object sender, DevComponents.AdvTree.AdvTreeNodeCancelEventArgs e)
        {
            Node parent = e.Node;
            if (parent.Nodes.Count > 0)
            {
                if (toRefresh)
                    parent.Nodes.Clear();
                else return;
            }

            toRefresh = false;
            if (parent.Tag is DInfo)
            {
                explorerTree.BeginUpdate();
                DInfo driveInfo = (DInfo)parent.Tag;
                LoadDirectories(parent, driveInfo.dir);
                parent.ExpandVisibility = eNodeExpandVisibility.Auto;
                explorerTree.EndUpdate(true);
            }
            else if (parent.Tag is DirectoryInfo)
            {
                LoadDirectories(parent, (DirectoryInfo)parent.Tag);
            }
        }
        private void LoadDirectories(Node parent, DirectoryInfo directoryInfo)
        {
            //katalogi
            DirectoryInfo[] directories = directoryInfo.GetDirectories();
            foreach (DirectoryInfo dir in directories)
            {
               // DirectoryInfo driveInfo = new DirectoryInfo(GamePath + @"\Mods");

                if ((dir.Attributes & FileAttributes.Hidden) == FileAttributes.Hidden) continue;
                Node node = new Node();
                node.Tag = dir;
                node.Text = dir.Name;
                if (parent.Tag is DInfo && directoryInfo.FullName.Contains("Mods"))
                {
                    node.Cells.Add(new Cell("Mod")); //typ
                    node.Image = global::Hapex.HapGame.Editor.Properties.Resources.folder_table;
                    node.ImageExpanded = node.Image;
                    node.Checked = true;
                }
                else
                {
                    node.Cells.Add(new Cell("Katalog")); //typ
                    node.Image = global::Hapex.HapGame.Editor.Properties.Resources.FolderClosed;
                    node.ImageExpanded = global::Hapex.HapGame.Editor.Properties.Resources.FolderOpen;
                }

                node.Cells.Add(new Cell()); //rozmiar
                node.ExpandVisibility = eNodeExpandVisibility.Visible;
                parent.Nodes.Add(node);
            }

            //pliki
            FileInfo[] files = directoryInfo.GetFiles();
            foreach (FileInfo file in files)
            {
                Node node = new Node();
                node.Tag = file;
                node.Text = file.Name;
                node.Image = global::Hapex.HapGame.Editor.Properties.Resources.File;
                node.Cells.Add(new Cell("Plik " + Path.GetExtension(file.FullName))); //typ
                Cell cell = new Cell(file.Length.ToString("N0") + ""); // rozmiar
                cell.StyleNormal = _RightAlignFileSizeStyle;
                node.Cells.Add(cell);

                parent.Nodes.Add(node);
            }
        }

        private void explorerTree_NodeDoubleClick(object sender, TreeNodeMouseEventArgs e)
        {
            if (BrowserNodeDoubleClick != null)
                BrowserNodeDoubleClick(sender, e as EventArgs);

            
        }

        private void explorerTree_NodeClick(object sender, TreeNodeMouseEventArgs e)
        {
            MouseButtons ms = e.Button;
            if(ms == MouseButtons.Right)
                if (BrowserNodeClick != null)
                    BrowserNodeClick(sender, e as EventArgs);
        }

        public override void Refresh()
        {
            base.Refresh();

            explorerTree.CollapseAll();
            toRefresh = true;
            explorerTree.Refresh();
            explorerTree.RefreshItems();
            explorerTree.Nodes[0].Expand();
        }
    }
    class DInfo
    {
        public DirectoryInfo dir;
        public bool isRoot;
    }
}
