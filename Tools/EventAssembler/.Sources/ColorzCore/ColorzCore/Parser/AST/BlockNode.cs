using ColorzCore.DataTypes;
using ColorzCore.IO;
using ColorzCore.Lexer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Parser.AST
{
    class BlockNode : ILineNode
    {
        public IList<ILineNode> Children { get; }

        public int Size {
            get
            {
                return Children.Sum((ILineNode n) => n.Size);
            } }

        public BlockNode()
        {
            Children = new List<ILineNode>();
        }

        public string PrettyPrint(int indentation)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(' ', indentation);
            sb.Append('{');
            sb.Append('\n');
            foreach(ILineNode i in Children)
            {
                sb.Append(i.PrettyPrint(indentation + 4));
                sb.Append('\n');
            }
            sb.Append(' ', indentation);
            sb.Append('}');
            return sb.ToString();
        }

        public void WriteData(IOutput output)
        {
            foreach(ILineNode child in Children)
            {
                child.WriteData(output);
            }
        }

        public void EvaluateExpressions(ICollection<Token> undefinedIdentifiers)
        {
            foreach (ILineNode line in Children)
                line.EvaluateExpressions(undefinedIdentifiers);
        }
    }
}
