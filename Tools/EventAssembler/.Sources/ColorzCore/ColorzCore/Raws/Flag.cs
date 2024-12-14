using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ColorzCore.DataTypes;

namespace ColorzCore.Raws
{
    class Flag
    {
        public Either<IList<string>, Tuple<int, int>> Values { get; }
        public Flag()
        {
            Values = new Left<IList<string>, Tuple<int, int>>(new List<string>());
        }
        public Flag(IList<string> values)
        {
            Values = new Left<IList<string>, Tuple<int, int>>(values);
        }
        public Flag(int valBeg, int valEnd)
        {
            Values = new Right<IList<string>, Tuple<int, int>>(new Tuple<int, int>(valBeg, valEnd));
        }
    }
}
