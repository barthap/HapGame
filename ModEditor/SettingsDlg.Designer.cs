namespace Hapex.HapGame.Editor
{
    partial class SettingsDlg
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
            DevComponents.DotNetBar.TabControlPanel tabControlPanel1;
            DevComponents.DotNetBar.Controls.GroupPanel groupPanel1;
            DevComponents.DotNetBar.LabelX labelX1;
            this.okBtn = new DevComponents.DotNetBar.ButtonX();
            this.cancelBtn = new DevComponents.DotNetBar.ButtonX();
            this.gamePathBrowseBtn = new DevComponents.DotNetBar.ButtonX();
            this.gamePath = new DevComponents.DotNetBar.Controls.TextBoxX();
            this.tabItem1 = new DevComponents.DotNetBar.TabItem(this.components);
            this.tabControl1 = new DevComponents.DotNetBar.TabControl();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            tabControlPanel1 = new DevComponents.DotNetBar.TabControlPanel();
            groupPanel1 = new DevComponents.DotNetBar.Controls.GroupPanel();
            labelX1 = new DevComponents.DotNetBar.LabelX();
            tabControlPanel1.SuspendLayout();
            groupPanel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.tabControl1)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControlPanel1
            // 
            tabControlPanel1.Controls.Add(this.okBtn);
            tabControlPanel1.Controls.Add(this.cancelBtn);
            tabControlPanel1.Controls.Add(groupPanel1);
            tabControlPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            tabControlPanel1.Location = new System.Drawing.Point(0, 22);
            tabControlPanel1.Name = "tabControlPanel1";
            tabControlPanel1.Padding = new System.Windows.Forms.Padding(1);
            tabControlPanel1.Size = new System.Drawing.Size(353, 253);
            tabControlPanel1.Style.BackColor1.Color = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            tabControlPanel1.Style.Border = DevComponents.DotNetBar.eBorderType.SingleLine;
            tabControlPanel1.Style.BorderColor.Color = System.Drawing.Color.FromArgb(((int)(((byte)(49)))), ((int)(((byte)(49)))), ((int)(((byte)(49)))));
            tabControlPanel1.Style.BorderSide = ((DevComponents.DotNetBar.eBorderSide)(((DevComponents.DotNetBar.eBorderSide.Left | DevComponents.DotNetBar.eBorderSide.Right)
                        | DevComponents.DotNetBar.eBorderSide.Bottom)));
            tabControlPanel1.Style.GradientAngle = 90;
            tabControlPanel1.TabIndex = 1;
            tabControlPanel1.TabItem = this.tabItem1;
            // 
            // okBtn
            // 
            this.okBtn.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.okBtn.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
            this.okBtn.Location = new System.Drawing.Point(174, 214);
            this.okBtn.Name = "okBtn";
            this.okBtn.Size = new System.Drawing.Size(75, 23);
            this.okBtn.Style = DevComponents.DotNetBar.eDotNetBarStyle.StyleManagerControlled;
            this.okBtn.TabIndex = 0;
            this.okBtn.Text = "OK";
            this.okBtn.Click += new System.EventHandler(this.okBtn_Click);
            // 
            // cancelBtn
            // 
            this.cancelBtn.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.cancelBtn.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
            this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelBtn.Location = new System.Drawing.Point(266, 214);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(75, 23);
            this.cancelBtn.Style = DevComponents.DotNetBar.eDotNetBarStyle.StyleManagerControlled;
            this.cancelBtn.TabIndex = 0;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // groupPanel1
            // 
            groupPanel1.BackColor = System.Drawing.Color.Transparent;
            groupPanel1.CanvasColor = System.Drawing.SystemColors.Control;
            groupPanel1.ColorSchemeStyle = DevComponents.DotNetBar.eDotNetBarStyle.Office2007;
            groupPanel1.Controls.Add(labelX1);
            groupPanel1.Controls.Add(this.gamePathBrowseBtn);
            groupPanel1.Controls.Add(this.gamePath);
            groupPanel1.Location = new System.Drawing.Point(12, 4);
            groupPanel1.Name = "groupPanel1";
            groupPanel1.Size = new System.Drawing.Size(329, 91);
            // 
            // 
            // 
            groupPanel1.Style.BackColor2SchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBackground2;
            groupPanel1.Style.BackColorGradientAngle = 90;
            groupPanel1.Style.BackColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBackground;
            groupPanel1.Style.BorderBottom = DevComponents.DotNetBar.eStyleBorderType.Solid;
            groupPanel1.Style.BorderBottomWidth = 1;
            groupPanel1.Style.BorderColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelBorder;
            groupPanel1.Style.BorderLeft = DevComponents.DotNetBar.eStyleBorderType.Solid;
            groupPanel1.Style.BorderLeftWidth = 1;
            groupPanel1.Style.BorderRight = DevComponents.DotNetBar.eStyleBorderType.Solid;
            groupPanel1.Style.BorderRightWidth = 1;
            groupPanel1.Style.BorderTop = DevComponents.DotNetBar.eStyleBorderType.Solid;
            groupPanel1.Style.BorderTopWidth = 1;
            groupPanel1.Style.Class = "";
            groupPanel1.Style.CornerDiameter = 4;
            groupPanel1.Style.CornerType = DevComponents.DotNetBar.eCornerType.Rounded;
            groupPanel1.Style.TextAlignment = DevComponents.DotNetBar.eStyleTextAlignment.Center;
            groupPanel1.Style.TextColorSchemePart = DevComponents.DotNetBar.eColorSchemePart.PanelText;
            groupPanel1.Style.TextLineAlignment = DevComponents.DotNetBar.eStyleTextAlignment.Near;
            // 
            // 
            // 
            groupPanel1.StyleMouseDown.Class = "";
            groupPanel1.StyleMouseDown.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            // 
            // 
            // 
            groupPanel1.StyleMouseOver.Class = "";
            groupPanel1.StyleMouseOver.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            groupPanel1.TabIndex = 0;
            groupPanel1.Text = "Game Path";
            // 
            // labelX1
            // 
            labelX1.BackColor = System.Drawing.Color.Transparent;
            // 
            // 
            // 
            labelX1.BackgroundStyle.Class = "";
            labelX1.BackgroundStyle.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            labelX1.Location = new System.Drawing.Point(15, 7);
            labelX1.Name = "labelX1";
            labelX1.SingleLineColor = System.Drawing.Color.Transparent;
            labelX1.Size = new System.Drawing.Size(296, 23);
            labelX1.TabIndex = 2;
            labelX1.Text = "Please specify path to HapGame folder";
            // 
            // gamePathBrowseBtn
            // 
            this.gamePathBrowseBtn.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.gamePathBrowseBtn.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
            this.gamePathBrowseBtn.Location = new System.Drawing.Point(236, 36);
            this.gamePathBrowseBtn.Name = "gamePathBrowseBtn";
            this.gamePathBrowseBtn.Size = new System.Drawing.Size(75, 23);
            this.gamePathBrowseBtn.Style = DevComponents.DotNetBar.eDotNetBarStyle.StyleManagerControlled;
            this.gamePathBrowseBtn.TabIndex = 1;
            this.gamePathBrowseBtn.Text = "Browse...";
            this.gamePathBrowseBtn.Click += new System.EventHandler(this.gamePathBrowseBtn_Click);
            // 
            // gamePath
            // 
            // 
            // 
            // 
            this.gamePath.Border.Class = "TextBoxBorder";
            this.gamePath.Border.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            this.gamePath.Location = new System.Drawing.Point(15, 36);
            this.gamePath.Name = "gamePath";
            this.gamePath.Size = new System.Drawing.Size(215, 20);
            this.gamePath.TabIndex = 0;
            this.gamePath.TextChanged += new System.EventHandler(this.gamePath_TextChanged);
            // 
            // tabItem1
            // 
            this.tabItem1.AttachedControl = tabControlPanel1;
            this.tabItem1.Name = "tabItem1";
            this.tabItem1.Text = "General";
            // 
            // tabControl1
            // 
            this.tabControl1.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(194)))), ((int)(((byte)(217)))), ((int)(((byte)(247)))));
            this.tabControl1.CanReorderTabs = true;
            this.tabControl1.Controls.Add(tabControlPanel1);
            this.tabControl1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedTabFont = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold);
            this.tabControl1.SelectedTabIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(353, 275);
            this.tabControl1.Style = DevComponents.DotNetBar.eTabStripStyle.Office2007Document;
            this.tabControl1.TabIndex = 0;
            this.tabControl1.TabLayoutType = DevComponents.DotNetBar.eTabLayoutType.FixedWithNavigationBox;
            this.tabControl1.Tabs.Add(this.tabItem1);
            this.tabControl1.Text = "tabControl1";
            // 
            // SettingsDlg
            // 
            this.AcceptButton = this.okBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelBtn;
            this.ClientSize = new System.Drawing.Size(353, 275);
            this.Controls.Add(this.tabControl1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "SettingsDlg";
            this.ShowIcon = false;
            this.Text = "ModEditor Settings";
            tabControlPanel1.ResumeLayout(false);
            groupPanel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.tabControl1)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private DevComponents.DotNetBar.TabControl tabControl1;
        private DevComponents.DotNetBar.TabItem tabItem1;
        private DevComponents.DotNetBar.ButtonX okBtn;
        private DevComponents.DotNetBar.ButtonX cancelBtn;
        private DevComponents.DotNetBar.ButtonX gamePathBrowseBtn;
        private DevComponents.DotNetBar.Controls.TextBoxX gamePath;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
    }
}