using ColorzCore.DataTypes;
using ColorzCore.Lexer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Parser.AST
{
    public interface IAtomNode : IParamNode
    {
        //TODO: Simplify() partial evaluation as much as is defined, to save on memory space.
		int Precedence { get; }
        Maybe<string> GetIdentifier();
        IEnumerable<Token> ToTokens();
        Maybe<int> TryEvaluate(TAction<Exception> handler); //Simplifies the AST as much as possible.
    }

    public static class AtomExtensions
    {
        public static int CoerceInt(this IAtomNode n)
        {
            return n.TryEvaluate((Exception e) => { throw e; }).FromJust;
        }
        public static IAtomNode Simplify(this IAtomNode n, TAction<Exception> handler)
        {
            IAtomNode ret = n;
            n.TryEvaluate(handler).IfJust((int i) => { ret = FromInt(n.MyLocation, i); });
            return ret;
        }
        public static IAtomNode FromInt(Location l, int i)
        {
            return new NumberNode(l, i);
        }
    }
}
