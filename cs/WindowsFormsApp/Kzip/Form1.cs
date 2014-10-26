using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace Kzip
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            //To make combobox non editable
            comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            comboBox3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            comboBox4.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;

            //Set preferred index to show as default value
            comboBox2.SelectedIndex = 0;
            comboBox3.SelectedIndex = 0;
            comboBox4.SelectedIndex = 0;
        }

        private void eddToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void ssToolStripMenuItem_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            saveFileDialog1.ShowDialog();
        }
        private void button4_Click(object sender, EventArgs e)
        {
            startCompression();
        }
        private void tabPage1_Click(object sender, EventArgs e)
        {

        }


        private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void saveFileDialog_FileOk(object sender, EventArgs e)
        {
            String name = saveFileDialog1.FileName;
            comboBox1.Text = name;
        }

        private void openFileDialog1_FileOk(object sender, CancelEventArgs e)
        {
            String name = openFileDialog1.FileName;
            comboBox5.Text = name;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            openFileDialog1.ShowDialog();
        }

        private void startCompression()
        {
            //String inputFileName = @"C:\Users\Calvin\Pictures\emma stone.PNG";
            //String outputFileName = @"C:\Users\Calvin\Pictures\Untitled.png.zip";
            String inputFilePath = openFileDialog1.FileName;
            SharpCompress.Common.CompressionInfo info = new SharpCompress.Common.CompressionInfo();
            FileStream outStream = new FileStream(
                saveFileDialog1.FileName, FileMode.Truncate, FileAccess.Write);
            //SharpCompress.Writer.Zip.ZipWriter a =
            //    new SharpCompress.Writer.Zip.ZipWriter(outStream, info, "");
            var archive = SharpCompress.Archive.Zip.ZipArchive.Create();
            archive.AddEntry(Path.GetFileName(inputFilePath), new System.IO.FileInfo(inputFilePath));
            archive.SaveTo(outStream, info);
        }
    }
}
