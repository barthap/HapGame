using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using Hapex.CodeEdit.Intellisense;
using Hapex.CodeEdit.SyntaxHighlight;
using CodeTextBox;

namespace Hapex.CodeEdit
{
    

    public partial class CodeTextBox : RichTextBox
    {

        #region Cursor position

        [Description("Event dotyczący zmiany pozycji kursora")]
        public event EventHandler CursorPositionChanged;

        protected virtual void OnCursorPositionChanged(EventArgs e)
        {
            if (CursorPositionChanged != null)
                CursorPositionChanged(this, e);
        }

        protected override void OnSelectionChanged(EventArgs e)
        {
            if (SelectionLength == 0)
                OnCursorPositionChanged(e);
            else
                base.OnSelectionChanged(e);
        }

        public int CurrentColumn
        {
            get { return CursorPosition.Column(this, SelectionStart); }
        }

        public int CurrentLine
        {
            get { return CursorPosition.Line(this, SelectionStart); }
        }

        public int CurrentPosition
        {
            get { return this.SelectionStart; }
        }

        public int SelectionEnd
        {
            get { return SelectionStart + SelectionLength; }
        }
        #endregion

        #region Members

        private SyntaxHighlighter m_SyntaxHighLighter = new SyntaxHighlighter();
        private IntellisenseManager m_IntellisenseManager;
        private ImageListBox mp_IntellisenseBox;
        private TreeView mp_IntellisenseTree;
        private Keys mp_IntellisenseKey = Keys.Space;
        
        #region Drawing
        /// <summary>
        /// Enables or disables control's painting - internal
        /// </summary>
        private bool mp_EnablePainting = true;
        #endregion

        #region Word lists
        private List<string> mp_CodeWords_Keywords;
        private List<string> mp_CodeWords_Types;
        private List<string> mp_CodeWords_Functions;
        private List<string> mp_CodeWords_Comments;
        private List<string> mp_CodeWords_ScopeOperators;
        #endregion

        #region Syntax highlightning colors
        private Color mp_CodeColor_Keyword           = Color.Blue;
        private Color mp_CodeColor_Type              = Color.CornflowerBlue;
        private Color mp_CodeColor_Function          = Color.CornflowerBlue;
        private Color mp_CodeColor_Comment           = Color.Green;
        private Color mp_CodeColor_PlainText         = Color.Black;
        private Color mp_CodeColor_String            = Color.DarkRed;
        #endregion

        #region Intellisense images
        private Image mp_CodeImage_Class = Resource._class;
        private Image mp_CodeImage_Event = Resource._event;
        private Image mp_CodeImage_Interface = Resource._interface;
        private Image mp_CodeImage_Method = Resource._method;
        private Image mp_CodeImage_Namespace = Resource._namespace;
        private Image mp_CodeImage_Property = Resource._property;
        #endregion

        #endregion

        #region Properties
        #region Public properties

        #region Word lists
        /// <summary>
        /// Gets or Sets the keywords used for syntax highlightning and intellisense.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the keywords used for syntax highlightning and intellisense.")]
        [Editor("System.Windows.Forms.Design.StringCollectionEditor, System.Design","System.Drawing.Design.UITypeEditor, System.Drawing")]
        public List<string> CodeWords_Keywords
        {
            get { return mp_CodeWords_Keywords; }
            set { mp_CodeWords_Keywords = value; }
        }
        
        /// <summary>
        /// Gets or Sets the types used for syntax highlightning and intellisense.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the types used for syntax highlightning and intellisense.")]
        [Editor("System.Windows.Forms.Design.StringCollectionEditor, System.Design", "System.Drawing.Design.UITypeEditor, System.Drawing")]
        public List<string> CodeWords_Types
        {
            get { return mp_CodeWords_Types; }
            set { mp_CodeWords_Types = value; }
        }
        
        /// <summary>
        /// Gets or Sets the functions used for syntax highlightning and intellisense.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the functions used for syntax highlightning and intellisense.")]
        [Editor("System.Windows.Forms.Design.StringCollectionEditor, System.Design", "System.Drawing.Design.UITypeEditor, System.Drawing")]
        public List<string> CodeWords_Functions
        {
            get { return mp_CodeWords_Functions; }
            set { mp_CodeWords_Functions = value; }
        }
        
        /// <summary>
        /// Gets or Sets the comment strings used for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the comment strings used for syntax highlightning.")]
        [Editor("System.Windows.Forms.Design.StringCollectionEditor, System.Design", "System.Drawing.Design.UITypeEditor, System.Drawing")]
        public List<string> CodeWords_Comments
        {
            get { return mp_CodeWords_Comments; }
            set { mp_CodeWords_Comments = value; }
        }

        /// <summary>
        /// Gets or Sets the comment strings used for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the object separator strings used for intellisense.")]
        [Editor("System.Windows.Forms.Design.StringCollectionEditor, System.Design", "System.Drawing.Design.UITypeEditor, System.Drawing")]
        public List<string> CodeWords_ScopeOperators
        {
            get { return mp_CodeWords_ScopeOperators; }
            set { mp_CodeWords_ScopeOperators = value; }
        }
        #endregion

        #region Syntax highlightning colors
        /// <summary>
        /// Gets or Sets the color of plain texts for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the color of plain texts for syntax highlightning.")]
        public Color CodeColor_PlainText
        {
            get { return mp_CodeColor_PlainText; }
            set { mp_CodeColor_PlainText = value; }
        }

        /// <summary>
        /// Gets or Sets the color of keywords for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the color of keywords for syntax highlightning.")]
        public Color CodeColor_Keyword
        {
            get { return mp_CodeColor_Keyword; }
            set { mp_CodeColor_Keyword = value; }
        }

        /// <summary>
        /// Gets or Sets the color of types for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the color of types for syntax highlightning.")]
        public Color CodeColor_Type
        {
            get { return mp_CodeColor_Type; }
            set { mp_CodeColor_Type = value; }
        }

        /// <summary>
        /// Gets or Sets the color of functions for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the color of functions for syntax highlightning.")]
        public Color CodeColor_Function
        {
            get { return mp_CodeColor_Function; }
            set { mp_CodeColor_Function = value; }
        }

        /// <summary>
        /// Gets or Sets the color of comments for syntax highlightning.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the color of comments for syntax highlightning.")]
        public Color CodeColor_Comment
        {
            get { return mp_CodeColor_Comment; }
            set { mp_CodeColor_Comment = value; }
        }

        /// <summary>
        /// Gets or Sets the color of strings for syntax highlighting.
        /// </summary>
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the color of comments for syntax highlightning.")]
        public Color CodeColor_String
        {
            get { return mp_CodeColor_String; }
            set { mp_CodeColor_String = value; }
        }
        #endregion

        #region Intellisense images
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense image of keywords.")]
        public Image CodeImage_Class
        {
            get { return mp_CodeImage_Class; }
            set { mp_CodeImage_Class = value; }
        }

        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense image of events.")]
        public Image CodeImage_Event
        {
            get { return mp_CodeImage_Event; }
            set { mp_CodeImage_Event = value; }
        }

        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense image of interfaces.")]
        public Image CodeImage_Interface
        {
            get { return mp_CodeImage_Interface; }
            set { mp_CodeImage_Interface = value; }
        }

        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense image of methods.")]
        public Image CodeImage_Method
        {
            get { return mp_CodeImage_Method; }
            set { mp_CodeImage_Method = value; }
        }

        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense image of namespaces.")]
        public Image CodeImage_Namespace
        {
            get { return mp_CodeImage_Namespace; }
            set { mp_CodeImage_Namespace = value; }
        }

        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense image of properties.")]
        public Image CodeImage_Property
        {
            get { return mp_CodeImage_Property; }
            set { mp_CodeImage_Property = value; }
        }
        #endregion

        [DesignerSerializationVisibility(DesignerSerializationVisibility.Content)]
        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the intellisense item tree.")]
        public TreeView IntellisenseTree
        {
            get { return mp_IntellisenseTree; }
            set { mp_IntellisenseTree = value; }
        }

        [Browsable(true), Category("CodeTexbox"), Description("Gets or Sets the key to open intellisense.")]
        public Keys IntellisenseKey
        {
            get { return mp_IntellisenseKey; }
            set { mp_IntellisenseKey = value; }
        }
        #endregion

        #region Internal Properties
        /// <summary>
        /// Enables or disables the control's paint event.
        /// </summary>
        internal bool EnablePainting
        {
            get { return mp_EnablePainting; }
            set { mp_EnablePainting = value; }
        }
        /// <summary>
        /// Gets the intellisense's ImageListBox
        /// </summary>
        internal ImageListBox IntellisenseBox
        {
            get { return mp_IntellisenseBox; }
        }

        #endregion
        #endregion

        #region Constructors
        public CodeTextBox()
        {

            InitializeComponent();

            //Set some defaults...
            this.AcceptsTab = true;
            this.Font = new Font(FontFamily.GenericMonospace, 8f);

            // TODO
            //
            //Do not enable drag and dropping text
            //The same problem, as paste - the onDragDrop event fires, BEFORE the text is written into the textbox
            //Need to be handled in WndPrc
            this.EnableAutoDragDrop = false;
            
            this.DetectUrls = false;
            this.WordWrap = false;
            this.AutoWordSelection = true;

            #region Instantiate Syntax highlightning and Intellisense members
            //Instantiate word lists
            mp_CodeWords_Keywords        = new List<string>();
            mp_CodeWords_Types           = new List<string>();
            mp_CodeWords_Functions       = new List<string>();
            mp_CodeWords_Comments        = new List<string>();
            mp_CodeWords_ScopeOperators  = new List<string>();

            //Instantiate intellisense manager
            m_IntellisenseManager        = new IntellisenseManager(this);

            //Instantiate the intellisense box
            mp_IntellisenseBox           = new ImageListBox();

            //Instantiate intellisense tree
            mp_IntellisenseTree          = new TreeView();
            #endregion

            #region Setup intellisense box
            //Setup intellisense box
            this.Controls.Add(mp_IntellisenseBox);
            mp_IntellisenseBox.Size = new Size(250, 150);
            mp_IntellisenseBox.Visible = false;
            mp_IntellisenseBox.KeyDown += new KeyEventHandler(mp_IntellisenseBox_KeyDown);
            mp_IntellisenseBox.DoubleClick += new EventHandler(mp_IntellisenseBox_DoubleClick);
            #endregion
        }
        #endregion

        #region Public methods
        /// <summary>
        /// Force a full update of syntax highlightning
        /// </summary>
        public void UpdateSyntaxHightlight()
        {
            m_SyntaxHighLighter.Update(this);
            m_SyntaxHighLighter.DoSyntaxHightlight_AllLines(this);
        }
        public void ShowIntellisense()
        {
            m_IntellisenseManager.ShowIntellisenseBox();
        }
        #endregion

        #region Overridden methods

        #region Some overridden native function...
        private const int WM_COPY = 0x301;
        private const int WM_CUT = 0x300;
        private const int WM_PASTE = 0x302;
        private const int WM_CLEAR = 0x303;
        private const int WM_UNDO = 0x304;
        private const int EM_UNDO = 0xC7;
        private const int EM_CANUNDO = 0xC6;
        private const int WM_PAINT = 0x000F;
        private const int WM_CHAR = 0x102;

        [DllImport("user32.dll", SetLastError = true)]
        public static extern bool PostMessage(
        IntPtr hWnd,// handle to destination window
        UInt32 Msg, // message
        IntPtr wParam, // first message parameter
        IntPtr lParam // second message parameter
        );

        /// <summary>
        /// Let control enable and disable it's drawing...
        /// </summary>
        /// <param name="m"></param>
        protected override void WndProc(ref System.Windows.Forms.Message m)
        {
            switch (m.Msg)
            {
                case WM_PAINT:
                    {
                        #region PAINT
                        if (mp_EnablePainting)
                        {
                            base.WndProc(ref m);
                        }
                        else
                        {
                            m.Result = IntPtr.Zero;
                        }
                        #endregion
                    }
                    break;

                case WM_PASTE:
                    {
                        #region PASTE
                        int selectionStart = this.SelectionStart;
                        int selectionLength = 0;
                        
                        string text = Clipboard.GetText();
                        

                        #region Hack to find out correct selection length
                        //Hack...
                        //The text's length readed from the clipboard doesn't match the length pasted to the richTextBox
                        //So I used a dummy richtextbox, to find out the correct length of the text...
                        RichTextBox rtxt = new RichTextBox();
                        rtxt.SelectionStart = 0;
                        rtxt.SelectedText = text;
                        selectionLength = rtxt.Text.Length;
                        #endregion


                        //Paste text from the clipboard...
                        this.SelectedText = text;

                        m_SyntaxHighLighter.DoSyntaxHightlight_Selection(this, selectionStart, selectionLength);
                        #endregion
                    }
                    break;

                case WM_CHAR:
                    {
                        #region CHAR
                        ProcessKeyDownWndPrc((uint)m.WParam, (uint)m.LParam);
                        base.WndProc(ref m);
                        #endregion
                    }
                    break;

                default:
                    {
                        base.WndProc(ref m);
                    }
                    break;
            }
        }
        /// <summary>
        /// I needed this for the WM_PASTE event to fire...
        /// </summary>
        /// <param name="m"></param>
        /// <param name="keyData"></param>
        /// <returns></returns>
        protected override bool ProcessCmdKey(ref Message m, Keys keyData)
        {
            switch (keyData)
            {
                #region Paste
                case Keys.Control | Keys.V:
                    {
                        PostMessage(this.Handle, WM_PASTE, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                case Keys.Shift | Keys.Insert:
                    {
                        PostMessage(this.Handle, WM_PASTE, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                #endregion

                #region Copy
                case Keys.Control | Keys.C:
                    {
                        PostMessage(this.Handle, WM_COPY, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                case Keys.Control | Keys.Insert:
                    {
                        PostMessage(this.Handle, WM_COPY, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                #endregion

                #region Cut
                case Keys.Control | Keys.X:
                    {
                        PostMessage(this.Handle, WM_CUT, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                case Keys.Shift | Keys.Delete:
                    {
                        PostMessage(this.Handle, WM_CUT, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                #endregion

                #region Delete
                case Keys.Control | Keys.Delete:
                    {
                        PostMessage(this.Handle, WM_CLEAR, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                #endregion

                #region Undo
                case Keys.Control | Keys.Z:
                    {
                        PostMessage(this.Handle, WM_UNDO, IntPtr.Zero, IntPtr.Zero);
                        return true;
                    }
                #endregion

                default:
                    return base.ProcessCmdKey(ref m, keyData);
            }
        }
        #endregion

        protected override void OnPaint(PaintEventArgs pe)
        {
            if (EnablePainting)
            {
                base.OnPaint(pe);
            }
        }
        protected override void OnTextChanged(EventArgs e)
        {
            //Syntax Highlight the current line... :)
            m_SyntaxHighLighter.DoSyntaxHightlight_CurrentLine(this);

            base.OnTextChanged(e);
        }
        void mp_IntellisenseBox_KeyDown(object sender, KeyEventArgs e)
        {
            //Let the textbox handle keypresses inside the intellisense box
            OnKeyDown(e);
        }
        void mp_IntellisenseBox_DoubleClick(object sender, EventArgs e)
        {
            m_IntellisenseManager.ConfirmIntellisense();
        }
        protected override void OnKeyDown(KeyEventArgs e)
        {
            #region Show Intellisense
            if (e.KeyData == mp_IntellisenseKey)
            {
                m_IntellisenseManager.ShowIntellisenseBox();
                e.Handled = true;
                this.Focus();
                return;
            }
            #endregion

            if (mp_IntellisenseBox.Visible)
            {
                #region ESCAPE - Hide Intellisense
                if (e.KeyCode == Keys.Escape)
                {
                    m_IntellisenseManager.HideIntellisenseBox();
                    e.Handled = true;
                }
                #endregion

                #region Navigation - Up, Down, PageUp, PageDown, Home, End
                else if (e.KeyCode == Keys.Up)
                {
                    m_IntellisenseManager.NavigateUp(1);
                    e.Handled = true;
                }
                else if (e.KeyCode == Keys.Down)
                {
                    m_IntellisenseManager.NavigateDown(1);
                    e.Handled = true;
                }
                else if (e.KeyCode == Keys.PageUp)
                {
                    m_IntellisenseManager.NavigateUp(10);
                    e.Handled = true;
                }
                else if (e.KeyCode == Keys.PageDown)
                {
                    m_IntellisenseManager.NavigateDown(10);
                    e.Handled = true;
                }
                else if (e.KeyCode == Keys.Home)
                {
                    m_IntellisenseManager.NavigateHome();
                    e.Handled = true;
                }
                else if (e.KeyCode == Keys.End)
                {
                    m_IntellisenseManager.NavigateEnd();
                    e.Handled = true;
                }
                #endregion

                #region Typing - Back
                else if (e.KeyCode == Keys.Back)
                {
                    m_IntellisenseManager.TypeBackspace();
                }
                #endregion

                #region Typing - Brackets
                else if (e.KeyCode == Keys.D9)
                {
                    // Trap the open bracket key, displaying a cheap and
                    // cheerful tooltip if the word just typed is in our tree
                    // (the parameters are stored in the tag property of the node)
                }
                else if (e.KeyCode == Keys.D8)
                {
                    // Close bracket key, hide the tooltip textbox
                }
                #endregion

                #region Typing - TAB and Enter
                else if (e.KeyCode == Keys.Tab)
                {
                    m_IntellisenseManager.ConfirmIntellisense();
                    e.Handled = true;
                }
                else if (e.KeyCode == Keys.Enter)
                {
                    m_IntellisenseManager.ConfirmIntellisense();
                    e.Handled = true;
                }
                #endregion
            }

            this.Focus();
            base.OnKeyDown(e);
        }
        private void ProcessKeyDownWndPrc(uint wParam, uint lParam)
        {
            //We process the keys here instead of the OnKeyDown event, because the OnKeyDown has made
            //some mistakes in converting e.KeyValue to char...

            //Convert wParam to char...
            char c = (char)wParam;


            if (!char.IsLetterOrDigit(c))
            {
                //Char is not alphanumerical
                m_IntellisenseManager.TypeNonAlphaNumerical(c);
            }
            else
            {
                //Char is alphanumerical
                m_IntellisenseManager.TypeAlphaNumerical(c);
            }
        }
        #endregion
    }
     
    internal class CursorPosition
    {
        [System.Runtime.InteropServices.DllImport("user32")]
        public static extern int GetCaretPos(ref Point lpPoint);

        private static int GetCorrection(RichTextBox e, int index)
        {
            Point pt1 = Point.Empty;
            GetCaretPos(ref pt1);
            Point pt2 = e.GetPositionFromCharIndex(index);

            if (pt1 != pt2)
                return 1;
            else
                return 0;
        }

        public static int Line(RichTextBox e, int index)
        {
            int correction = GetCorrection(e, index);
            return e.GetLineFromCharIndex(index) - correction + 1;
        }

        public static int Column(RichTextBox e, int index1)
        {
            int correction = GetCorrection(e, index1);
            Point p = e.GetPositionFromCharIndex(index1 - correction);

            if (p.X == 1)
                return 1;

            p.X = 0;
            int index2 = e.GetCharIndexFromPosition(p);

            int col = index1 - index2 + 1;

            return col;
        }
    }
}
