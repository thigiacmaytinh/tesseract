using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Diagnostics;
using System.Text;
using System.Threading;
using System.Windows.Forms;
using TGMT;
using TGMTcs;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;

namespace UI
{

    public partial class FormImage : Form
    {
        static FormImage m_instance;
        Stopwatch watch;
        bool m_isBusy = false;


        [DllImport("user32.dll")]
        private static extern bool GetCursorPos(out Point lpPoint);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public FormImage()
        {
            InitializeComponent();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static FormImage GetInstance()
        {
            if (m_instance == null)
                m_instance = new FormImage();
            return m_instance;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormImage_Load(object sender, EventArgs e)
        {
            
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void btn_select_Click(object sender, EventArgs e)
        {
            txt_fileName.Text = "";
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Image file |*.jpg;*.png*.bmp;*.PNG;";
            ofd.ShowDialog();
            if (ofd.FileName != "")
            {
                txt_fileName.Text = ofd.FileName;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void txt_fileName_TextChanged(object sender, EventArgs e)
        {
            if (txt_fileName.Text == "")
                return;

            btn_select.Enabled = false;


            Bitmap bmp = TGMTimage.LoadBitmapWithoutLock(txt_fileName.Text);
            if(bmp != null)
            {
                picResult.Image = bmp;
                FormMain.GetInstance().PrintMessage("");


                watch = Stopwatch.StartNew();
                Thread t = new Thread(() => Read(txt_fileName.Text));
                t.Start();
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void Read(string imagePath)
        {
            string result = Program.reader.Read(imagePath);
            this.Invoke(new Action(() =>
            {
                watch.Stop();


                textBox1.Text = result;

                if (Program.playSound)
                {
                    TGMTsound.PlaySound(Program.wavFile);
                }

                FormMain.GetInstance().PrintMessage("Elapsed: " + watch.ElapsedMilliseconds.ToString() + "ms");
                btn_select.Enabled = true;                
                
            }));
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormImage_SizeChanged(object sender, EventArgs e)
        {
            picResult.Width = this.Width * 2 / 3;
        }


    }
}
