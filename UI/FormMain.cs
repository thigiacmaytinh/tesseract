﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using TGMT;
using TGMTcs;

namespace UI
{
    public partial class FormMain : Form
    {
        


        static FormMain m_instance;
        Button currentButton;
        Form activeForm;
        bool m_keyboardPressed = false;
        
        bool m_activated = false;

        bool m_isCtrlPressed = false;
        bool m_isShiftPressed = false;
        

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public FormMain()
        {
            InitializeComponent();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static FormMain GetInstance()
        {
            if (m_instance == null)
                m_instance = new FormMain();
            return m_instance;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormMain_Load(object sender, EventArgs e)
        {
            TGMTregistry.GetInstance().Init("ScreenReader");

            Program.lineHeight = TGMTregistry.GetInstance().ReadInt("lineHeight", Program.lineHeight);
            Program.maxWidth = TGMTregistry.GetInstance().ReadInt("maxWidth", Program.maxWidth);
            Program.copyToClipboard = TGMTregistry.GetInstance().ReadBool("copyToClipboard", Program.copyToClipboard);
            Program.playSound = TGMTregistry.GetInstance().ReadBool("playSound", Program.copyToClipboard);


            BackgroundWorker worker = new BackgroundWorker();
            worker.WorkerReportsProgress = true;
            worker.DoWork += Worker_DoWork;
            

            worker.RunWorkerCompleted += worker_RunWorkerCompleted;
            worker.RunWorkerAsync();

            StartProgressbar();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        protected override void OnActivated(EventArgs e)
        {
            m_activated = true;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        protected override void OnDeactivate(EventArgs e)
        {
            m_activated = false;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void Worker_DoWork(object sender, DoWorkEventArgs e)
        {
            Program.reader = new TextReader();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void worker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            btnImage.PerformClick();
            StopProgressbar();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void btnImage_Click(object sender, EventArgs e)
        {
            OpenChildForm(FormImage.GetInstance(), sender);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void btnSettings_Click(object sender, EventArgs e)
        {
            OpenChildForm(FormOption.GetInstance(), sender);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public void PrintError(string message)
        {
            lblMessage.ForeColor = Color.Red;
            lblMessage.Text = DateTime.Now.ToString("(hh:mm:ss)") + message;

            timerClear.Stop();
            timerClear.Start();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public void PrintSuccess(string message)
        {
            lblMessage.ForeColor = Color.Green;
            lblMessage.Text = DateTime.Now.ToString("(hh:mm:ss)") + message;
            timerClear.Stop();
            timerClear.Start();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public void PrintMessage(string message)
        {
            lblMessage.ForeColor = Color.Black;
            lblMessage.Text = DateTime.Now.ToString("(hh:mm:ss)") + message;
            timerClear.Stop();
            timerClear.Start();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void OpenChildForm(Form childForm, object btnSender)
        {
            if (activeForm != null)
            {
                activeForm.Hide();
            }

            ActiveButton(btnSender);

            activeForm = childForm;
            childForm.TopLevel = false;
            childForm.FormBorderStyle = FormBorderStyle.None;
            childForm.Dock = DockStyle.Fill;
            this.panelDesktop.Controls.Add(childForm);
            this.panelDesktop.Tag = childForm;
            childForm.BringToFront();
            childForm.Show();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void ActiveButton(object btnSender)
        {
            if (btnSender != null)
            {
                if (currentButton != (Button)btnSender)
                {
                    DisableButton();
                    currentButton = (Button)btnSender;

                    Color color = SelectThemeColor(currentButton);
                    currentButton.BackColor = color;
                    currentButton.ForeColor = Color.White;
                    currentButton.Font = new Font("Microsoft Sans Serif", 12.5F, FontStyle.Regular, GraphicsUnit.Point, ((byte)(0)));

                    //panelLogo.BackColor = ThemeColor.ChangeColorBrightness(color, -0.3);

                    ThemeColor.PrimaryColor = color;
                    ThemeColor.SecondaryColor = ThemeColor.ChangeColorBrightness(color, -0.3);
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void DisableButton()
        {
            foreach (Control previousBtn in panelMenu.Controls)
            {
                if (previousBtn.GetType() == typeof(Button))
                {
                    previousBtn.BackColor = Color.FromArgb(51, 51, 76);
                    previousBtn.ForeColor = Color.Gainsboro;
                    previousBtn.Font = new Font("Microsoft Sans Serif", 11F, FontStyle.Regular, GraphicsUnit.Point, ((byte)(0)));
                }
            }
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        Color SelectThemeColor(Button btn)
        {
            int index = FindIndexOfBtn(btn);
            string color = ThemeColor.ColorList[index];
            return ColorTranslator.FromHtml(color);
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        int FindIndexOfBtn(Button btn)
        {
            int index = -1;
            foreach (Control ctrl in panelMenu.Controls)
            {
                if (ctrl.GetType() == typeof(Button))
                {
                    index++;
                    if ((Button)ctrl == btn)
                    {
                        return index;
                    }
                }
            }

            return index;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public void StartProgressbar()
        {
            timerProgressbar.Start();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public void StopProgressbar()
        {
            timerProgressbar.Stop();
            progressBar1.Value = progressBar1.Minimum;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void timerProgressbar_Tick(object sender, EventArgs e)
        {
            if (progressBar1.Value >= progressBar1.Maximum)
                progressBar1.Value = progressBar1.Minimum;
            progressBar1.Value += 1;
        }
    }
}