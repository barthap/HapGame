using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace Hapex.CodeEdit.Intellisense
{
    internal class IntellisenseManager
    {
        #region Members
        private CodeTextBox m_CodeTextBox = null;
        private bool m_LastCharWasAScopeOperator = false;
        #endregion

        #region Properties
        #endregion

        #region Constructors
        public IntellisenseManager(CodeTextBox parent)
        {
            m_CodeTextBox = parent;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Shows the intellisense box.
        /// </summary>
        public void ShowIntellisenseBox()
        {
            if (m_CodeTextBox.IntellisenseTree == null)
            {
                return;
            }

            //If our box has no elements, do not show it...
            if (!UpdateIntellisense(false,"",""))
            {
                return;
            }

            ShowIntellisenseBoxWithoutUpdate();
        }
        internal void ShowIntellisenseBoxWithoutUpdate()
        {
            if (m_CodeTextBox.IntellisenseTree == null)
            {
                return;
            }

            //our box has some elements, choose the first
            try
            {
                m_CodeTextBox.IntellisenseBox.SelectedIndex = 0;
            }
            catch { }


            //Get top-left coordinate for our intellisenseBox
            Point topLeft = m_CodeTextBox.GetPositionFromCharIndex(m_CodeTextBox.SelectionStart);
            topLeft.Offset(-35, 18);

            #region Place the intellisense box, to fit the space...
            if (m_CodeTextBox.Size.Height < (topLeft.Y + m_CodeTextBox.IntellisenseBox.Height))
            {
                topLeft.Offset(0, -18 - 18 - m_CodeTextBox.IntellisenseBox.Height);
            }

            if (m_CodeTextBox.Size.Width < (topLeft.X + m_CodeTextBox.IntellisenseBox.Width))
            {
                topLeft.Offset(35 + 15 - m_CodeTextBox.IntellisenseBox.Width, 0);
            }

            if (topLeft.X < 0)
            {
                topLeft.X = 0;
            }

            if (topLeft.Y < 0)
            {
                topLeft.Y = 0;
            }
            #endregion

            m_CodeTextBox.IntellisenseBox.Location = topLeft;
            m_CodeTextBox.IntellisenseBox.Visible = true;
            m_CodeTextBox.Focus();
        }
        /// <summary>
        /// Hides the intellisense box.
        /// </summary>
        public void HideIntellisenseBox()
        {
            m_CodeTextBox.IntellisenseBox.Items.Clear();
            m_CodeTextBox.IntellisenseBox.Visible = false;
        }
        /// <summary>
        /// Navigates up in the intellisense box.
        /// </summary>
        public void NavigateUp(int elements)
        {
            #region Some checkings for the intellisense box
            //Do nothing if the intellisense is not visible...
            if (!m_CodeTextBox.IntellisenseBox.Visible)
            {
                return;
            }
            //If our box has no elements, do not show it...
            if (m_CodeTextBox.IntellisenseBox.Items.Count <= 0)
            {
                return;
            }
            #endregion

            if (m_CodeTextBox.IntellisenseBox.SelectedIndex > elements - 1)
            {
                m_CodeTextBox.IntellisenseBox.SelectedIndex -= elements;
            }
            else
            {
                m_CodeTextBox.IntellisenseBox.SelectedIndex = 0;
            }
        }
        /// <summary>
        /// Navigates down in the intellisense box.
        /// </summary>
        public void NavigateDown(int elements)
        {
            #region Some checkings for the intellisense box
            //Do nothing if the intellisense is not visible...
            if (!m_CodeTextBox.IntellisenseBox.Visible)
            {
                return;
            }
            //If our box has no elements, do not show it...
            if (m_CodeTextBox.IntellisenseBox.Items.Count <= 0)
            {
                return;
            }
            #endregion

            if (m_CodeTextBox.IntellisenseBox.SelectedIndex < m_CodeTextBox.IntellisenseBox.Items.Count - elements - 1)
            {
                m_CodeTextBox.IntellisenseBox.SelectedIndex += elements;
            }
            else 
            {
                m_CodeTextBox.IntellisenseBox.SelectedIndex = m_CodeTextBox.IntellisenseBox.Items.Count - 1;
            }
        }
        /// <summary>
        /// Navigates to the first element in the intellisense box.
        /// </summary>
        public void NavigateHome()
        {
            #region Some checkings for the intellisense box
            //Do nothing if the intellisense is not visible...
            if (!m_CodeTextBox.IntellisenseBox.Visible)
            {
                return;
            }
            //If our box has no elements, do not show it...
            if (m_CodeTextBox.IntellisenseBox.Items.Count <= 0)
            {
                return;
            }
            #endregion

            m_CodeTextBox.IntellisenseBox.SelectedIndex = 0;
        }
        /// <summary>
        /// Navigates to the last element in the intellisense box.
        /// </summary>
        public void NavigateEnd()
        {
            #region Some checkings for the intellisense box
            //Do nothing if the intellisense is not visible...
            if (!m_CodeTextBox.IntellisenseBox.Visible)
            {
                return;
            }
            //If our box has no elements, do not show it...
            if (m_CodeTextBox.IntellisenseBox.Items.Count <= 0)
            {
                return;
            }
            #endregion

            m_CodeTextBox.IntellisenseBox.SelectedIndex = m_CodeTextBox.IntellisenseBox.Items.Count - 1;
        }
        /// <summary>
        /// Calls, when a backspace typed.
        /// </summary>
        public void TypeBackspace()
        {
            #region Some checkings for the intellisense box
            //Do nothing if the intellisense is not visible...
            if (!m_CodeTextBox.IntellisenseBox.Visible)
            {
                return;
            }
            #endregion

            m_LastCharWasAScopeOperator = false;
            UpdateIntellisense(false,"","\b");
        }
        /// <summary>
        /// Calls, when an alphanumerical character typed.
        /// </summary>
        /// <param name="c"></param>
        public void TypeAlphaNumerical(char c)
        {
            CheckScopeOperator(c);
            UpdateIntellisense(false,"",c.ToString());
        }
        /// <summary>
        /// Calls, when a non-alphanumerical character typed.
        /// </summary>
        /// <param name="c"></param>
        public void TypeNonAlphaNumerical(char c)
        {
            CheckScopeOperator(c);

             //Search the last letter(s) for separator, and update if found...
            string lastWord = RichTextboxHelper.GetLastWord(m_CodeTextBox);
            //last word doesn't contains the processed char yet...
            lastWord += c;

            foreach (string separator in m_CodeTextBox.CodeWords_ScopeOperators)
            {
                int index = lastWord.LastIndexOf(separator);
                if ( index > -1)
                {
                    if (index == (lastWord.Length - separator.Length))
                    {
                        //found...
                        UpdateIntellisense(true, lastWord.Substring(0,index),c.ToString());
                        return;
                    }
                }
            }
        }
        /// <summary>
        /// Updates the intellisense box's elements to show the right object list.
        /// </summary>
        /// <param name="forceNextLevel"></param>
        /// <param name="word"></param>
        /// <param name="justRead"></param>
        /// <returns></returns>
        public bool UpdateIntellisense(bool forceNextLevel, string word, string justRead)
        {
            //Clear all elements
            m_CodeTextBox.IntellisenseBox.Items.Clear();

            //Get the actual line
            if (word == "")
            {
                word = RichTextboxHelper.GetLastWord(m_CodeTextBox);
            }

            if (justRead == "\b")
            {
                if (!String.IsNullOrEmpty(word))
                {
                    word = word.Substring(0, word.Length - 1);
                }
            }
            else
            {
                word += justRead;
            }
            

            //Get subwords
            string[] words = word.Split(
                m_CodeTextBox.CodeWords_ScopeOperators.ToArray(), 
                StringSplitOptions.None);

            //if none, return...
            if (words.Length < 1)
            {
                //Hide intellisense...
                return false;
            }

            string lastWord = words[words.Length - 1];

            if (words.Length == 1 && !forceNextLevel)
            {
                #region Search in the root level
                foreach (TreeNode n in m_CodeTextBox.IntellisenseTree.Nodes)
                {
                    if (!Like(n, lastWord))
                    {
                        continue;
                    }

                    ImageListItem li = new ImageListItem();
                    li.Text = n.Name;
                    li.Image = AssociateImage(n);

                    m_CodeTextBox.IntellisenseBox.Items.Add(li);
                }

                //Not show, when no elements available...
                if (m_CodeTextBox.IntellisenseBox.Items.Count > 0)
                {
                    return true;
                }
                return false;
                #endregion
            }


            int i = 1;
            TreeNode node = null;

            //Find the root node
            node = m_CodeTextBox.IntellisenseTree.Nodes[words[0]];

            //Search the tree for the last node - before the one we actually type in
            while (node != null && i < words.Length - 1)
            {
                node = node.Nodes[words[i]];
                i++;
            }

            //Couldn't find one of the sub words...
            if (node == null)
            {
                return false;
            }

            if (forceNextLevel)
            {
                lastWord = "";
            }

            //Add it's nodes to the intellisense list
            foreach (TreeNode n in node.Nodes)
            {
                if (!Like(n, lastWord))
                {
                    continue;
                }

                ImageListItem li = new ImageListItem();
                li.Text = n.Name;
                li.Image = AssociateImage(n);

                m_CodeTextBox.IntellisenseBox.Items.Add(li);
            }

            //Show box
            ShowIntellisenseBoxWithoutUpdate();
            return true;
        }    
        /// <summary>
        /// Confirms the selection from the intellisense, and write the selected text back to the textbox.
        /// </summary>
        public void ConfirmIntellisense()
        {
            string wordSelected;

            try
            {
                wordSelected = ((ImageListItem)m_CodeTextBox.IntellisenseBox.SelectedItem).Text;
            }
            catch 
            {
                return;
            }

            //Get the actual position
            int currentPosition = m_CodeTextBox.SelectionStart;
            //Get the start position of the last word
            int lastWordPosition = GetLastWordStartPosition(m_CodeTextBox, m_CodeTextBox.CodeWords_ScopeOperators);

            //Set selection
            m_CodeTextBox.SelectionStart = lastWordPosition;
            m_CodeTextBox.SelectionLength = currentPosition - lastWordPosition;

            //Change the word
            m_CodeTextBox.SelectedText = wordSelected;

            //Hide the intellisense
            HideIntellisenseBox();
        }
        #endregion

        #region Private methods
        /// <summary>
        /// Searches for the first node in the given node's nodes, which name starts with the text.
        /// If the node is null, it searches the tree.
        /// </summary>
        /// <param name="node"></param>
        /// <param name="text"></param>
        /// <returns></returns>
        private TreeNode SearchLike(TreeNode node, string text)
        {
            if (node == null)
            {
                //search tree
                foreach (TreeNode n in m_CodeTextBox.IntellisenseTree.Nodes)
                {
                    if (Like(n, text))
                    {
                        return n;
                    }
                }
            }
            else
            { 
                //search node
                foreach (TreeNode n in node.Nodes)
                {
                    if (Like(n, text))
                    {
                        return n;
                    }
                }
            }

            return null;
        }
        /// <summary>
        /// Returns true if the tag's name likes the text
        /// </summary>
        /// <param name="node"></param>
        /// <param name="text"></param>
        /// <returns></returns>
        private bool Like(TreeNode node, string text)
        {
            if (text == "")
            {
                return true;
            }

            return node.Name.ToLower().StartsWith(text.ToLower());
        }
        /// <summary>
        /// Associates an image by the Tag value of the node...
        /// </summary>
        /// <param name="node"></param>
        /// <returns></returns>
        private Image AssociateImage(TreeNode node)
        {
            try
            {
                switch (node.Tag.ToString().ToLower())
                {
                    case "class":
                        return m_CodeTextBox.CodeImage_Class;

                    case "event":
                        return m_CodeTextBox.CodeImage_Event;

                    case "interface":
                        return m_CodeTextBox.CodeImage_Interface;

                    case "method":
                        return m_CodeTextBox.CodeImage_Method;

                    case "namespace":
                        return m_CodeTextBox.CodeImage_Namespace;

                    case "property":
                        return m_CodeTextBox.CodeImage_Property;
                }
            }
            catch { }

            return null;
        }
        /// <summary>
        /// Returns the position of the last word. Position searching takes care of scope operators!
        /// </summary>
        /// <param name="richTextbox"></param>
        /// <param name="scopeOperators"></param>
        /// <returns></returns>
        public int GetLastWordStartPosition(RichTextBox richTextbox, List<string> scopeOperators)
        {
            int pos = richTextbox.SelectionStart - 1;
            List<string> lastScopeOperatorChars = GetLastCharsOfScopeOperators();

            //If the last char was a scope separator, we need the current position...
            if (m_LastCharWasAScopeOperator)
            {
                return richTextbox.SelectionStart;
            }
            try
            {
                //If we deleted the last char, and arrive at a scope operator...
                string lastWord = RichTextboxHelper.GetLastWord(m_CodeTextBox);
                string lastChar = lastWord.Substring(lastWord.Length - 1, 1);
                if (lastScopeOperatorChars.Contains(lastChar))
                {
                    return richTextbox.SelectionStart; ;
                }
            }
            catch { }

            while (pos > 1)
            {
                string substr = richTextbox.Text.Substring(pos - 1, 1);

                if (Char.IsWhiteSpace(substr, 0))
                {
                    return pos;
                }
                else if (lastScopeOperatorChars.Contains(substr))
                {
                    return pos;
                }

                pos--;
            }

            return 0;
        }
        /// <summary>
        /// Returns the last chars of scope operators
        /// </summary>
        /// <returns></returns>
        private List<string> GetLastCharsOfScopeOperators()
        {
            List<string> scopeOperatorLastCharList = new List<string>();

            foreach (string s in m_CodeTextBox.CodeWords_ScopeOperators)
            {
                if (String.IsNullOrEmpty(s))
                {
                    continue;
                }
                scopeOperatorLastCharList.Add(s.Substring(s.Length - 1, 1));
            }

            return scopeOperatorLastCharList;
        }
        /// <summary>
        /// Checks, if the actual char is a scope operator, and sets the state of the
        /// m_LastCharWasAScopeOperator variable
        /// </summary>
        /// <param name="c"></param>
        internal void CheckScopeOperator(char c)
        {
            List<string> lastScopeOperatorLetters = GetLastCharsOfScopeOperators();
            m_LastCharWasAScopeOperator = lastScopeOperatorLetters.Contains( c.ToString() );
        }
        #endregion
    }
}
