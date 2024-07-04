using System;
using System.Collections.Generic;
using System.Threading;
using System.Data;
using System.Drawing;
using System.Linq;
using System.IO;
using System.Drawing.Imaging;
using System.Windows.Forms;
using TGMT;
using TGMTcs;
using AForge.Video;
using AForge.Video.DirectShow;
using System.Diagnostics;

namespace UI
{
    public partial class FormWebcam : Form
    {
        static FormWebcam m_instance;
        VideoCaptureDevice m_videoSource;
        Bitmap g_bmp;
        double g_scaleX = 1;
        double g_scaleY = 1;

        private static Random random = new Random();
        Stopwatch watch;
        bool m_formClosed = false;
        bool m_isBusy = false;

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public FormWebcam()
        {
            InitializeComponent();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static FormWebcam GetInstance()
        {
            if (m_instance == null)
                m_instance = new FormWebcam();
            return m_instance;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void Form1_Load(object sender, EventArgs e)
        {
            Directory.CreateDirectory("input");
            //InitCamera();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void InitCamera()
        {
            cbCamera.Items.Clear();

            FilterInfoCollection videosources = new FilterInfoCollection(FilterCategory.VideoInputDevice);

            if (videosources.Count == 0)
            {
                FormMain.GetInstance().PrintError("Can not find camera");
                return;
            }


            for (int i = 0; i < videosources.Count; i++)
            {
                cbCamera.Items.Add(videosources[i].Name);
            }
            cbCamera.Enabled = true;
            if(cbCamera.Items.Count == 1)
            {
                cbCamera.SelectedIndex = 0;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void cbCamera_SelectedIndexChanged(object sender, EventArgs e)
        {
            //ConnectLocalCamera();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void ConnectLocalCamera()
        {
            if (cbCamera.Items.Count == 0 || cbCamera.SelectedIndex == -1)
                return;
            if (m_videoSource != null)
            {
                m_videoSource.Stop();
            }
            else
            {
                FilterInfoCollection videosources = new FilterInfoCollection(FilterCategory.VideoInputDevice);
                m_videoSource = new VideoCaptureDevice(videosources[cbCamera.SelectedIndex].MonikerString);
            }

            m_videoSource.NewFrame += new NewFrameEventHandler(OnCameraFrame);
            m_videoSource.Start();
            btnRead.Enabled = true;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void OnCameraFrame(object sender, NewFrameEventArgs eventArgs)
        {
            if (m_formClosed)
                return;

            if (g_bmp != null)
                g_bmp.Dispose();

            g_scaleX = (float)eventArgs.Frame.Width / picResult.Width;
            g_scaleY = (float)eventArgs.Frame.Height / picResult.Height;

            g_bmp = (Bitmap)eventArgs.Frame.Clone();



            if (rd_auto.Checked)
            {                
                if (!m_isBusy)
                {
                    watch = Stopwatch.StartNew();
                    m_isBusy = true;
                    Thread t = new Thread(() => Read((Bitmap)g_bmp.Clone()));
                    t.Start();
                }
            }
            else
            {
                picResult.Image = g_bmp;
            }
            
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static string RandomString(int length)
        {
            const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
            return new string(Enumerable.Repeat(chars, length)
                .Select(s => s[random.Next(s.Length)]).ToArray());
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void btnRead_Click(object sender, EventArgs e)
        {
            if (g_bmp == null)
            {
                return;
            }

            Bitmap bmp = (Bitmap)g_bmp.Clone();
            string filePath = "input\\" + DateTime.Now.ToString("yyyy-MM-dd-hh-mm-ss") + "_" + RandomString(10) + ".jpg";
            bmp.Save(filePath, ImageFormat.Jpeg);

            btnRead.Enabled = false;
            FormMain.GetInstance().PrintMessage("");
            FormMain.GetInstance().StartProgressbar();

            watch = Stopwatch.StartNew();

            if (!m_isBusy)
            {
                m_isBusy = true;
                Thread t = new Thread(() => Read(bmp));
                t.Start();
            }            
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void Read(Bitmap bmp)
        {
            if (m_formClosed)
                return;
            //OcrPredicted result = Program.reader.Read(bmp, chk_drawRect.Checked, chk_drawText.Checked );
            
            //this.Invoke(new Action(() =>
            //{
            //    if (m_formClosed)
            //        return;
                
                    
                
            //    listBox1.Items.Clear();

            //    for (int i = 0; i < result.texts.Length; i++)
            //    {
            //        listBox1.Items.Add(result.texts[i]);
            //    }

            //    if (result.bitmap == null)
            //    {
            //        FormMain.GetInstance().PrintError(result.error);
            //    }
            //    else
            //    {
            //        picResult.Image = result.bitmap;
            //        FormMain.GetInstance().PrintMessage("Elapsed: " + watch.ElapsedMilliseconds.ToString() + "ms");
            //    }


            //    btnRead.Enabled = true;
            //    watch.Stop();
            //}));

            m_isBusy = false;

        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormWebcam_VisibleChanged(object sender, EventArgs e)
        {
            if (this.Visible)
            {
                if(cbCamera.Items.Count == 1)
                {
                    cbCamera.SelectedIndex = 0;
                }
            }
            else
            {
                StopAllCamera();
                cbCamera.SelectedIndex = -1;
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public void StopAllCamera()
        {

            if (m_videoSource != null)
                m_videoSource.Stop();

            picResult.Image = null;
            btnRead.Enabled = false;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormWebcam_FormClosed(object sender, FormClosedEventArgs e)
        {
            m_formClosed = true;
            StopAllCamera();
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormWebcam_SizeChanged(object sender, EventArgs e)
        {
            picResult.Width = (this.Width / 2) + 100;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void rd_auto_CheckedChanged(object sender, EventArgs e)
        {
            btnRead.Enabled = rd_manual.Checked;
        }
    }
}
