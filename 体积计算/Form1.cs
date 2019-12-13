using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace 体积计算
{
    public partial class form1 : Form
    {
        public form1()
        {
            InitializeComponent();
            calc();
            kgCalc();
        }

        void calc()
        {
            UInt32 length;
            UInt32 width;
            UInt32 height;
            String err="";

            try
            {
                try
                {
                    length = (UInt32)(float.Parse(lengthText.Text) * 10);
                    if (length == 0 || length > 1000)
                        throw new MyException();
                }
                catch
                {
                    err = "长度不对";
                    throw;
                }

                try
                {
                    width = (UInt32)(float.Parse(widthText.Text) * 10);
                    if (width == 0 || width > 1000)
                        throw new MyException();
                }
                catch
                {
                    err = "宽度不对";
                    throw;
                }

                try
                {
                    height = (UInt32)(float.Parse(heightText.Text) * 10);
                    if (height == 0 || height > 1000)
                        throw new MyException();
                }
                catch
                {
                    err = "高度不对";
                    throw;
                }

                UInt32 result = (1000 / length) * (1000 / width) * (1000 / height);

                UInt32 r_len = 1000 % length;
                UInt32 r_wid = 1000 % width;
                UInt32 r_high = 1000 % height;

                if (r_len >= width || r_len >= height)
                {
                    UInt32 r1 = 0, r2 = 0;
                    if (r_len >= width)
                        r1 = (r_len / width) * (1000 / length) * (1000 / height);
                    if (r_len >= height)
                        r2 = (r_len / height) * (1000 / length) * (1000 / width);
                    result += max(r1, r2);
                }

                if (r_wid >= length || r_wid >= height)
                {
                    UInt32 r1 = 0, r2 = 0;
                    if (r_wid >= length)
                        r1 = (r_wid / length) * ((1000 - r_len) / width) * (1000 / height);
                    if (r_wid >= height)
                        r2 = (r_wid / height) * ((1000 - r_len) / width) * (1000 / length);
                    result += max(r1, r2);
                }

                if (r_high >= width || r_high >= length)
                {
                    UInt32 r1 = 0, r2 = 0;
                    if (r_high >= width)
                        r1 = (r_high / width) * ((1000 - r_len) / length) * ((1000 - r_wid) / height);
                    if (r_high >= height)
                        r2 = (r_high / height) * ((1000 - r_len) / length) * ((1000 - r_wid) / width);
                    result += max(r1, r2);
                }

                resultLabel.Text = result.ToString();
            }
            catch
            {
                resultLabel.Text = err;
            }
        }

        private UInt32 max(UInt32 val1, UInt32 val2)
        {
            return val1 > val2 ? val1 : val2;
        }

        private void button1_Click(object sender, EventArgs e)
        {
            calc();
        }

        private void lengthText_TextChanged(object sender, EventArgs e)
        {
            calc();
        }

        private void kgText_TextChanged(object sender, EventArgs e)
        {
            kgCalc();
        }

        private void kgCalc()
        {
            try
            {
                float kg = float.Parse(kgText.Text);
                if (kg < 0.001 || kg > 200.0)
                    throw new MyException();
                kgResult.Text = ((UInt32)(200.0 / kg)).ToString();
            }
            catch
            {
                kgResult.Text = "重量非法";
            }
        }
    }

    public class MyException : Exception
    {
    }
}
