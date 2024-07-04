namespace UI
{
    partial class FormWebcam
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
            this.timerProgressbar = new System.Windows.Forms.Timer(this.components);
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.timerClear = new System.Windows.Forms.Timer(this.components);
            this.panelLogo = new System.Windows.Forms.Panel();
            this.btnRead = new System.Windows.Forms.Button();
            this.cbCamera = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.picResult = new System.Windows.Forms.PictureBox();
            this.rd_auto = new System.Windows.Forms.RadioButton();
            this.rd_manual = new System.Windows.Forms.RadioButton();
            this.chk_drawText = new System.Windows.Forms.CheckBox();
            this.chk_drawRect = new System.Windows.Forms.CheckBox();
            this.panelLogo.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picResult)).BeginInit();
            this.SuspendLayout();
            // 
            // timerProgressbar
            // 
            this.timerProgressbar.Interval = 10;
            // 
            // imageList1
            // 
            this.imageList1.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.imageList1.ImageSize = new System.Drawing.Size(100, 100);
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // timerClear
            // 
            this.timerClear.Interval = 2000;
            // 
            // panelLogo
            // 
            this.panelLogo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(67)))), ((int)(((byte)(183)))), ((int)(((byte)(110)))));
            this.panelLogo.Controls.Add(this.chk_drawRect);
            this.panelLogo.Controls.Add(this.chk_drawText);
            this.panelLogo.Controls.Add(this.rd_manual);
            this.panelLogo.Controls.Add(this.rd_auto);
            this.panelLogo.Controls.Add(this.btnRead);
            this.panelLogo.Controls.Add(this.cbCamera);
            this.panelLogo.Controls.Add(this.label1);
            this.panelLogo.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelLogo.Location = new System.Drawing.Point(0, 0);
            this.panelLogo.Name = "panelLogo";
            this.panelLogo.Size = new System.Drawing.Size(982, 114);
            this.panelLogo.TabIndex = 21;
            // 
            // btnRead
            // 
            this.btnRead.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(148)))), ((int)(((byte)(188)))));
            this.btnRead.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Bold);
            this.btnRead.ForeColor = System.Drawing.Color.White;
            this.btnRead.Location = new System.Drawing.Point(384, 13);
            this.btnRead.Name = "btnRead";
            this.btnRead.Size = new System.Drawing.Size(129, 35);
            this.btnRead.TabIndex = 4;
            this.btnRead.Text = "Chụp ảnh";
            this.btnRead.UseVisualStyleBackColor = false;
            this.btnRead.Click += new System.EventHandler(this.btnRead_Click);
            // 
            // cbCamera
            // 
            this.cbCamera.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cbCamera.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.cbCamera.FormattingEnabled = true;
            this.cbCamera.Location = new System.Drawing.Point(102, 19);
            this.cbCamera.Name = "cbCamera";
            this.cbCamera.Size = new System.Drawing.Size(264, 25);
            this.cbCamera.TabIndex = 3;
            this.cbCamera.SelectedIndexChanged += new System.EventHandler(this.cbCamera_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.White;
            this.label1.Location = new System.Drawing.Point(16, 20);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(70, 21);
            this.label1.TabIndex = 0;
            this.label1.Text = "Webcam";
            // 
            // listBox1
            // 
            this.listBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBox1.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);
            this.listBox1.FormattingEnabled = true;
            this.listBox1.ItemHeight = 20;
            this.listBox1.Location = new System.Drawing.Point(513, 114);
            this.listBox1.Margin = new System.Windows.Forms.Padding(0);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(469, 364);
            this.listBox1.TabIndex = 35;
            // 
            // picResult
            // 
            this.picResult.BackColor = System.Drawing.Color.White;
            this.picResult.Dock = System.Windows.Forms.DockStyle.Left;
            this.picResult.Location = new System.Drawing.Point(0, 114);
            this.picResult.Name = "picResult";
            this.picResult.Size = new System.Drawing.Size(513, 364);
            this.picResult.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.picResult.TabIndex = 34;
            this.picResult.TabStop = false;
            // 
            // rd_auto
            // 
            this.rd_auto.AutoSize = true;
            this.rd_auto.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rd_auto.ForeColor = System.Drawing.Color.White;
            this.rd_auto.Location = new System.Drawing.Point(49, 66);
            this.rd_auto.Name = "rd_auto";
            this.rd_auto.Size = new System.Drawing.Size(61, 25);
            this.rd_auto.TabIndex = 5;
            this.rd_auto.Text = "Auto";
            this.rd_auto.UseVisualStyleBackColor = true;
            this.rd_auto.CheckedChanged += new System.EventHandler(this.rd_auto_CheckedChanged);
            // 
            // rd_manual
            // 
            this.rd_manual.AutoSize = true;
            this.rd_manual.Checked = true;
            this.rd_manual.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.rd_manual.ForeColor = System.Drawing.Color.White;
            this.rd_manual.Location = new System.Drawing.Point(148, 66);
            this.rd_manual.Name = "rd_manual";
            this.rd_manual.Size = new System.Drawing.Size(80, 25);
            this.rd_manual.TabIndex = 6;
            this.rd_manual.TabStop = true;
            this.rd_manual.Text = "Manual";
            this.rd_manual.UseVisualStyleBackColor = true;
            // 
            // chk_drawText
            // 
            this.chk_drawText.AutoSize = true;
            this.chk_drawText.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chk_drawText.ForeColor = System.Drawing.Color.White;
            this.chk_drawText.Location = new System.Drawing.Point(271, 66);
            this.chk_drawText.Name = "chk_drawText";
            this.chk_drawText.Size = new System.Drawing.Size(95, 25);
            this.chk_drawText.TabIndex = 7;
            this.chk_drawText.Text = "Draw text";
            this.chk_drawText.UseVisualStyleBackColor = true;
            // 
            // chk_drawRect
            // 
            this.chk_drawRect.AutoSize = true;
            this.chk_drawRect.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chk_drawRect.ForeColor = System.Drawing.Color.White;
            this.chk_drawRect.Location = new System.Drawing.Point(384, 66);
            this.chk_drawRect.Name = "chk_drawRect";
            this.chk_drawRect.Size = new System.Drawing.Size(96, 25);
            this.chk_drawRect.TabIndex = 8;
            this.chk_drawRect.Text = "Draw rect";
            this.chk_drawRect.UseVisualStyleBackColor = true;
            // 
            // FormWebcam
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(982, 478);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.picResult);
            this.Controls.Add(this.panelLogo);
            this.Name = "FormWebcam";
            this.Text = "Phần mềm đọc Căn cước Công dân";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormWebcam_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.SizeChanged += new System.EventHandler(this.FormWebcam_SizeChanged);
            this.VisibleChanged += new System.EventHandler(this.FormWebcam_VisibleChanged);
            this.panelLogo.ResumeLayout(false);
            this.panelLogo.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picResult)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Timer timerProgressbar;
        private System.Windows.Forms.ImageList imageList1;
        private System.Windows.Forms.Timer timerClear;
        private System.Windows.Forms.Panel panelLogo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cbCamera;
        private System.Windows.Forms.Button btnRead;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.PictureBox picResult;
        private System.Windows.Forms.RadioButton rd_auto;
        private System.Windows.Forms.RadioButton rd_manual;
        private System.Windows.Forms.CheckBox chk_drawText;
        private System.Windows.Forms.CheckBox chk_drawRect;
    }
}

