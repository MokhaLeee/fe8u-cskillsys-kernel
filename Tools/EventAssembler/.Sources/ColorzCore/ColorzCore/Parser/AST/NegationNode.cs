using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ColorzCore.DataTypes;
using ColorzCore.Lexer;

namespace ColorzCore.Parser.AST
{
    class NegationNode : AtomNodeKernel
    {
        private Token myToken;
        private IAtomNode interior;

        public NegationNode(Token token, IAtomNode inside)
        {
            myToken = token;
            interior = inside;
        }

        public override int Precedence => 11;
        public override Location MyLocation => myToken.Location;

        public override string PrettyPrint()
        {
            return '-' + interior.PrettyPrint();
        }

        public override IEnumerable<Token> ToTokens()
        {
            yield return myToken;
            foreach(Token t in interior.ToTokens())
                yield return t;
        }

        public override Maybe<int> TryEvaluate(TAction<Exception> handler)
        {
            return interior.TryEvaluate(handler).Fmap((int x) => -x);
        }
    }
}
