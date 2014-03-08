using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    public partial class Form2 : Form
    {
        public Form2()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            MessageBox.Show("시작");
        }

        public void printString(string inputString)
        {
            MessageBox.Show(inputString);
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            Form1 newForm = new Form1();
            newForm.option = this.checkBox5.Checked;
            newForm.Show();
        }

        private void label1_Click(object sender, EventArgs e)
        {
            MessageBox.Show("OK");
        }

        private void Form2_FormClosing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            DialogResult dr = MessageBox.Show("Do you REALLY want to close this app?",
                "Closing event!", MessageBoxButtons.YesNoCancel);
            if (dr == DialogResult.No || dr == DialogResult.Cancel)
                e.Cancel = true;
            else
                e.Cancel = false;             
        }

        private void checkBox5_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void groupBox1_Enter(object sender, EventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}
