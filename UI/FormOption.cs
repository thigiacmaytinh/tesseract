using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using TGMTcs;
using TGMT;

namespace UI
{
    public partial class FormOption : Form
    {
        static FormOption m_instance;

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public FormOption()
        {
            InitializeComponent();         
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static FormOption GetInstance()
        {
            if (m_instance == null)
                m_instance = new FormOption();
            return m_instance;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void FormOption_Load(object sender, EventArgs e)
        {
            txt_lineHeight.Text = Program.lineHeight.ToString();
            txt_maxWidth.Text = Program.maxWidth.ToString();
            chk_copyToClipboard.Checked = Program.copyToClipboard;
            chk_beep.Checked = Program.playSound;
        }

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////

        private void btn_save_Click(object sender, EventArgs e)
        {
            Program.lineHeight = int.Parse(txt_lineHeight.Text);
            TGMTregistry.GetInstance().SaveValue("lineHeight", Program.lineHeight);

            Program.maxWidth = int.Parse(txt_maxWidth.Text);
            TGMTregistry.GetInstance().SaveValue("maxWidth", Program.maxWidth);

            Program.copyToClipboard = chk_copyToClipboard.Checked;
            TGMTregistry.GetInstance().SaveValue("copyToClipboard", Program.copyToClipboard);

            Program.playSound = chk_beep.Checked;
            TGMTregistry.GetInstance().SaveValue("playSound", Program.playSound);

            FormMain.GetInstance().PrintSuccess("Save thành công");
        }

    }
}
