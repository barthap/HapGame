namespace Hapex.HapGame.Editor
{
    partial class FileExplorer
    {
        /// <summary> 
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary> 
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.explorerTree = new DevComponents.AdvTree.AdvTree();
            this.columnHeader1 = new DevComponents.AdvTree.ColumnHeader();
            this.columnHeader2 = new DevComponents.AdvTree.ColumnHeader();
            this.columnHeader3 = new DevComponents.AdvTree.ColumnHeader();
            this.nodeConnector1 = new DevComponents.AdvTree.NodeConnector();
            this.elementStyle1 = new DevComponents.DotNetBar.ElementStyle();
            this.elementStyle2 = new DevComponents.DotNetBar.ElementStyle();
            ((System.ComponentModel.ISupportInitialize)(this.explorerTree)).BeginInit();
            this.SuspendLayout();
            // 
            // explorerTree
            // 
            this.explorerTree.AccessibleRole = System.Windows.Forms.AccessibleRole.Outline;
            this.explorerTree.AllowDrop = true;
            this.explorerTree.BackColor = System.Drawing.SystemColors.Window;
            // 
            // 
            // 
            this.explorerTree.BackgroundStyle.Class = "TreeBorderKey";
            this.explorerTree.BackgroundStyle.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            this.explorerTree.Columns.Add(this.columnHeader1);
            this.explorerTree.Columns.Add(this.columnHeader2);
            this.explorerTree.Columns.Add(this.columnHeader3);
            this.explorerTree.Dock = System.Windows.Forms.DockStyle.Fill;
            this.explorerTree.Location = new System.Drawing.Point(0, 0);
            this.explorerTree.Name = "explorerTree";
            this.explorerTree.NodesConnector = this.nodeConnector1;
            this.explorerTree.NodeStyle = this.elementStyle1;
            this.explorerTree.PathSeparator = ";";
            this.explorerTree.SelectionBoxStyle = DevComponents.AdvTree.eSelectionStyle.FullRowSelect;
            this.explorerTree.Size = new System.Drawing.Size(365, 482);
            this.explorerTree.Styles.Add(this.elementStyle1);
            this.explorerTree.Styles.Add(this.elementStyle2);
            this.explorerTree.TabIndex = 0;
            this.explorerTree.Text = "advTree1";
            this.explorerTree.BeforeExpand += new DevComponents.AdvTree.AdvTreeNodeCancelEventHandler(this.fileExplorer_BeforeExpand);
            this.explorerTree.NodeClick += new DevComponents.AdvTree.TreeNodeMouseEventHandler(this.explorerTree_NodeClick);
            this.explorerTree.NodeDoubleClick += new DevComponents.AdvTree.TreeNodeMouseEventHandler(this.explorerTree_NodeDoubleClick);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Name = "columnHeader1";
            this.columnHeader1.Text = "Nazwa";
            this.columnHeader1.Width.Absolute = 150;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Name = "columnHeader2";
            this.columnHeader2.Text = "Typ";
            this.columnHeader2.Width.Absolute = 75;
            // 
            // columnHeader3
            // 
            this.columnHeader3.Name = "columnHeader3";
            this.columnHeader3.Text = "Rozmiar (w bajtach)";
            this.columnHeader3.Width.Absolute = 105;
            // 
            // nodeConnector1
            // 
            this.nodeConnector1.LineColor = System.Drawing.SystemColors.ControlText;
            // 
            // elementStyle1
            // 
            this.elementStyle1.Class = "";
            this.elementStyle1.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            this.elementStyle1.Name = "elementStyle1";
            this.elementStyle1.TextColor = System.Drawing.SystemColors.ControlText;
            // 
            // elementStyle2
            // 
            this.elementStyle2.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(221)))), ((int)(((byte)(230)))), ((int)(((byte)(247)))));
            this.elementStyle2.BackColor2 = System.Drawing.Color.FromArgb(((int)(((byte)(138)))), ((int)(((byte)(168)))), ((int)(((byte)(228)))));
            this.elementStyle2.BackColorGradientAngle = 90;
            this.elementStyle2.BorderBottom = DevComponents.DotNetBar.eStyleBorderType.Solid;
            this.elementStyle2.BorderBottomWidth = 1;
            this.elementStyle2.BorderColor = System.Drawing.Color.DarkGray;
            this.elementStyle2.BorderLeft = DevComponents.DotNetBar.eStyleBorderType.Solid;
            this.elementStyle2.BorderLeftWidth = 1;
            this.elementStyle2.BorderRight = DevComponents.DotNetBar.eStyleBorderType.Solid;
            this.elementStyle2.BorderRightWidth = 1;
            this.elementStyle2.BorderTop = DevComponents.DotNetBar.eStyleBorderType.Solid;
            this.elementStyle2.BorderTopWidth = 1;
            this.elementStyle2.Class = "";
            this.elementStyle2.CornerDiameter = 4;
            this.elementStyle2.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            this.elementStyle2.Description = "Blue";
            this.elementStyle2.Name = "elementStyle2";
            this.elementStyle2.PaddingBottom = 1;
            this.elementStyle2.PaddingLeft = 1;
            this.elementStyle2.PaddingRight = 1;
            this.elementStyle2.PaddingTop = 1;
            this.elementStyle2.TextColor = System.Drawing.Color.Black;
            // 
            // FileExplorer
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.explorerTree);
            this.Name = "FileExplorer";
            this.Size = new System.Drawing.Size(365, 482);
            this.Load += new System.EventHandler(this.FileExplorerTree_Load);
            ((System.ComponentModel.ISupportInitialize)(this.explorerTree)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private DevComponents.AdvTree.AdvTree explorerTree;
        private DevComponents.AdvTree.NodeConnector nodeConnector1;
        private DevComponents.DotNetBar.ElementStyle elementStyle1;
        private DevComponents.AdvTree.ColumnHeader columnHeader1;
        private DevComponents.AdvTree.ColumnHeader columnHeader2;
        private DevComponents.AdvTree.ColumnHeader columnHeader3;
        private DevComponents.DotNetBar.ElementStyle elementStyle2;
    }
}
