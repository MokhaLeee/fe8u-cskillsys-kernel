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
    class UndefineDirective : IDirective
    {
        public int MinParams => 1;

        public int? MaxParams => null;

        public bool RequireInclusion => true;

        public Maybe<ILineNode> Execute(EAParser p, Token self, IList<IParamNode> parameters, MergeableGenerator<Token> tokens)
        {
            foreach (IParamNode parm in parameters)
            {
                string s = parm.ToString()!;
                if (p.Definitions.ContainsKey(s))
                    p.Definitions.Remove(s);
                else
                    p.Warning(parm.MyLocation, "Undefining non-existant definition: " + s);
            }
            return new Nothing<ILineNode>();
        }
    }
}
