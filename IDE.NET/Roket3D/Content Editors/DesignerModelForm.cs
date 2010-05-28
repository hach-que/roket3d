using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using IrrlichtNETCP;
using IrrlichtNETCP.Inheritable;
using System.Threading;
using DockPanelSuite;

namespace Roket3D.ContentEditors
{
    public partial class DesignerModelForm : EditorBase
    {
        public MainForm MainForm;
        private IrrlichtDevice device;
        private Rect dimensions;
        private Boolean deviceClosed = false;
        private Boolean isActive = true;
        private IntPtr windowHandle;
        private Thread deviceThread = null;
        private AnimatedMeshSceneNode node;
        private GUIStaticText modelInformation;
        private Boolean isRotating = false;
        private Double xRotate = 0;
        private Double yRotate = 0;
        private CameraSceneNode cam;
        private Point originalPoint = new Point(0, 0);
        private String currentTextureName = "";

        public DesignerModelForm(MainForm myMain)
        {
            InitializeComponent();
            this.MainForm = myMain;
        }

        private void DesignerModelForm_Load(object sender, EventArgs e)
        {
            this.windowHandle = this.Handle;

            this.textureToolStripComboBox.Items.Clear();
            this.textureToolStripComboBox.Items.AddRange(this.MainForm.GetAllFilesOfType(Xml.FileType.Image).ToArray());
            if (this.textureToolStripComboBox.Items.Count == 0)
                this.textureToolStripComboBox.Enabled = false;
            else
            {
                try
                {
                    String tex = this.File.Settings["texture"];
                    Roket3D.Xml.File imageFile = null;
                    foreach (Roket3D.Xml.File f in this.textureToolStripComboBox.Items)
                    {
                        if (f.ToString() == tex)
                        {
                            imageFile = f;
                        }
                    }
                    if (imageFile == null)
                    {
                        // The image file currently associated with the model is not
                        // currently in the project.  Here we should prompt the user
                        // to add it to the project.
                        //
                        // TODO: Implement add-to-project prompt.
                        this.textureToolStripComboBox.Items.Insert(0, "<image not in project>");
                        this.textureToolStripComboBox.SelectedIndex = 0;
                        currentTextureName = tex;
                    }
                    else
                    {
                        // Just select the correct index for the combo box.
                        this.textureToolStripComboBox.SelectedItem = imageFile;
                        currentTextureName = this.textureToolStripComboBox.SelectedItem.ToString();
                    }
                }
                catch (KeyNotFoundException)
                {
                    this.textureToolStripComboBox.SelectedIndex = 0;
                    currentTextureName = this.textureToolStripComboBox.SelectedItem.ToString();
                }
            }
        }

        private void DesignerModelForm_Shown(object sender, EventArgs e)
        {
            //GenerateDevice();

            deviceThread = new Thread(new ThreadStart(RunIrrlichtLoop));
            deviceThread.Start();
        }

        private void textureToolStripComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Change the texture.
            if (currentTextureName != "") // when the index is first begin set, this will still be "".  Since
                                          // we dont want to prompt the user when the first open the model
                                          // this prevents the message from showing.
            {
                if (textureToolStripComboBox.Items[0].ToString() == "<image not in project>" &&
                    textureToolStripComboBox.SelectedIndex == 0)
                {
                    MessageBox.Show("Please select a valid texture.", "Invalid Texture", MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }
                else if (textureToolStripComboBox.Items[0].ToString() == "<image not in project>")
                {
                    textureToolStripComboBox.Items.RemoveAt(0);
                }
            }

            if (device != null)
            {
                device.FileSystem.WorkingDirectory = Environment.CurrentDirectory;
                node.SetMaterialTexture(0, device.VideoDriver.GetTexture(
                    textureToolStripComboBox.SelectedItem.ToString()
                    ));
                currentTextureName = textureToolStripComboBox.SelectedItem.ToString();
            }
        }

        private void RegenerateDevice()
        {
            // If the device is null, then it hasn't been
            // set up at all yet.
            if (device != null)
            {
                // Close the existing device.
                deviceClosed = true;
                deviceThread.Join();

                deviceClosed = false;
                this.Refresh();

                //GenerateDevice();

                deviceThread = new Thread(new ThreadStart(RunIrrlichtLoop));
                deviceThread.Start();
            }
        }

        private void GenerateDevice()
        {
            device = new IrrlichtDevice(DriverType.Direct3D9,
                                            new Dimension2D(640, 480),
                                            32, false, true, true, true, this.windowHandle);
            this.dimensions = new Rect(0, 0, this.Width, this.Height);

            /*Now we set the caption of the window to some nice text. Note that there is a 'L' in front of the string: the Irrlicht Engine uses
wide character strings when displaying text.
            */
            device.WindowCaption = "Hello World! - Irrlicht Engine Demo";
            device.FileSystem.WorkingDirectory = Environment.CurrentDirectory;

            //
            VideoDriver driver = device.VideoDriver;
            SceneManager smgr = device.SceneManager;
            GUIEnvironment guienv = device.GUIEnvironment;

            modelInformation = guienv.AddStaticText("Hello World! This is the Irrlicht Software engine!",
                new Rect(new Position2D(10, 36), new Dimension2D(500, 16)), true, false, guienv.RootElement, -1, true);

            // We need to write the irrlicht font to the temporary folder so that the
            // engine can read it.
            if (!System.IO.Directory.Exists(System.IO.Path.GetTempPath() + "\\Roket3D"))
                System.IO.Directory.CreateDirectory(System.IO.Path.GetTempPath() + "\\Roket3D");
            Roket3D.Properties.Resources.irrlicht_font.Save(System.IO.Path.GetTempPath() + "\\Roket3D\\irrlicht_font.png");
            modelInformation.OverrideFont = guienv.GetFont(System.IO.Path.GetTempPath() + "\\Roket3D\\irrlicht_font.png");
            System.IO.File.Delete(System.IO.Path.GetTempPath() + "\\Roket3D\\irrlicht_font.png");

            //
            AnimatedMesh mesh = smgr.GetMesh(this.Path);
            node = smgr.AddAnimatedMeshSceneNode(mesh);
            //node.SetMaterialFlag(MaterialFlag.NormalizeNormals, false);
            //node.SetMaterialFlag(MaterialFlag.PointCloud, false);
            //node.SetMaterialFlag(MaterialFlag.Wireframe, false);
            //node.SetMaterialFlag(MaterialFlag.BackFaceCulling, false);
            //node.SetMaterialFlag(MaterialFlag.ZBuffer, false);
            //node.SetMaterialFlag(MaterialFlag.ZWriteEnable, false);
            //node.SetMaterialFlag(MaterialFlag.TrilinearFilter, false);
            //MessageBox.Show(((Int32)MaterialFlag.TrilinearFilter).ToString());
            //node.SetMaterialFlag(MaterialFlag.BilinearFilter, false);
            //node.SetMaterialFlag(MaterialFlag.AnisotropicFilter, false);
            //node.SetMaterialFlag(MaterialFlag.FogEnable, false);
            //node.SetMaterialFlag(MaterialFlag.GouraudShading, false);
            //node.SetMaterialFlag(MaterialFlag.TextureWrap, false);
            //MessageBox.Show(((Int32)MaterialFlag.MaterialFlagCount).ToString());

            //
            if (node != null)
            {
                node.SetMaterialFlag(MaterialFlag.Lighting, false);
                node.SetMaterialTexture(0, driver.GetTexture(currentTextureName));
            }
            //
            cam = smgr.AddCameraSceneNode(smgr.RootSceneNode);
            //cam.Position = new Vector3D(0, 30, -40);
            cam.Target = new Vector3D(0, 0, 0);

            float newY = (float)Math.Sin(this.yRotate * Math.PI / 180) * 80;
            float dist = (float)Math.Cos(this.yRotate * Math.PI / 180) * 80;

            float newX = (float)Math.Sin(this.xRotate * Math.PI / 180) * dist;
            float newZ = (float)Math.Cos(this.xRotate * Math.PI / 180) * dist;

            cam.Position = new Vector3D(newX, newY, newZ);
        }

        public override void OnActiveTabChanged()
        {
            this.CheckIfActive();
        }

        private void CheckIfActive()
        {
            this.isActive = (this.MainForm.dockWorkspace.ActiveTab == this);

            if (!this.isActive)
            {
                if (!deviceClosed)
                {
                    deviceClosed = true;
                    deviceThread.Join();
                }
            }
            else
            {
                if (deviceClosed)
                {
                    deviceClosed = false;
                    GenerateDevice();

                    deviceThread = new Thread(new ThreadStart(RunIrrlichtLoop));
                    deviceThread.Start();
                }
            }
        }

        private void RunIrrlichtLoop()
        {
            GenerateDevice();
            
            while (device.Run() && !deviceClosed)
            {
                modelInformation.Text = "Model Current Texture: " + currentTextureName;

                device.VideoDriver.BeginScene(true, true, new IrrlichtNETCP.Color(255, 100, 101, 140));
                device.SceneManager.DrawAll();
                device.GUIEnvironment.DrawAll();

                /*device.VideoDriver.Draw2DRectangle(
                        new Rect(
                            device.VideoDriver.ViewPort.X,
                            device.VideoDriver.ViewPort.Y,
                            device.VideoDriver.ViewPort.Width,
                            device.VideoDriver.ViewPort.Height
                            ),
                        IrrlichtNETCP.Color.From(255,244, 243, 241)
                        );*/

                device.VideoDriver.EndScene();
            }

            device.Dispose();
        }

        // This is the event reciever.
        /*private bool device_OnEvent(Event e)
        {
            // See if the mouse left cursor was just pressed.
            // If so, we should go into rotate model mode.
            if (e.Type == EventType.MouseInputEvent &&
                e.MouseInputEvent == MouseInputEvent.LMousePressedDown)
            {
                device.CursorControl.Visible = false;
                isRotating = true;
                originalPoint = e.MousePosition.dotNETPoint;
            }
            // If we are in rotate mode, move the model depending
            // on the mouse position.
            if (e.Type == EventType.MouseInputEvent &&
                isRotating &&
                e.MouseInputEvent != MouseInputEvent.LMouseLeftUp)
            {
                // Do the camera rotation here.
                this.xRotate += e.MousePosition.X - originalPoint.X;
                this.yRotate += e.MousePosition.Y - originalPoint.Y;

                cam.Position.SetLength(300);
                cam.Position.RotateXYBy(this.yRotate, cam.Target);
                cam.Position.RotateXZBy(this.xRotate, cam.Target);
            }
            // See if the mouse left cursor was released
            if (e.Type == EventType.MouseInputEvent &&
                e.MouseInputEvent == MouseInputEvent.LMouseLeftUp)
            {
                device.CursorControl.Visible = true;
                isRotating = false;
            }

            return false;
        }*/

        private void DesignerModelForm_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (!deviceClosed)
            {
                deviceClosed = true;
                deviceThread.Abort();
                device.Close();
            }
        }

        public override void TerminateThread()
        {
            if (!deviceClosed)
            {
                deviceClosed = true;
                deviceThread.Abort();
                device.Close();
            }
        }

        public override void OnResizeEnd()
        {
            if (this.dimensions.Width != this.Width || this.dimensions.Height != this.Height)
            {
                RegenerateDevice();
            }
        }

        private void DesignerModelForm_ResizeEnd(object sender, EventArgs e)
        {
            OnResizeEnd();
        }

        private void DesignerModelForm_MouseDown(object sender, MouseEventArgs e)
        {
            // See if the mouse left cursor was just pressed.
            // If so, we should go into rotate model mode.
            if (e.Button == MouseButtons.Left)
            {
                device.CursorControl.Visible = false;
                isRotating = true;
                originalPoint = e.Location;
            }
        }

        private void DesignerModelForm_MouseMove(object sender, MouseEventArgs e)
        {
            // If we are in rotate mode, move the model depending
            // on the mouse position.
            if (isRotating)
            {
                // Do the camera rotation here.
                this.xRotate += e.X - originalPoint.X;
                this.yRotate += e.Y - originalPoint.Y;
                if (this.yRotate > 89) this.yRotate = 89;
                if (this.yRotate < -89) this.yRotate = -89;
                originalPoint = e.Location;
                
                float newY = (float)Math.Sin(this.yRotate * Math.PI / 180) * 80;
                float dist = (float)Math.Cos(this.yRotate * Math.PI / 180) * 80;

                float newX = (float)Math.Sin(this.xRotate * Math.PI / 180) * dist;
                float newZ = (float)Math.Cos(this.xRotate * Math.PI / 180) * dist;

                cam.Position = new Vector3D(newX, newY, newZ);
            }
        }

        private void DesignerModelForm_MouseUp(object sender, MouseEventArgs e)
        {
            // See if the mouse left cursor was released
            if (e.Button == MouseButtons.Left)
            {
                device.CursorControl.Visible = true;
                isRotating = false;
            }
        }
    }
}
