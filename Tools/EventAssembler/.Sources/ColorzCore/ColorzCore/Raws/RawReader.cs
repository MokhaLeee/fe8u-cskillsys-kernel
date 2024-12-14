using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using ColorzCore.DataTypes;

namespace ColorzCore.Raws
{
    static class RawReader
    {
        /***
         * Flags: 
         priority
          Affects where disassembly uses the code. Existing priorities are:
          main, low, pointer, unit, moveManual, shopList, ballista, ASM,
          battleData, reinforcementData and unknown.

         repeatable
          Means that the last parameter can be repeated and for every 
          repetition a new code is made. Currently requires code to have
          only one parameter.

         unsafe
          EA normally checks for things like parameter collisions and
          other index errors. With this flag, you can bypass them.
          Do not use unless you know what you are doing.

         end
          Means that the code ends disassembly of particular branch in 
          chapter-wide disassembly or in disassembly to end. 

         indexMode
          Affect how many bits lengths and positions mean. 8 means lengths 
          and positions are in bytes. Default is 1.

         terminatingList
          Means that the code is a variable length array of parameters which
          ends in specified value. Requires for code to have only one parameter.

         offsetMod
          The modulus in which the beginning offset of the code has to be 0.
          Default is 4. 

         noAssembly
          Forbids code from participating in assembly.

         noDisassembly
          Forbids code from participating in disassembly.
        */

        public static readonly char CHAR_COMMENT = '#';

        // Raw flags
        public static readonly string FLAG_BITUNIT = "bitUnit";
        public static readonly string FLAG_GAME = "game";
        public static readonly string FLAG_ALIGNMENT = "alignment";
        public static readonly string FLAG_LIST_TERMINATOR = "listTerminator";
        public static readonly string FLAG_REPEATABLE = "repeatable";
        public static readonly string FLAG_UNSAFE = "unsafe";

        // Param flags
        public static readonly string FLAG_POINTER = "pointer";
        public static readonly string FLAG_FIXED = "fixed";
        public static readonly string FLAG_COORDCOUNT = "coordinates";

        // Flag aliases
        public static readonly Dictionary<string, string> FLAG_ALIAS_MAP = new Dictionary<string, string>
        {
            { "language", FLAG_GAME },
            { "indexMode", FLAG_BITUNIT },
            { "offsetMod", FLAG_ALIGNMENT },
            { "terminatingList", FLAG_LIST_TERMINATOR },

            { "coordinate", FLAG_COORDCOUNT },
        };

        // Other notable constants
        public static readonly string GAME_ANY = "any";

        public static IEnumerable<Raw> ParseAllRaws(FileStream fs)
        {
            using (FileLineReader reader = new FileLineReader(fs))
            {
                while (!reader.EndOfStream)
                {
                    Raw? raw = null;

                    try
                    {
                        raw = ParseRaw(reader);
                    }
                    catch (EndOfStreamException)
                    {
                        break;
                    }

                    yield return raw;
                }
            }
        }

        private static Raw ParseRaw(FileLineReader source)
        {
            // Since the writer of the raws is expected to know what they're doing, I'm going to be a lot more lax with error messages and graceful failure.

            string? rawLine;

            do
            {
                rawLine = source.ReadLine();

                if (rawLine == null)
                    break;

                if (rawLine.Trim().Length == 0 || rawLine[0] == CHAR_COMMENT)
                    continue;

                break;
            }
            while (rawLine != null);

            if (rawLine == null)
                throw new EndOfStreamException();

            int lineNumber = source.LineNumber;

            if (char.IsWhiteSpace(rawLine[0]))
            {
                throw new RawParseException("Raw not at start of line.", source.FileName, lineNumber);
            }

            var parts = rawLine.Split(new char[1] { ',' }, StringSplitOptions.RemoveEmptyEntries);

            if (parts.Length < 3)
            {
                // Raws need to have at least name+code+size
                throw new RawParseException("Missing info on raw (need at least name, code and size).", source.FileName, lineNumber);
            }

            var nameStr = parts[0].Trim().ToUpperInvariant();
            var codeStr = parts[1].Trim();
            var sizeStr = parts[2].Trim();

            var flagsStr = parts.Length >= 4
                ? parts[3].Trim()
                : string.Empty;

            int code = codeStr.ToInt();

            Dictionary<string, Flag> flags;

            try
            {
                flags = ParseFlags(flagsStr);
            }
            catch (Exception e)
            {
                throw new RawParseException(e.Message, source.FileName, lineNumber);
            }

            int bitUnit = flags.ContainsKey(FLAG_BITUNIT)
                ? flags[FLAG_BITUNIT].Values.GetLeft[0].ToInt()
                : 1;

            int size = sizeStr.ToInt() * bitUnit;

            var parameters = new List<IRawParam>();
            var fixedParams = new List<Raw.FixedParam>();

            while (!source.EndOfStream)
            {
                int next = source.PeekChar();

                if (next == CHAR_COMMENT)
                {
                    source.ReadLine();
                    continue;
                }

                if (!char.IsWhiteSpace((char)next))
                    break;

                string? line = source.ReadLine();

                if (string.IsNullOrEmpty(line) || line.Trim().Length == 0)
                    continue;

                try
                {
                    Either<IRawParam, Raw.FixedParam> param = ParseParam(line, bitUnit);

                    param.Case(
                        (IRawParam val) => parameters.Add(val),
                        (Raw.FixedParam val) => fixedParams.Add(val));
                }
                catch (Exception e)
                {
                    throw new RawParseException(e.Message, source.FileName, source.LineNumber);
                }
            }

            if (!flags.ContainsKey(FLAG_UNSAFE))
            {
                // TODO: Check for parameter offset collisions
            }

            HashSet<string> game = flags.ContainsKey(FLAG_GAME)
                ? new HashSet<string>(flags[FLAG_GAME].Values.GetLeft)
                : new HashSet<string>();

            if (game.Contains(GAME_ANY))
                game.Clear(); // Empty game list means all games supported

            int alignment = flags.ContainsKey(FLAG_ALIGNMENT)
                ? flags[FLAG_ALIGNMENT].Values.GetLeft[0].ToInt()
                : 4;

            Maybe<int> listTerminator = flags.ContainsKey(FLAG_LIST_TERMINATOR)
                ? (Maybe<int>)new Just<int>(flags[FLAG_LIST_TERMINATOR].Values.GetLeft[0].ToInt())
                : new Nothing<int>();

            if (!listTerminator.IsNothing && code != 0)
            {
                throw new RawParseException("TerminatingList with code nonzero.", source.FileName, lineNumber);
            }

            bool isRepeatable = flags.ContainsKey(FLAG_REPEATABLE);

            if ((isRepeatable || !listTerminator.IsNothing) && (parameters.Count > 1) && fixedParams.Count > 0)
            {
                throw new RawParseException("Repeatable or terminatingList code with multiple parameters or fixed parameters.", source.FileName, lineNumber);
            }

            // HACK: support terminating lists that have a code size of 0 (ugh)

            if (!listTerminator.IsNothing)
            {
                foreach (var param in parameters)
                    size = Math.Max(size, param.Position + param.Length);
            }

            // Check for integrity

            foreach (var param in parameters)
            {
                if (param.Position < 0 || param.Position + param.Length > size)
                    throw new RawParseException(string.Format("parameter {0} doesn't fit the bounds of {1}", param.Name, nameStr), source.FileName, lineNumber);
            }

            foreach (var fp in fixedParams)
            {
                if (fp.position < 0 || fp.position + fp.size > size)
                    throw new RawParseException(string.Format("fixed parameter at +{0} doesn't fit the bounds of {1}", fp.position / bitUnit, nameStr), source.FileName, lineNumber);
            }

            return new Raw(nameStr, size, (short)code, alignment, game, parameters, fixedParams, listTerminator, isRepeatable);
        }

        private static Either<IRawParam, Raw.FixedParam> ParseParam(string line, int bitUnit)
        {
            if (!char.IsWhiteSpace(line[0]))
                throw new Exception("Raw param does not start with whitespace.");

            var parts = line.Trim().Split(new char[1] { ',' }, StringSplitOptions.RemoveEmptyEntries);

            if (parts.Length < 3)
                throw new Exception("Missing info on raw param (need at least name, position and size).");

            var nameStr = parts[0];
            var positionStr = parts[1];
            var sizeStr = parts[2];

            var flagsStr = parts.Length >= 4
                ? parts[3].Trim()
                : string.Empty;

            // NOTE: throws Exceptions
            var flags = ParseFlags(flagsStr);

            int position = positionStr.ToInt() * bitUnit;
            int size = sizeStr.ToInt() * bitUnit;

            if (flags.ContainsKey(FLAG_FIXED))
            {
                int fixedValue = nameStr.ToInt();

                return new Right<IRawParam, Raw.FixedParam>(new Raw.FixedParam
                {
                    position = position,
                    size = size,
                    value = fixedValue,
                });
            }

            if (flags.ContainsKey(FLAG_COORDCOUNT))
            {
                Either<IList<string>, Tuple<int, int>> coordNum = flags[FLAG_COORDCOUNT].Values;

                int nCoords = coordNum.IsLeft
                    ? coordNum.GetLeft.Max((string s) => s.ToInt())
                    : Math.Max(coordNum.GetRight.Item1, coordNum.GetRight.Item2);

                return new Left<IRawParam, Raw.FixedParam>(new ListParam(nameStr, position, size, nCoords));
            }

            bool isPointer = flags.ContainsKey(FLAG_POINTER);

            return new Left<IRawParam, Raw.FixedParam>(new AtomicParam(nameStr, position, size, isPointer));
        }

        private static Dictionary<string, Flag> ParseFlags(string flagStr)
        {
            var result = new Dictionary<string, Flag>();

            if (flagStr.Length > 0)
            {
                var parts = flagStr.Split(new char[1] { ' ' }, StringSplitOptions.RemoveEmptyEntries);

                foreach (string flag in parts)
                {
                    if (flag[0] != '-')
                        throw new Exception("Flag does not start with '-'");

                    var withoutDash = flag.Substring(1);

                    var name = string.Empty;
                    var value = new Flag();

                    if (withoutDash.Contains(':'))
                    {
                        var split = withoutDash.Split(new char[1] { ':' }, StringSplitOptions.RemoveEmptyEntries);

                        name = split[0];

                        if (split.Length == 2 && split[1].Contains('-'))
                        {
                            var rangeStr = split[1].Split(new char[1] { '-' }, StringSplitOptions.RemoveEmptyEntries);

                            value = new Flag(rangeStr[0].ToInt(), rangeStr[1].ToInt());
                        }
                        else
                        {
                            value = new Flag(new List<string>(split.Skip(1)));
                        }
                    }
                    else
                    {
                        name = withoutDash;
                    }

                    if (FLAG_ALIAS_MAP.TryGetValue(name, out string? realName))
                    {
                        name = realName;
                    }

                    result[name] = value;
                }
            }

            return result;
        }

        private class FileLineReader : IDisposable
        {
            // Helper wrapper class for reading lines from file while counting them
            // That way we can print line number in error messages

            private StreamReader reader;

            public string FileName { get; }
            public int LineNumber { get; private set; }

            public bool EndOfStream => reader.EndOfStream;

            public FileLineReader(FileStream input)
            {
                reader = new StreamReader(input);

                FileName = input.Name;
                LineNumber = 0;
            }

            public string? ReadLine()
            {
                string? line = reader.ReadLine();

                if (line != null)
                    LineNumber = LineNumber + 1;

                return line;
            }

            public int PeekChar()
            {
                return reader.Peek();
            }

            public void Dispose()
            {
                reader.Dispose();
            }
        }

        public class RawParseException : Exception
        {
            public string FileName { get; }
            public int LineNumber { get; }

            public RawParseException(string msg, string filename, int lineNumber)
                : base(msg)
            {
                FileName = filename;
                LineNumber = lineNumber;
            }
        }
    }
}
