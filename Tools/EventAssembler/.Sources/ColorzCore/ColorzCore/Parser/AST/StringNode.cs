using ColorzCore.DataTypes;
using ColorzCore.Lexer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace ColorzCore.Parser.AST
{
    public class StringNode : IParamNode
    {
        public static readonly Regex idRegex = new Regex("^([a-zA-Z_][a-zA-Z0-9_]*)$");
        public Token MyToken { get; }

        public Location MyLocation { get { return MyToken.Location; } }
        public ParamType Type { get { return ParamType.STRING; } }

        public StringNode(Token value)
        {
            MyToken = value;
        }

        public IEnumerable<byte> ToBytes()
        {
            return Encoding.ASCII.GetBytes(ToString());
        }

        public override string ToString()
        {
            return MyToken.Content;
        }
        public string PrettyPrint()
        {
            return '"' + ToString() + '"';
        }
        public IEnumerable<Token> ToTokens() { yield return MyToken; }

        public Either<int, string> TryEvaluate()
        {
            return new Right<int, string>("Expected atomic parameter.");
        }

        public bool IsValidIdentifier()
        {
            return idRegex.IsMatch(MyToken.Content);
        }
        public IdentifierNode ToIdentifier(ImmutableStack<Closure> scope)
        {
            return new IdentifierNode(MyToken, scope);
        }

        public Maybe<IAtomNode> AsAtom() { return new Nothing<IAtomNode>(); }

        public IParamNode SimplifyExpressions(TAction<Exception> handler) { return this; }
    }
}
