using ColorzCore.DataTypes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Lexer
{
    public class Token
    {
        public Location Location { get; }

        public TokenType Type { get; }
        public string FileName { get { return Location.file; } }
        public int LineNumber { get { return Location.lineNum; } }
        public int ColumnNumber { get { return Location.colNum; } }
        public string Content { get; }

        public Token(TokenType type, string fileName, int lineNum, int colNum, string original = "")
        {
            Type = type;
            Location = new Location(fileName, lineNum, colNum+1);
            Content = original;
        }

        public Token(TokenType type, Location newLoc, string content)
        {
            Type = type;
            this.Location = newLoc;
            Content = content;
        }

        public override string ToString()
        {
            return String.Format("File {4}, Line {0}, Column {1}, {2}: {3}", LineNumber, ColumnNumber, Type, Content, FileName);
        }
    }
}
