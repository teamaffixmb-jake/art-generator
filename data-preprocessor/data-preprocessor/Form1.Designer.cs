namespace data_preprocessor
{
    partial class frmImagePreprocessor
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
            this.btnSelectOutputDirectory = new System.Windows.Forms.Button();
            this.btnSelectInputDirectory = new System.Windows.Forms.Button();
            this.btnProcessAndDump = new System.Windows.Forms.Button();
            this.lblInputDirectory = new System.Windows.Forms.Label();
            this.lblOutputDirectory = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // btnSelectOutputDirectory
            // 
            this.btnSelectOutputDirectory.Location = new System.Drawing.Point(16, 74);
            this.btnSelectOutputDirectory.Name = "btnSelectOutputDirectory";
            this.btnSelectOutputDirectory.Size = new System.Drawing.Size(187, 22);
            this.btnSelectOutputDirectory.TabIndex = 0;
            this.btnSelectOutputDirectory.Text = "Select Dataset Output Directory";
            this.btnSelectOutputDirectory.UseVisualStyleBackColor = true;
            this.btnSelectOutputDirectory.Click += new System.EventHandler(this.btnSelectOutputDirectory_Click);
            // 
            // btnSelectInputDirectory
            // 
            this.btnSelectInputDirectory.Location = new System.Drawing.Point(11, 23);
            this.btnSelectInputDirectory.Name = "btnSelectInputDirectory";
            this.btnSelectInputDirectory.Size = new System.Drawing.Size(187, 22);
            this.btnSelectInputDirectory.TabIndex = 1;
            this.btnSelectInputDirectory.Text = "Select Dataset Input Directory";
            this.btnSelectInputDirectory.UseVisualStyleBackColor = true;
            this.btnSelectInputDirectory.Click += new System.EventHandler(this.btnSelectInputDirectory_Click);
            // 
            // btnProcessAndDump
            // 
            this.btnProcessAndDump.Location = new System.Drawing.Point(108, 129);
            this.btnProcessAndDump.Name = "btnProcessAndDump";
            this.btnProcessAndDump.Size = new System.Drawing.Size(187, 22);
            this.btnProcessAndDump.TabIndex = 2;
            this.btnProcessAndDump.Text = "Process and Dump Data";
            this.btnProcessAndDump.UseVisualStyleBackColor = true;
            this.btnProcessAndDump.Click += new System.EventHandler(this.btnProcessAndDump_Click);
            // 
            // lblInputDirectory
            // 
            this.lblInputDirectory.AutoSize = true;
            this.lblInputDirectory.Location = new System.Drawing.Point(12, 48);
            this.lblInputDirectory.Name = "lblInputDirectory";
            this.lblInputDirectory.Size = new System.Drawing.Size(79, 13);
            this.lblInputDirectory.TabIndex = 3;
            this.lblInputDirectory.Text = "Input Directory:";
            // 
            // lblOutputDirectory
            // 
            this.lblOutputDirectory.AutoSize = true;
            this.lblOutputDirectory.Location = new System.Drawing.Point(13, 99);
            this.lblOutputDirectory.Name = "lblOutputDirectory";
            this.lblOutputDirectory.Size = new System.Drawing.Size(87, 13);
            this.lblOutputDirectory.TabIndex = 4;
            this.lblOutputDirectory.Text = "Output Directory:";
            // 
            // frmImagePreprocessor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(402, 174);
            this.Controls.Add(this.lblOutputDirectory);
            this.Controls.Add(this.lblInputDirectory);
            this.Controls.Add(this.btnProcessAndDump);
            this.Controls.Add(this.btnSelectInputDirectory);
            this.Controls.Add(this.btnSelectOutputDirectory);
            this.Name = "frmImagePreprocessor";
            this.Text = "Image Preprocessor";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnSelectOutputDirectory;
        private System.Windows.Forms.Button btnSelectInputDirectory;
        private System.Windows.Forms.Button btnProcessAndDump;
        private System.Windows.Forms.Label lblInputDirectory;
        private System.Windows.Forms.Label lblOutputDirectory;
    }
}

