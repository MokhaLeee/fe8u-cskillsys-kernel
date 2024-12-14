using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ColorzCore.DataTypes;
using ColorzCore.Lexer;
using ColorzCore.Parser;
using ColorzCore.Parser.AST;

namespace ColorzCore.Preprocessor.Directives
{
    class IfNotDefinedDirective : IDirective
    {
        public int MinParams => 1;

        public int? MaxParams => 1;

        public bool RequireInclusion => false;

        public Maybe<ILineNode> Execute(EAParser p, Token self, IList<IParamNode> parameters, MergeableGenerator<Token> tokens)
        {
            bool flag = true;
            Maybe<string> identifier;
            foreach (IParamNode parameter in parameters)
            {
                if(parameter.Type==ParamType.ATOM && !(identifier = ((IAtomNode)parameter).GetIdentifier()).IsNothing)
                {
                    flag &= !p.Macros.ContainsName(identifier.FromJust) && !p.Definitions.ContainsKey(identifier.FromJust); //TODO: Built in definitions?
                }
                else
                {
                    p.Error(parameter.MyLocation, "Definition name must be an identifier.");
                }
            }
            p.Inclusion = new ImmutableStack<bool>(flag, p.Inclusion);
            return new Nothing<ILineNode>();
        }
    }
}
