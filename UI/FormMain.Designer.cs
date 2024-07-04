﻿namespace UI
{
    partial class FormMain
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lblMessage = new System.Windows.Forms.ToolStripStatusLabel();
            this.progressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.timerProgressbar = new System.Windows.Forms.Timer(this.components);
            this.timerClear = new System.Windows.Forms.Timer(this.components);
            this.timerRefresh = new System.Windows.Forms.Timer(this.components);
            this.panelLogo = new System.Windows.Forms.Panel();
            this.btnImage = new System.Windows.Forms.Button();
            this.btnWebcam = new System.Windows.Forms.Button();
            this.btnSettings = new System.Windows.Forms.Button();
            this.panelMenu = new System.Windows.Forms.Panel();
            this.panelDesktop = new System.Windows.Forms.Panel();
            this.statusStrip1.SuspendLayout();
            this.panelMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblMessage,
            this.progressBar1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 570);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Padding = new System.Windows.Forms.Padding(1, 0, 19, 0);
            this.statusStrip1.Size = new System.Drawing.Size(1187, 22);
            this.statusStrip1.TabIndex = 18;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // lblMessage
            // 
            this.lblMessage.Font = new System.Drawing.Font("Segoe UI", 10F);
            this.lblMessage.Name = "lblMessage";
            this.lblMessage.Size = new System.Drawing.Size(0, 17);
            // 
            // progressBar1
            // 
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(100, 16);
            // 
            // timerProgressbar
            // 
            this.timerProgressbar.Interval = 10;
            this.timerProgressbar.Tick += new System.EventHandler(this.timerProgressbar_Tick);
            // 
            // timerClear
            // 
            this.timerClear.Interval = 2000;
            // 
            // timerRefresh
            // 
            this.timerRefresh.Enabled = true;
            this.timerRefresh.Interval = 300000;
            // 
            // panelLogo
            // 
            this.panelLogo.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(234)))), ((int)(((byte)(72)))), ((int)(((byte)(51)))));
            this.panelLogo.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelLogo.Location = new System.Drawing.Point(0, 0);
            this.panelLogo.Name = "panelLogo";
            this.panelLogo.Size = new System.Drawing.Size(200, 74);
            this.panelLogo.TabIndex = 0;
            // 
            // btnImage
            // 
            this.btnImage.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnImage.FlatAppearance.BorderSize = 0;
            this.btnImage.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnImage.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnImage.ForeColor = System.Drawing.Color.Gainsboro;
            this.btnImage.Image = global::UI.Properties.Resources.picture_32;
            this.btnImage.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnImage.Location = new System.Drawing.Point(0, 74);
            this.btnImage.Name = "btnImage";
            this.btnImage.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.btnImage.Size = new System.Drawing.Size(200, 60);
            this.btnImage.TabIndex = 1;
            this.btnImage.Text = "  Image";
            this.btnImage.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnImage.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnImage.UseVisualStyleBackColor = true;
            this.btnImage.Click += new System.EventHandler(this.btnImage_Click);
            // 
            // btnWebcam
            // 
            this.btnWebcam.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnWebcam.FlatAppearance.BorderSize = 0;
            this.btnWebcam.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnWebcam.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnWebcam.ForeColor = System.Drawing.Color.Gainsboro;
            this.btnWebcam.Image = global::UI.Properties.Resources.webcam_32;
            this.btnWebcam.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnWebcam.Location = new System.Drawing.Point(0, 134);
            this.btnWebcam.Name = "btnWebcam";
            this.btnWebcam.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.btnWebcam.Size = new System.Drawing.Size(200, 60);
            this.btnWebcam.TabIndex = 2;
            this.btnWebcam.Text = "   Webcam";
            this.btnWebcam.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnWebcam.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnWebcam.UseVisualStyleBackColor = true;
            this.btnWebcam.Visible = false;
            this.btnWebcam.Click += new System.EventHandler(this.btnWebcam_Click);
            // 
            // btnSettings
            // 
            this.btnSettings.Dock = System.Windows.Forms.DockStyle.Top;
            this.btnSettings.FlatAppearance.BorderSize = 0;
            this.btnSettings.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSettings.Font = new System.Drawing.Font("Microsoft Sans Serif", 11F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSettings.ForeColor = System.Drawing.Color.Gainsboro;
            this.btnSettings.Image = global::UI.Properties.Resources.cog;
            this.btnSettings.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnSettings.Location = new System.Drawing.Point(0, 194);
            this.btnSettings.Name = "btnSettings";
            this.btnSettings.Padding = new System.Windows.Forms.Padding(12, 0, 0, 0);
            this.btnSettings.Size = new System.Drawing.Size(200, 60);
            this.btnSettings.TabIndex = 6;
            this.btnSettings.Text = "   Option";
            this.btnSettings.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.btnSettings.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.btnSettings.UseVisualStyleBackColor = true;
            this.btnSettings.Click += new System.EventHandler(this.btnSettings_Click);
            // 
            // panelMenu
            // 
            this.panelMenu.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(51)))), ((int)(((byte)(51)))), ((int)(((byte)(76)))));
            this.panelMenu.Controls.Add(this.btnSettings);
            this.panelMenu.Controls.Add(this.btnWebcam);
            this.panelMenu.Controls.Add(this.btnImage);
            this.panelMenu.Controls.Add(this.panelLogo);
            this.panelMenu.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelMenu.Location = new System.Drawing.Point(0, 0);
            this.panelMenu.Name = "panelMenu";
            this.panelMenu.Size = new System.Drawing.Size(200, 570);
            this.panelMenu.TabIndex = 16;
            // 
            // panelDesktop
            // 
            this.panelDesktop.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelDesktop.Location = new System.Drawing.Point(200, 0);
            this.panelDesktop.Name = "panelDesktop";
            this.panelDesktop.Size = new System.Drawing.Size(987, 570);
            this.panelDesktop.TabIndex = 17;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1187, 592);
            this.Controls.Add(this.panelDesktop);
            this.Controls.Add(this.panelMenu);
            this.Controls.Add(this.statusStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "FormMain";
            this.Text = "Screen reader";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.panelMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel lblMessage;
        private System.Windows.Forms.Timer timerProgressbar;
        private System.Windows.Forms.Timer timerClear;
        private System.Windows.Forms.Timer timerRefresh;
        private System.Windows.Forms.Panel panelLogo;
        private System.Windows.Forms.Button btnImage;
        private System.Windows.Forms.Button btnWebcam;
        private System.Windows.Forms.Button btnSettings;
        private System.Windows.Forms.Panel panelMenu;
        private System.Windows.Forms.Panel panelDesktop;
        private System.Windows.Forms.ToolStripProgressBar progressBar1;
    }
}