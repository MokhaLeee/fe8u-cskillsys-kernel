using ColorzCore.DataTypes;
using ColorzCore.Parser.AST;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.Raws
{
    class Raw
    {
        public string Name { get; }
        public int Alignment { get; }
        public HashSet<string> Game { get; }

        private readonly IList<IRawParam> parameters;
        private readonly bool repeatable;

        private readonly int unitSize;
        private readonly byte[] baseUnit;
        private readonly byte[]? endUnit;

        // TODO: fixed mask?

        public Raw(string name, int length, short code, int offsetMod, HashSet<string> game, IList<IRawParam> varParams, 
            IList<FixedParam> fixedParams, Maybe<int> terminatingList, bool repeatable)
        {
            Name = name;
            Game = game;
            Alignment = offsetMod;

            this.parameters = varParams;
            this.repeatable = repeatable;

            unitSize = length;
            baseUnit = new byte[(unitSize + 7) / 8];

            // Build base unit

            if (code != 0)
                baseUnit.SetBits(0, 16, code);

            foreach (var fp in fixedParams)
                baseUnit.SetBits(fp.position, fp.size, fp.value);

            // Build end unit, if needed

            if (!terminatingList.IsNothing)
            {
                int terminator = terminatingList.FromJust;

                if (parameters.Count == 0)
                    return;

                endUnit = (byte[])baseUnit.Clone();
                endUnit.SetBits(parameters[0].Position, parameters[0].Length, terminator);

                // force repeatable to be true if this is terminating list
                this.repeatable = true;
            }
        }

        public int UnitCount(int paramCount)
        {
            if (parameters.Count == 0)
                return 1;

            return paramCount / parameters.Count;
        }

        public int LengthBits(int paramCount)
        {
            int count = UnitCount(paramCount);

            if (endUnit != null)
                count++;

            return count * unitSize;
        }

        public int LengthBytes(int paramCount)
        {
            return (LengthBits(paramCount) + 7) / 8;
        }

        public bool Fits(IList<IParamNode> arguments)
        {
            int unitCount = repeatable
                ? UnitCount(arguments.Count)
                : 1;

            if (unitCount == 0)
                return false;

            if (arguments.Count != parameters.Count * unitCount)
                return false;

            for (int i = 0; i < unitCount; ++i)
            {
                for (int j = 0; j < parameters.Count; ++j)
                {
                    if (!parameters[j].Fits(arguments[i * parameters.Count + j]))
                        return false;
                }
            }

            return true;
        }
        
        /* Precondition: params fits the shape of this raw's params. */
        public byte[] GetBytes(IList<IParamNode> arguments)
        {
            var result = new byte[LengthBytes(arguments.Count)];

            var count = UnitCount(arguments.Count);

            for (int i = 0; i < count; ++i)
            {
                var unit = (byte[])baseUnit.Clone();

                for (int j = 0; j < parameters.Count; ++j)
                {
                    parameters[j].Set(unit, arguments[i * parameters.Count + j]);
                }

                result.SetBits(i * unitSize, unitSize, unit);
            }

            if (endUnit != null)
                result.SetBits(count * unitSize, unitSize, endUnit);

            return result;
        }

        public struct FixedParam
        {
            public int position;
            public int size;
            public int value;
        }
    }
}
