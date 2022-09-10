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

namespace data_preprocessor
{
    public partial class frmImagePreprocessor : Form
    {
        public frmImagePreprocessor()
        {
            InitializeComponent();
        }

        private void btnSelectInputDirectory_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog l_ofd = new FolderBrowserDialog();
            if (l_ofd.ShowDialog() == DialogResult.OK)
            {
                lblInputDirectory.Text = l_ofd.SelectedPath;
            }
        }

        private void btnSelectOutputDirectory_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog l_ofd = new FolderBrowserDialog();
            if (l_ofd.ShowDialog() == DialogResult.OK)
            {
                lblOutputDirectory.Text = l_ofd.SelectedPath;
            }
        }

        List<List<List<int>>> ConvertImage(Image a_image)
        {
            List<List<List<int>>> l_result = new List<List<List<int>>>();

            int l_acceptable_processed_height = 100;
            int l_acceptable_processed_width = 100;

            // Add R, G, B channels
            l_result.Add(new List<List<int>>());
            l_result.Add(new List<List<int>>());
            l_result.Add(new List<List<int>>());

            // Add labels for the individual channel matrices
            List<List<int>> l_red_matrix = l_result[0];
            List<List<int>> l_green_matrix = l_result[1];
            List<List<int>> l_blue_matrix = l_result[2];

            Bitmap l_bitmap = new Bitmap(a_image);

            for (int i = 0; i < l_acceptable_processed_height; i++)
            {
                l_red_matrix.Add(new List<int>());
                l_green_matrix.Add(new List<int>());
                l_blue_matrix.Add(new List<int>());
                for (int j = 0; j < l_acceptable_processed_width; j++)
                {
                    Color l_color = l_bitmap.GetPixel(j, i);
                    l_red_matrix[i].Add((int)l_color.R);
                    l_green_matrix[i].Add((int)l_color.G);
                    l_blue_matrix[i].Add((int)l_color.B);
                }
            }

            return l_result;

        }

        void WriteProcessedImageToFile(string a_file_path, List<List<List<int>>> a_processed_image)
        {
            BinaryWriter l_writer = new BinaryWriter(new FileStream(a_file_path, FileMode.Create));
            l_writer.Write(a_processed_image[0].Count);
            l_writer.Write(a_processed_image[0][0].Count);
            for (int i = 0; i < a_processed_image.Count; i++)
            {
                for (int j = 0; j < a_processed_image[0].Count; j++)
                {
                    for (int k = 0; k < a_processed_image[0][0].Count; k++)
                    {
                        l_writer.Write(a_processed_image[i][j][k]);
                    }
                }
            }
            l_writer.Close();
        }

        private void btnProcessAndDump_Click(object sender, EventArgs e)
        {
            string[] l_files_in_input = Directory.GetFiles(lblInputDirectory.Text);
            foreach (string l_input_path in l_files_in_input)
            {
                Image l_image = Image.FromFile(l_input_path);
                var l_converted = ConvertImage(l_image);
                WriteProcessedImageToFile(lblOutputDirectory.Text + "/" + Path.GetFileName(l_input_path) + "-processed.bin", l_converted);
            }
        }
    }
}
