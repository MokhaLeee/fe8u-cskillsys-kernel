using ColorzCore.DataTypes;
using ColorzCore.Lexer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Parser.AST
{
    public class ListNode : IParamNode
    {
        public Location MyLocation { get; }
        public IList<IAtomNode> Interior { get; }

        public ParamType Type { get { return ParamType.LIST; } }

        public ListNode(Location startLocation, IList<IAtomNode> param)
        {
            MyLocation = startLocation;
            Interior = param;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append('[');
            for(int i=0; i<Interior.Count; i++)
            {
                sb.Append(Interior[i].CoerceInt());
                if(i < Interior.Count - 1)
                    sb.Append(',');
            }
            sb.Append(']');
            return sb.ToString();
        }

        public string PrettyPrint()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append('[');
            for (int i = 0; i < Interior.Count; i++)
            {
                sb.Append(Interior[i].PrettyPrint());
                if (i < Interior.Count - 1)
                    sb.Append(',');
            }
            sb.Append(']');
            return sb.ToString();
        }
        public IEnumerable<Token> ToTokens()
        {
            //Similar code to ParenthesizedAtom
            IList<IList<Token>> temp = new List<IList<Token>>();
            foreach(IAtomNode n in Interior)
            {
                temp.Add(new List<Token>(n.ToTokens()));
            }
            Location myStart = MyLocation;
            Location myEnd = temp.Count > 0 ? temp.Last().Last().Location : MyLocation;
            yield return new Token(TokenType.OPEN_BRACKET, new Location(myStart.file, myStart.lineNum, myStart.colNum - 1), "[");
            for (int i = 0; i < temp.Count; i++)
            {
                foreach (Token t in temp[i])
                {
                    yield return t;
                }
                if (i < temp.Count - 1)
                {
                    Location tempEnd = temp[i].Last().Location;
                    yield return new Token(TokenType.COMMA, new Location(tempEnd.file, tempEnd.lineNum, tempEnd.colNum + temp[i].Last().Content.Length), ",");
                }
            }
            yield return new Token(TokenType.CLOSE_BRACKET, new Location(myEnd.file, myEnd.lineNum, myEnd.colNum + (temp.Count > 0 ? temp.Last().Last().Content.Length : 1)), "]");
        }

        public Either<int, string> TryEvaluate()
        {
            return new Right<int, string>("Expected atomic parameter.");
        }

        public IParamNode SimplifyExpressions(TAction<Exception> handler)
        {
            IEnumerable<Token> acc = new List<Token>();
            for(int i=0; i<Interior.Count; i++)
            {
                Interior[i] = Interior[i].Simplify(handler);
            }
            return this;
        }

        public int NumCoords { get { return Interior.Count; } }

        public Maybe<IAtomNode> AsAtom() { return new Nothing<IAtomNode>(); }
    }
}
