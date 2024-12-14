using ColorzCore.IO;
using ColorzCore.Lexer;
using ColorzCore.Raws;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Parser.AST
{
    class RawNode : StatementNode
    {
        private Raw myRaw;
        private Token myToken;
        private int offset;

        public RawNode(Raw raw, Token t, int offset, IList<IParamNode> paramList) : base(paramList)
        {
            myToken = t;
            myRaw = raw;
            this.offset = offset;
        }

        public override int Size => myRaw.LengthBytes(Parameters.Count);

        public override string PrettyPrint(int indentation)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(' ', indentation);
            sb.Append(myToken.Content);
            foreach (IParamNode n in Parameters)
            {
                sb.Append(' ');
                sb.Append(n.PrettyPrint());
            }
            return sb.ToString();
        }
        public override void WriteData(IOutput output)
        {
            output.WriteTo(offset, myRaw.GetBytes(Parameters));
        }
    }
}
