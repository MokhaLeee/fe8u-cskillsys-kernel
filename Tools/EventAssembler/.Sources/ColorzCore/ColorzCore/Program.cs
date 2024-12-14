using System;
using System.IO;
using System.Collections.Generic;
using ColorzCore.IO;
using ColorzCore.DataTypes;

namespace ColorzCore
{
    class Program
    {
        public static bool Debug = false;

        private static string[] helpstringarr = {
            "EA Colorz Core. Usage:",
            "./ColorzCore <A|D|AA> <game> [-opts]",
            "",
            "A is to write ROM directly",
            "AA is to output assembly source file and linker script",
            "D is not allowed currently.",
            "Game may be any string; the respective _game_ variable gets defined in scripts.",
            "",
            "Available options:",
            "-raws:<dir>",
            "   Sets the raws directory to the one provided (relative to ColorzCore). Defaults to \"Language Raws\".",
            "-rawsExt:<ext>",
            "   Sets the extension of files used for raws to the one provided. Defaults to .txt.",
            "-output:<filename>",
            "   Set the file to write assembly to.",
            "-input:<filename>",
            "   Set the file to take input script from. Defaults to stdin.",
            "-error:<filename>",
            "   Set a file to redirect messages, warnings, and errors to. Defaults to stderr.",
            "--nocash-sym",
            "   Outputs a no$ compatible .sym file corresponding to the output file.",
            "-I:<path>|--inlude:<path>",
            "   Add given path to list of paths to search for included files in.",
            "-T:<path>|--tools:<path>",
            "   Add given path to list of paths to search for tools in.",
            "-IT:<path>|-TI:<path>",
            "   Combines --include:<path> and --tools:<path>.",
            "-werr",
            "   Treat all warnings as errors and prevent assembly.",
            "--no-mess",
            "   Suppress output of messages.",
            "--no-warn",
            "   Suppress output of warnings.",
            "-quiet",
            "   Equivalent to --no-mess --no-warn.",
            "--no-colored-log",
            "   Don't use colored log tags when outputting logs to console/stderr.",
            "-[D|def|define]:<defname>=<defvalue>",
            "   Assembles as if \"#define <defname> <defvalue>\" were at the top of the input stream.",
            "-debug",
            "   Enable debug mode. Not recommended for end users.",
            "--build-times",
            "   Print build times at the end of build.",
            "-romoffset:<number>",
            "   Treats the offset of the ROM as the given number,",
            "   for the purposes of POIN. Addresses are or'd.",
            "   Hex literals only. Defaults to 0x08000000.",
            "-h|--help",
            "   Display this message and exit.",
            ""
        };

        private static readonly string helpstring = System.Linq.Enumerable.Aggregate(helpstringarr,
            (string a, string b) => { return a + '\n' + b; });

        private const int EXIT_SUCCESS = 0;
        private const int EXIT_FAILURE = 1;

        static int Main(string[] args)
        {
            IncludeFileSearcher rawSearcher = new IncludeFileSearcher();
            rawSearcher.IncludeDirectories.Add(AppDomain.CurrentDomain.BaseDirectory);

            Stream inStream = Console.OpenStandardInput();
            string inFileName = "stdin";

            IOutput? output = null;
            string outFileName = "none";
            string ldsFileName = "none";

            TextWriter errorStream = Console.Error;

            Maybe<string> rawsFolder = rawSearcher.FindDirectory("Language Raws");
            string rawsExtension = ".txt";

            if (args.Length < 2)
            {
                Console.WriteLine("Required parameters missing.");
                return EXIT_FAILURE;
            }

            bool outputASM = false;
            if (args[0] == "AA")
                outputASM = true;
            else
                if (args[0] != "A")
            {
                Console.WriteLine("Only assembly is supported currently. Please run as \"./ColorzCore.exe A ...\".");
                return EXIT_FAILURE;
            }

            for (int i = 2; i < args.Length; i++)
            {
                if (args[i][0] != '-')
                {
                    Console.Error.WriteLine("Unrecognized paramter: " + args[i]);
                }
                else
                {
                    string[] flag = args[i].Substring(1).Split(new char[] { ':' }, 2);

                    try
                    {
                        switch (flag[0])
                        {
                            case "raws":
                                rawsFolder = rawSearcher.FindDirectory(flag[1]);
                                break;

                            case "rawsExt":
                                rawsExtension = flag[1];
                                break;

                            case "output":
                                outFileName = flag[1];
                                if(outputASM)
                                {
                                    ldsFileName = Path.ChangeExtension(outFileName, "lds");
                                    output = new ASM(new StreamWriter(outFileName, false),
                                                     new StreamWriter(ldsFileName, false));
                                } else
                                {
                                    FileStream outStream;
                                    if(File.Exists(outFileName) && !File.GetAttributes(outFileName).HasFlag(FileAttributes.ReadOnly))
                                    {
                                        outStream = File.Open(outFileName, FileMode.Open, FileAccess.ReadWrite);
                                    } else if(!File.Exists(outFileName))
                                    {
                                        outStream = File.Create(outFileName);
                                    } else
                                    {
                                        Console.Error.WriteLine("Output file is read-only.");
                                        return EXIT_FAILURE;
                                    }
                                    output = new ROM(outStream);
                                } 
                                break;

                            case "input":
                                inFileName = flag[1];
                                inStream = File.OpenRead(flag[1]);
                                break;

                            case "error":
                                errorStream = new StreamWriter(File.OpenWrite(flag[1]));
                                EAOptions.Instance.noColoredLog = true;
                                break;

                            case "debug":
                                Debug = true;
                                break;

                            case "werr":
                                EAOptions.Instance.werr = true;
                                break;

                            case "-no-mess":
                                EAOptions.Instance.nomess = true;
                                break;

                            case "-no-warn":
                                EAOptions.Instance.nowarn = true;
                                break;

                            case "-no-colored-log":
                                EAOptions.Instance.noColoredLog = true;
                                break;

                            case "quiet":
                                EAOptions.Instance.nomess = true;
                                EAOptions.Instance.nowarn = true;
                                break;

                            case "-nocash-sym":
                                EAOptions.Instance.nocashSym = true;
                                break;

                            case "-build-times":
                                EAOptions.Instance.buildTimes = true;
                                break;

                            case "I":
                            case "-include":
                                EAOptions.Instance.includePaths.Add(flag[1]);
                                break;

                            case "T":
                            case "-tools":
                                EAOptions.Instance.toolsPaths.Add(flag[1]);
                                break;

                            case "IT":
                            case "TI":
                                EAOptions.Instance.includePaths.Add(flag[1]);
                                EAOptions.Instance.toolsPaths.Add(flag[1]);
                                break;

                            case "h":
                            case "-help":
                                Console.Out.WriteLine(helpstring);
                                return EXIT_SUCCESS;

                            case "D":
                            case "def":
                            case "define":
                                try {
                                    string[] def_args = flag[1].Split(new char[] { '=' }, 2);
                                    EAOptions.Instance.defs.Add(Tuple.Create(def_args[0], def_args[1]));
                                } catch (IndexOutOfRangeException)
                                {
                                    Console.Error.WriteLine("Improperly formed -define directive.");
                                }
                                break;

                            case "romoffset":
                                try
                                {
                                    EAOptions.Instance.romOffset = Convert.ToInt32(flag[1], 16);
                                } catch {
                                    Console.Error.WriteLine("Invalid hex offset given for ROM.");
                                }
                                break;

                            default:
                                Console.Error.WriteLine("Unrecognized flag: " + flag[0]);
                                return EXIT_FAILURE;
                        }
                    }
                    catch (IOException e)
                    {
                        Console.Error.WriteLine("Exception: " + e.Message);
                        return EXIT_FAILURE;
                    }
                }
            }
            
            if (output == null)
            {
                Console.Error.WriteLine("No output specified for assembly.");
                return EXIT_FAILURE;
            }

            if (rawsFolder.IsNothing)
            {
                Console.Error.WriteLine("Couldn't find raws folder");
                return EXIT_FAILURE;
            }

            string game = args[1];

            //FirstPass(Tokenizer.Tokenize(inputStream));

            Log log = new Log {
                Output = errorStream,
                WarningsAreErrors = EAOptions.Instance.werr,
                NoColoredTags = EAOptions.Instance.noColoredLog
            };

            if (EAOptions.Instance.nowarn)
                log.IgnoredKinds.Add(Log.MsgKind.WARNING);

            if (EAOptions.Instance.nomess)
                log.IgnoredKinds.Add(Log.MsgKind.MESSAGE);

            EAInterpreter myInterpreter = new EAInterpreter(output, game, rawsFolder.FromJust, rawsExtension, inStream, inFileName, log);

            ExecTimer.Timer.AddTimingPoint(ExecTimer.KEY_RAWPROC);

            bool success = myInterpreter.Interpret();

            if (success && EAOptions.Instance.nocashSym)
            {
                using (var symOut = File.CreateText(Path.ChangeExtension(outFileName, "sym")))
                {
                    if (!(success = myInterpreter.WriteNocashSymbols(symOut)))
                    {
                        log.Message(Log.MsgKind.ERROR, "Error trying to write no$gba symbol file.");
                    }
                }
            }

            if (EAOptions.Instance.buildTimes) { 

            // Print times

            log.Output.WriteLine();
            log.Output.WriteLine("Times:");

            foreach (KeyValuePair<TimeSpan, string> time in ExecTimer.Timer.SortedTimes)
            {
                log.Output.WriteLine("  " + time.Value + ": " + time.Key.ToString() + " (" + ExecTimer.Timer.Counts[time.Value] + ")");
            }

            // Print total time

            log.Output.WriteLine();
            log.Output.WriteLine("Total:");

            log.Output.WriteLine("  " + ExecTimer.Timer.TotalTime.ToString());

            }

            inStream.Close();
            output.Close();
            errorStream.Close();

            return success ? EXIT_SUCCESS : EXIT_FAILURE;

        }
    }
}

