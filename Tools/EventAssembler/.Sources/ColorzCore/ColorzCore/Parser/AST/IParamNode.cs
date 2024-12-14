using ColorzCore.DataTypes;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ColorzCore.Lexer;

namespace ColorzCore.Parser.AST
{
    public interface IParamNode
    {
        string? ToString(); //For use in other programs.
        ParamType Type { get; }
        string PrettyPrint();
        Location MyLocation { get; }
        IParamNode SimplifyExpressions(TAction<Exception> handler); //TODO: Abstract this into a general traverse method.
        Maybe<IAtomNode> AsAtom();
    }

    public static class ParamExtensions
    {
        public static IParamNode Simplify(this IParamNode n)
        {
            return n.SimplifyExpressions((Exception e) => { });
        }
    }
}
