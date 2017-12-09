namespace Hapex.HapGame.Editor
{
    partial class frmDocument
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

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(frmDocument));
            System.Windows.Forms.TreeNode treeNode1 = new System.Windows.Forms.TreeNode("health");
            System.Windows.Forms.TreeNode treeNode2 = new System.Windows.Forms.TreeNode("mana");
            System.Windows.Forms.TreeNode treeNode3 = new System.Windows.Forms.TreeNode("gold");
            System.Windows.Forms.TreeNode treeNode4 = new System.Windows.Forms.TreeNode("wood");
            System.Windows.Forms.TreeNode treeNode5 = new System.Windows.Forms.TreeNode("stone");
            System.Windows.Forms.TreeNode treeNode6 = new System.Windows.Forms.TreeNode("iron");
            System.Windows.Forms.TreeNode treeNode7 = new System.Windows.Forms.TreeNode("level");
            System.Windows.Forms.TreeNode treeNode8 = new System.Windows.Forms.TreeNode("exp");
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("Player", new System.Windows.Forms.TreeNode[] {
            treeNode1,
            treeNode2,
            treeNode3,
            treeNode4,
            treeNode5,
            treeNode6,
            treeNode7,
            treeNode8});
            System.Windows.Forms.TreeNode treeNode10 = new System.Windows.Forms.TreeNode("player");
            System.Windows.Forms.TreeNode treeNode11 = new System.Windows.Forms.TreeNode("chr");
            System.Windows.Forms.TreeNode treeNode12 = new System.Windows.Forms.TreeNode("item");
            System.Windows.Forms.TreeNode treeNode13 = new System.Windows.Forms.TreeNode("ObjType", new System.Windows.Forms.TreeNode[] {
            treeNode10,
            treeNode11,
            treeNode12});
            System.Windows.Forms.TreeNode treeNode14 = new System.Windows.Forms.TreeNode("chrToBase");
            System.Windows.Forms.TreeNode treeNode15 = new System.Windows.Forms.TreeNode("getChr");
            System.Windows.Forms.TreeNode treeNode16 = new System.Windows.Forms.TreeNode("getTrigger");
            System.Windows.Forms.TreeNode treeNode17 = new System.Windows.Forms.TreeNode("createChr");
            System.Windows.Forms.TreeNode treeNode18 = new System.Windows.Forms.TreeNode("createItem");
            System.Windows.Forms.TreeNode treeNode19 = new System.Windows.Forms.TreeNode("ObjManager", new System.Windows.Forms.TreeNode[] {
            treeNode14,
            treeNode15,
            treeNode16,
            treeNode17,
            treeNode18});
            this.codeLineNumber = new Hapex.CodeEdit.CodeLineNumber();
            this.codeTextBox = new Hapex.CodeEdit.CodeTextBox();
            this.commandCut = new DevComponents.DotNetBar.Command(this.components);
            this.commandCopy = new DevComponents.DotNetBar.Command(this.components);
            this.commandPaste = new DevComponents.DotNetBar.Command(this.components);
            this.commandUndo = new DevComponents.DotNetBar.Command(this.components);
            this.commandRedo = new DevComponents.DotNetBar.Command(this.components);
            this.SuspendLayout();
            // 
            // codeLineNumber
            // 
            this.codeLineNumber._SeeThroughMode_ = false;
            this.codeLineNumber.AutoSizing = true;
            this.codeLineNumber.BackgroundGradient_AlphaColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.codeLineNumber.BackgroundGradient_BetaColor = System.Drawing.Color.LightSteelBlue;
            this.codeLineNumber.BackgroundGradient_Direction = System.Drawing.Drawing2D.LinearGradientMode.Horizontal;
            this.codeLineNumber.BorderLines_Color = System.Drawing.Color.SlateGray;
            this.codeLineNumber.BorderLines_Style = System.Drawing.Drawing2D.DashStyle.Solid;
            this.codeLineNumber.BorderLines_Thickness = 1F;
            this.codeLineNumber.Dock = System.Windows.Forms.DockStyle.Left;
            this.codeLineNumber.DockSide = Hapex.CodeEdit.CodeLineNumber.LineNumberDockSide.Left;
            this.codeLineNumber.GridLines_Color = System.Drawing.Color.SlateGray;
            this.codeLineNumber.GridLines_Style = System.Drawing.Drawing2D.DashStyle.Solid;
            this.codeLineNumber.GridLines_Thickness = 1F;
            this.codeLineNumber.LineNrs_Alignment = System.Drawing.ContentAlignment.TopRight;
            this.codeLineNumber.LineNrs_AntiAlias = true;
            this.codeLineNumber.LineNrs_AsHexadecimal = false;
            this.codeLineNumber.LineNrs_ClippedByItemRectangle = true;
            this.codeLineNumber.LineNrs_LeadingZeroes = true;
            this.codeLineNumber.LineNrs_Offset = new System.Drawing.Size(0, 0);
            this.codeLineNumber.Location = new System.Drawing.Point(0, 0);
            this.codeLineNumber.Margin = new System.Windows.Forms.Padding(0);
            this.codeLineNumber.MarginLines_Color = System.Drawing.Color.SlateGray;
            this.codeLineNumber.MarginLines_Side = Hapex.CodeEdit.CodeLineNumber.LineNumberDockSide.Right;
            this.codeLineNumber.MarginLines_Style = System.Drawing.Drawing2D.DashStyle.Solid;
            this.codeLineNumber.MarginLines_Thickness = 1F;
            this.codeLineNumber.Name = "codeLineNumber";
            this.codeLineNumber.Padding = new System.Windows.Forms.Padding(0, 0, 2, 0);
            this.codeLineNumber.ParentRichTextBox = this.codeTextBox;
            this.codeLineNumber.Show_BackgroundGradient = true;
            this.codeLineNumber.Show_BorderLines = true;
            this.codeLineNumber.Show_GridLines = true;
            this.codeLineNumber.Show_LineNrs = true;
            this.codeLineNumber.Show_MarginLines = true;
            this.codeLineNumber.Size = new System.Drawing.Size(17, 436);
            this.codeLineNumber.TabIndex = 0;
            // 
            // codeTextBox
            // 
            this.codeTextBox.AcceptsTab = true;
            this.codeTextBox.AutoWordSelection = true;
            this.codeTextBox.CodeColor_Comment = System.Drawing.Color.Green;
            this.codeTextBox.CodeColor_Function = System.Drawing.Color.CornflowerBlue;
            this.codeTextBox.CodeColor_Keyword = System.Drawing.Color.Blue;
            this.codeTextBox.CodeColor_PlainText = System.Drawing.Color.Black;
            this.codeTextBox.CodeColor_String = System.Drawing.Color.DarkRed;
            this.codeTextBox.CodeColor_Type = System.Drawing.Color.Chocolate;
            this.codeTextBox.CodeImage_Class = ((System.Drawing.Image)(resources.GetObject("codeTextBox.CodeImage_Class")));
            this.codeTextBox.CodeImage_Event = ((System.Drawing.Image)(resources.GetObject("codeTextBox.CodeImage_Event")));
            this.codeTextBox.CodeImage_Interface = ((System.Drawing.Image)(resources.GetObject("codeTextBox.CodeImage_Interface")));
            this.codeTextBox.CodeImage_Method = ((System.Drawing.Image)(resources.GetObject("codeTextBox.CodeImage_Method")));
            this.codeTextBox.CodeImage_Namespace = ((System.Drawing.Image)(resources.GetObject("codeTextBox.CodeImage_Namespace")));
            this.codeTextBox.CodeImage_Property = ((System.Drawing.Image)(resources.GetObject("codeTextBox.CodeImage_Property")));
            this.codeTextBox.CodeWords_Comments = ((System.Collections.Generic.List<string>)(resources.GetObject("codeTextBox.CodeWords_Comments")));
            this.codeTextBox.CodeWords_Functions = ((System.Collections.Generic.List<string>)(resources.GetObject("codeTextBox.CodeWords_Functions")));
            this.codeTextBox.CodeWords_Keywords = ((System.Collections.Generic.List<string>)(resources.GetObject("codeTextBox.CodeWords_Keywords")));
            this.codeTextBox.CodeWords_ScopeOperators = ((System.Collections.Generic.List<string>)(resources.GetObject("codeTextBox.CodeWords_ScopeOperators")));
            this.codeTextBox.CodeWords_Types = ((System.Collections.Generic.List<string>)(resources.GetObject("codeTextBox.CodeWords_Types")));
            this.codeTextBox.DetectUrls = false;
            this.codeTextBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.codeTextBox.Font = new System.Drawing.Font("Courier New", 9F);
            this.codeTextBox.IntellisenseKey = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Alt)
                        | System.Windows.Forms.Keys.Space)));
            // 
            // 
            // 
            this.codeTextBox.IntellisenseTree.LineColor = System.Drawing.Color.Empty;
            this.codeTextBox.IntellisenseTree.Location = new System.Drawing.Point(0, 0);
            this.codeTextBox.IntellisenseTree.Name = "";
            treeNode1.Name = "health";
            treeNode1.Tag = "property";
            treeNode1.Text = "health";
            treeNode2.Name = "mana";
            treeNode2.Tag = "property";
            treeNode2.Text = "mana";
            treeNode3.Name = "gold";
            treeNode3.Tag = "property";
            treeNode3.Text = "gold";
            treeNode4.Name = "wood";
            treeNode4.Tag = "property";
            treeNode4.Text = "wood";
            treeNode5.Name = "stone";
            treeNode5.Tag = "property";
            treeNode5.Text = "stone";
            treeNode6.Name = "iron";
            treeNode6.Tag = "property";
            treeNode6.Text = "iron";
            treeNode7.Name = "level";
            treeNode7.Tag = "property";
            treeNode7.Text = "level";
            treeNode8.Name = "exp";
            treeNode8.Tag = "property";
            treeNode8.Text = "exp";
            treeNode9.Name = "Player";
            treeNode9.Tag = "class";
            treeNode9.Text = "Player";
            treeNode10.Name = "player";
            treeNode10.Tag = "event";
            treeNode10.Text = "player";
            treeNode11.Name = "chr";
            treeNode11.Tag = "event";
            treeNode11.Text = "chr";
            treeNode12.Name = "item";
            treeNode12.Tag = "event";
            treeNode12.Text = "item";
            treeNode13.Name = "ObjType";
            treeNode13.Tag = "namespace";
            treeNode13.Text = "ObjType";
            treeNode14.Name = "chrToBase";
            treeNode14.Tag = "method";
            treeNode14.Text = "chrToBase";
            treeNode15.Name = "getChr";
            treeNode15.Tag = "method";
            treeNode15.Text = "getChr";
            treeNode16.Name = "getTrigger";
            treeNode16.Tag = "method";
            treeNode16.Text = "getTrigger";
            treeNode17.Name = "createChr";
            treeNode17.Tag = "method";
            treeNode17.Text = "createChr";
            treeNode18.Name = "createItem";
            treeNode18.Tag = "method";
            treeNode18.Text = "createItem";
            treeNode19.Name = "ObjManager";
            treeNode19.Tag = "class";
            treeNode19.Text = "ObjManager";
            this.codeTextBox.IntellisenseTree.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode9,
            treeNode13,
            treeNode19});
            this.codeTextBox.IntellisenseTree.TabIndex = 0;
            this.codeTextBox.Location = new System.Drawing.Point(17, 0);
            this.codeTextBox.Name = "codeTextBox";
            this.codeTextBox.Size = new System.Drawing.Size(466, 436);
            this.codeTextBox.TabIndex = 1;
            this.codeTextBox.Text = "";
            this.codeTextBox.WordWrap = false;
            this.codeTextBox.TextChanged += new System.EventHandler(this.OnTextChanged);
            // 
            // commandCut
            // 
            this.commandCut.Name = "commandCut";
            this.commandCut.Executed += new System.EventHandler(this.commandCut_Executed);
            // 
            // commandCopy
            // 
            this.commandCopy.Name = "commandCopy";
            this.commandCopy.Executed += new System.EventHandler(this.commandCopy_Executed);
            // 
            // commandPaste
            // 
            this.commandPaste.Name = "commandPaste";
            this.commandPaste.Executed += new System.EventHandler(this.commandPaste_Executed);
            // 
            // commandUndo
            // 
            this.commandUndo.Name = "commandUndo";
            this.commandUndo.Executed += new System.EventHandler(this.commandUndo_Executed);
            // 
            // commandRedo
            // 
            this.commandRedo.Name = "commandRedo";
            this.commandRedo.Executed += new System.EventHandler(this.commandRedo_Executed);
            // 
            // frmDocument
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(483, 436);
            this.Controls.Add(this.codeTextBox);
            this.Controls.Add(this.codeLineNumber);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "frmDocument";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "frmDocument";
            this.Load += new System.EventHandler(this.frmDocument_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private Hapex.CodeEdit.CodeLineNumber codeLineNumber;
        private Hapex.CodeEdit.CodeTextBox codeTextBox;
        public DevComponents.DotNetBar.Command commandCut;
        public DevComponents.DotNetBar.Command commandCopy;
        public DevComponents.DotNetBar.Command commandPaste;
        public DevComponents.DotNetBar.Command commandUndo;
        public DevComponents.DotNetBar.Command commandRedo;
    }
}