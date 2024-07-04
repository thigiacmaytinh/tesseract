using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using TGMT;

namespace UI
{
    static class Program
    {
        public static TextReader reader;

        public static int lineHeight = 40;
        public static int maxWidth = 0;
        public static bool copyToClipboard = true;
        public static bool playSound = true;
        public static string wavFile = "mixkit-censorship-beep-1082.wav";

        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(FormMain.GetInstance());
        }
    }
}
