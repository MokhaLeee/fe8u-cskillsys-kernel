using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.IO
{
    public interface IOutput
    {
        void WriteTo(int position, byte[] data);
        void Commit();
        void Close();
    }
}
