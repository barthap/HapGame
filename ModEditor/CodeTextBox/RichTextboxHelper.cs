using System;
using System.Windows.Forms;

namespace Hapex.CodeEdit
{
    internal class RichTextboxHelper
    {
        #region Methods
        public static string GetLastWord(RichTextBox richTextbox)
        {
            int pos = richTextbox.SelectionStart;

            while (pos > 1)
            {
                string substr = richTextbox.Text.Substring(pos - 1, 1);

                if (Char.IsWhiteSpace(substr, 0))
                {
                    return richTextbox.Text.Substring(pos, richTextbox.SelectionStart - pos);
                }

                pos--;
            }

            return richTextbox.Text.Substring(0, richTextbox.SelectionStart);
        }
        public static string GetLastLine(RichTextBox richTextbox)
        {
            int charIndex = richTextbox.SelectionStart;
            int currentLineNumber = richTextbox.GetLineFromCharIndex(charIndex);

            // the carriage return hasn't happened yet... 
            //      so the 'previous' line is the current one.
            string previousLineText;
            if (richTextbox.Lines.Length <= currentLineNumber)
                previousLineText = richTextbox.Lines[richTextbox.Lines.Length - 1];
            else
                previousLineText = richTextbox.Lines[currentLineNumber];

            return previousLineText;
        }       
        public static string GetCurrentLine(RichTextBox richTextbox)
        {
            int charIndex = richTextbox.SelectionStart;
            int currentLineNumber = richTextbox.GetLineFromCharIndex(charIndex);

            if (currentLineNumber < richTextbox.Lines.Length)
            {
                return richTextbox.Lines[currentLineNumber];
            }
            else
            {
                return string.Empty;
            }
        }
        public static int GetCurrentLineStartIndex(RichTextBox richTextbox)
        {
            return richTextbox.GetFirstCharIndexOfCurrentLine();
        }
        #endregion
    }
}
