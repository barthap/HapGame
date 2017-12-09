using System.Drawing;

namespace Hapex.CodeEdit
{
    public class ImageListItem : object
    {
        #region Members
        // forecolor: transparent = inherit
		private Color mp_Forecolor = Color.FromKnownColor(KnownColor.Transparent);
		private bool mp_Bold = false;
        private Image mp_Image = null;
		private object mp_Tag = null;
		private string mp_Text = null;
        #endregion

        #region Constructors
        public ImageListItem()
		{
		}
		public ImageListItem(string text) 
		{
			mp_Text = text;	
		}
		public ImageListItem(string text, Image image)
		{
			mp_Text = text;
			mp_Image = image;
		}
		public ImageListItem(string text, Image image, bool bold)
		{
			mp_Text = text;
			mp_Image = image;
			mp_Bold = bold;
		}
		public ImageListItem(string text, Image image, bool bold, Color foreColor)
		{
            mp_Text = text;
            mp_Image = image;
            mp_Bold = bold;
			mp_Forecolor = foreColor;
		}
        public ImageListItem(string text, Image image, bool bold, Color foreColor, object tag)
		{
            mp_Text = text;
            mp_Image = image;
            mp_Bold = bold;
            mp_Forecolor = foreColor;
			mp_Tag = tag;
        }
        #endregion

        #region Properties
        public Color ForeColor 
		{
			get 
			{
				return mp_Forecolor;
			}
			set
			{
				mp_Forecolor = value;
			}
		}
		public Image Image
		{
			get 
			{
				return mp_Image;
			}
			set 
			{
				mp_Image = value;
			}
		}
		public bool Bold
		{
			get
			{
				return mp_Bold;
			}
			set
			{
				mp_Bold = value;
			}
		}
		public object Tag
		{
			get
			{
				return mp_Tag;
			}
			set
			{
				mp_Tag = value;
			}
		}
		public string Text 
		{
			get
			{
				return mp_Text;
			}
			set
			{
				mp_Text = value;
			}
        }
        #endregion

        #region Overridden methods
        // ToString() should return item text
		public override string ToString() 
		{
			return mp_Text;
        }
        #endregion
    }
}
