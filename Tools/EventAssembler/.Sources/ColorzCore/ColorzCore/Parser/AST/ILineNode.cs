using ColorzCore.IO;
using ColorzCore.Lexer;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Parser.AST
{
    interface ILineNode
    {
        int Size { get; }
        string PrettyPrint(int indentation);
        void WriteData(IOutput output);
        void EvaluateExpressions(ICollection<Token> undefinedIdentifiers); //Return: undefined identifiers
    }
}
