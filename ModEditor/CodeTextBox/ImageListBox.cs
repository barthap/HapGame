using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Drawing.Imaging;
using System.Windows.Forms;

namespace Hapex.CodeEdit
{
    public partial class ImageListBox : ListBox
    {
        #region Members
        private Size mp_ImageSize = new Size(16, 16);
        #endregion

        #region Constructors
        public ImageListBox()
        {
            InitializeComponent();

            // set draw mode to owner draw
            this.DrawMode = DrawMode.OwnerDrawFixed;	
        }
        #endregion

        #region Properties
        [Browsable(true),Category("ImageListBox"),Description("Every image in the ImageListBox will be resized to this size.")]
        public Size ImageSize
        {
            get { return mp_ImageSize; }
            set { mp_ImageSize = value; }
        }
        #endregion

        #region Overridden methods
        public void AddItem(ImageListItem imageListItem)
        {
            this.Items.Add(imageListItem);
        }
        public void RemoveItem(ImageListItem imageListItem)
        {
            this.Items.Remove(imageListItem);
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            base.OnPaint(pe);
        }
        protected override void OnDrawItem(DrawItemEventArgs e)
        {
            if (this.DesignMode)
            {
                base.OnDrawItem(e);
                return;
            }

            // draw background & focus rect
            e.DrawBackground();
            e.DrawFocusRectangle();

            // check if it is an item from the Items collection
            if (e.Index < 0)

                // not an item, draw the text (indented)
                e.Graphics.DrawString(
                    this.Text, 
                    e.Font, 
                    new SolidBrush(e.ForeColor), 
                    e.Bounds.Left + mp_ImageSize.Width, 
                    e.Bounds.Top);

            else
            {

                // check if item is an ImageComboItem
                if (this.Items[e.Index].GetType() == typeof(ImageListItem))
                {

                    // get item to draw
                    ImageListItem item = (ImageListItem)this.Items[e.Index];

                    // get forecolor & font
                    Color forecolor = (item.ForeColor != Color.FromKnownColor(KnownColor.Transparent)) ? item.ForeColor : e.ForeColor;
                    Font font = item.Bold ? new Font(e.Font, FontStyle.Bold) : e.Font;


                    // draw the item...
                    if (item.Image != null)
                    {
                        #region If has image...
                        //Resize image if necessary...
                        if (
                            item.Image.Width != mp_ImageSize.Width ||
                            item.Image.Height != mp_ImageSize.Height
                            )
                        {
                            ResizeImage(item.Image, mp_ImageSize.Width, mp_ImageSize.Height);
                        }

                        //Draw image
                        e.Graphics.DrawImage(
                            item.Image,
                            e.Bounds.Left,
                            e.Bounds.Top);

                        //Draw text
                        e.Graphics.DrawString(
                            item.Text,
                            font,
                            new SolidBrush(forecolor),
                            e.Bounds.Left + mp_ImageSize.Width,
                            e.Bounds.Top);
                        #endregion
                    }
                    else
                    {
                        #region If doesn'h has image
                        // draw text (indented)
                        e.Graphics.DrawString(
                            item.Text,
                            font,
                            new SolidBrush(forecolor),
                            e.Bounds.Left + mp_ImageSize.Width,
                            e.Bounds.Top);
                        #endregion
                    }

                }
                else

                    // it is not an ImageComboItem, draw it
                    e.Graphics.DrawString(this.Items[e.Index].ToString(), e.Font, new SolidBrush(e.ForeColor), e.Bounds.Left + mp_ImageSize.Width, e.Bounds.Top);

            }

            base.OnDrawItem(e);
        }
        #endregion

        #region Private methods
        private static Image ResizeImage(Image imgPhoto, int width, int height)
        {
            int sourceWidth = imgPhoto.Width;
            int sourceHeight = imgPhoto.Height;
            int sourceX = 0;
            int sourceY = 0;
            int destX = 0;
            int destY = 0;

            float nPercent = 0;
            float nPercentW = 0;
            float nPercentH = 0;

            nPercentW = ((float)width / (float)sourceWidth);
            nPercentH = ((float)height / (float)sourceHeight);
            if (nPercentH < nPercentW)
            {
                nPercent = nPercentH;
                destX = System.Convert.ToInt16((width -
                              (sourceWidth * nPercent)) / 2);
            }
            else
            {
                nPercent = nPercentW;
                destY = System.Convert.ToInt16((height -
                              (sourceHeight * nPercent)) / 2);
            }

            int destWidth = (int)(sourceWidth * nPercent);
            int destHeight = (int)(sourceHeight * nPercent);

            Bitmap bmPhoto = new Bitmap(width, height,
                              PixelFormat.Format24bppRgb);
            bmPhoto.SetResolution(imgPhoto.HorizontalResolution,
                             imgPhoto.VerticalResolution);

            Graphics grPhoto = Graphics.FromImage(bmPhoto);
            grPhoto.Clear(Color.Red);
            grPhoto.InterpolationMode =
                    InterpolationMode.HighQualityBicubic;

            grPhoto.DrawImage(imgPhoto,
                new Rectangle(destX, destY, destWidth, destHeight),
                new Rectangle(sourceX, sourceY, sourceWidth, sourceHeight),
                GraphicsUnit.Pixel);

            grPhoto.Dispose();
            return bmPhoto;
        }
        #endregion
    }
}
