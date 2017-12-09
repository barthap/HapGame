namespace Hapex.HapGame.Editor
{
    partial class NewModName
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
            DevComponents.DotNetBar.LabelX labelX1;
            this.mName = new DevComponents.DotNetBar.Controls.TextBoxX();
            this.cancelBtn = new DevComponents.DotNetBar.ButtonX();
            this.okBtn = new DevComponents.DotNetBar.ButtonX();
            labelX1 = new DevComponents.DotNetBar.LabelX();
            this.SuspendLayout();
            // 
            // labelX1
            // 
            // 
            // 
            // 
            labelX1.BackgroundStyle.Class = "";
            labelX1.BackgroundStyle.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            labelX1.Location = new System.Drawing.Point(3, 3);
            labelX1.Name = "labelX1";
            labelX1.Size = new System.Drawing.Size(170, 23);
            labelX1.TabIndex = 0;
            labelX1.Text = "Please specify mod name here";
            // 
            // mName
            // 
            // 
            // 
            // 
            this.mName.Border.Class = "TextBoxBorder";
            this.mName.Border.CornerType = DevComponents.DotNetBar.eCornerType.Square;
            this.mName.Location = new System.Drawing.Point(3, 32);
            this.mName.Name = "mName";
            this.mName.Size = new System.Drawing.Size(170, 20);
            this.mName.TabIndex = 1;
            // 
            // cancelBtn
            // 
            this.cancelBtn.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.cancelBtn.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
            this.cancelBtn.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.cancelBtn.Location = new System.Drawing.Point(207, 3);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(75, 23);
            this.cancelBtn.Style = DevComponents.DotNetBar.eDotNetBarStyle.StyleManagerControlled;
            this.cancelBtn.TabIndex = 2;
            this.cancelBtn.Text = "Cancel";
            this.cancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // okBtn
            // 
            this.okBtn.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.okBtn.ColorTable = DevComponents.DotNetBar.eButtonColor.OrangeWithBackground;
            this.okBtn.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.okBtn.Location = new System.Drawing.Point(207, 32);
            this.okBtn.Name = "okBtn";
            this.okBtn.Size = new System.Drawing.Size(75, 23);
            this.okBtn.Style = DevComponents.DotNetBar.eDotNetBarStyle.StyleManagerControlled;
            this.okBtn.TabIndex = 2;
            this.okBtn.Text = "OK";
            this.okBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // NewModName
            // 
            this.AcceptButton = this.okBtn;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.cancelBtn;
            this.ClientSize = new System.Drawing.Size(298, 64);
            this.Controls.Add(this.okBtn);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.mName);
            this.Controls.Add(labelX1);
            this.DoubleBuffered = true;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedToolWindow;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "NewModName";
            this.ShowIcon = false;
            this.Text = "NewModName";
            this.ResumeLayout(false);

        }

        #endregion

        private DevComponents.DotNetBar.ButtonX cancelBtn;
        private DevComponents.DotNetBar.ButtonX okBtn;
        public DevComponents.DotNetBar.Controls.TextBoxX mName;
    }
}