using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ColorzCore.DataTypes;
using ColorzCore.Lexer;
using ColorzCore.Parser;
using ColorzCore.Parser.AST;
using ColorzCore.IO;
using System.IO;

namespace ColorzCore.Preprocessor.Directives
{
    class IncludeToolEventDirective : IDirective
    {
        public int MinParams { get { return 1; } }
        public int? MaxParams { get { return null; } }
        public bool RequireInclusion { get { return true; } }

        public IncludeFileSearcher FileSearcher { get; set; } = new IncludeFileSearcher();

        public Maybe<ILineNode> Execute(EAParser parse, Token self, IList<IParamNode> parameters, MergeableGenerator<Token> tokens)
        {
            ExecTimer.Timer.AddTimingPoint(ExecTimer.KEY_GENERIC);

            Maybe<string> validFile = FileSearcher.FindFile(Path.GetDirectoryName(self.FileName), IOUtility.GetToolFileName(parameters[0].ToString()!));

            if (validFile.IsNothing)
            {
                parse.Error(parameters[0].MyLocation, "Tool " + parameters[0].ToString() + " not found.");
                return new Nothing<ILineNode>();
            }

            //from http://stackoverflow.com/a/206347/1644720
            // Start the child process.
            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo.RedirectStandardError = true;
            // Redirect the output stream of the child process.
            p.StartInfo.WorkingDirectory = Path.GetDirectoryName(self.FileName)!;
            p.StartInfo.UseShellExecute = false;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.FileName = validFile.FromJust;
            StringBuilder argumentBuilder = new StringBuilder();
            for (int i = 1; i < parameters.Count; i++)
            {
                parameters[i].AsAtom().IfJust((IAtomNode n) => { parameters[i] = n.Simplify(); });
                argumentBuilder.Append(parameters[i].PrettyPrint());
                argumentBuilder.Append(' ');
            }
            argumentBuilder.Append("--to-stdout");
            p.StartInfo.Arguments = argumentBuilder.ToString();
            p.Start();
            // Do not wait for the child process to exit before
            // reading to the end of its redirected stream.
            // p.WaitForExit();
            // Read the output stream first and then wait.
            MemoryStream outputBytes = new MemoryStream();
            MemoryStream errorStream = new MemoryStream();
            p.StandardOutput.BaseStream.CopyTo(outputBytes);
            p.StandardError.BaseStream.CopyTo(errorStream);
            p.WaitForExit();

            byte[] output = outputBytes.GetBuffer().Take((int)outputBytes.Length).ToArray();
            if(errorStream.Length > 0)
            {
                parse.Error(self.Location, Encoding.ASCII.GetString(errorStream.GetBuffer().Take((int)errorStream.Length).ToArray()));
            }
            else if (output.Length >= 7 && Encoding.ASCII.GetString(output.Take(7).ToArray()) == "ERROR: ")
            {
                parse.Error(self.Location, Encoding.ASCII.GetString(output.Skip(7).ToArray()));
            }
            else
            {
                outputBytes.Position = 0; //Reset MemoryStream position so StreamReader starts reading from beginning.
                Tokenizer t = new Tokenizer();
                tokens.PrependEnumerator(t.Tokenize(outputBytes, Path.GetFileName(self.FileName) + ", Line " + self.LineNumber + "; " + parameters[0].ToString()).GetEnumerator());
            }

            ExecTimer.Timer.AddTimingPoint(parameters[0].ToString()!.ToLower());

            return new Nothing<ILineNode>();
        }
    }
}
