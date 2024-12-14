using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ColorzCore.Parser.AST;
using ColorzCore.DataTypes;
using System.Collections;

namespace ColorzCore.Raws
{
    class ListParam : IRawParam
    {
        public string Name { get; }
        public int Position { get; }
        public int Length { get; }

        private readonly int numCoords;

        public ListParam(string name, int position, int length, int numCoords)
        {
            Name = name;
            Position = position;
            Length = length;
            this.numCoords = numCoords;
        }

        public void Set(byte[] data, IParamNode input)
        {
            IList<IAtomNode> interior = ((ListNode)input).Interior;

            int bitsPerAtom = Length / numCoords;
            int count = 0;

            foreach (IAtomNode a in interior)
            {
                data.SetBits(Position + count, bitsPerAtom, a.CoerceInt());
                count += bitsPerAtom;
            }

            // NOTE: do we need this? a unit will always start filled with zeroes,
            // Setting zeroes here only matters if the code has overlapping parameters

            if (count != Length)
                data.SetBits(Position + count, Length - count, 0);
        }

        public bool Fits(IParamNode input)
        {
            if (input.Type == ParamType.LIST)
            {
                ListNode n = (ListNode)input;
                return n.NumCoords <= numCoords;
            }
            else
                return false;
        }
    }
}
