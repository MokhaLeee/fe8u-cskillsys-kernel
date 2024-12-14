using ColorzCore.DataTypes;
using ColorzCore.Lexer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Parser
{
    public class Definition
    {
        private IList<Token> replacement;

        public Definition()
        {
            replacement = new List<Token>();
        }

        public Definition(IList<Token> defn)
        {
            replacement = defn;
        }

        public IEnumerable<Token> ApplyDefinition(Token toReplace)
        {
            for(int i=0; i<replacement.Count; i++)
            {
                Location newLoc = new Location(toReplace.FileName, toReplace.LineNumber, toReplace.ColumnNumber);
                yield return new Token(replacement[i].Type, newLoc, replacement[i].Content);
            }
        }
    }
}
