using ColorzCore.DataTypes;
using ColorzCore.IO;
using ColorzCore.Lexer;
using ColorzCore.Parser;
using ColorzCore.Parser.AST;
using ColorzCore.Preprocessor.Directives;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Preprocessor
{
    class DirectiveHandler
    {
        private Dictionary<string, IDirective> directives;

        public DirectiveHandler(IncludeFileSearcher includeSearcher, IncludeFileSearcher toolSearcher)
        {
            directives = new Dictionary<string, IDirective>
            {
                { "include", new IncludeDirective { FileSearcher = includeSearcher } },
                { "incbin", new IncludeBinaryDirective { FileSearcher = includeSearcher } },
                { "incext", new IncludeExternalDirective { FileSearcher = toolSearcher } },
                { "inctext", new IncludeToolEventDirective { FileSearcher = toolSearcher } },
                { "inctevent", new IncludeToolEventDirective { FileSearcher = toolSearcher } },
                { "ifdef", new IfDefinedDirective() },
                { "ifndef", new IfNotDefinedDirective() },
                { "else", new ElseDirective() },
                { "endif", new EndIfDirective() },
                { "define", new DefineDirective() },
                { "pool", new PoolDirective() },
                { "undef", new UndefineDirective() },
            };
        }

        public Maybe<ILineNode> HandleDirective(EAParser p, Token directive, IList<IParamNode> parameters, MergeableGenerator<Token> tokens)
        {
            string directiveName = directive.Content.Substring(1);

            if (directives.TryGetValue(directiveName, out IDirective? toExec))
            {
                if (!toExec.RequireInclusion || p.IsIncluding)
                {
                    if (toExec.MinParams <= parameters.Count && (!toExec.MaxParams.HasValue || parameters.Count <= toExec.MaxParams))
                    {
                        return toExec.Execute(p, directive, parameters, tokens);
                    }
                    else
                    {
                        p.Error(directive.Location, "Invalid number of parameters (" + parameters.Count + ") to directive " + directiveName + ".");
                    }
                }
            }
            else
            {
                p.Error(directive.Location, "Directive not recognized: " + directiveName);
            }

            return new Nothing<ILineNode>();
        }
    }
}
