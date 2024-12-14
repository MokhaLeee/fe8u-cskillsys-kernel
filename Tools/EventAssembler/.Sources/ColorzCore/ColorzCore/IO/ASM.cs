using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.IO
{
    class ASM : IOutput
    {
        private StreamWriter asmStream, ldsStream;

        public ASM(StreamWriter asmStream, StreamWriter ldsStream)
        {
            this.asmStream = asmStream;
            this.ldsStream = ldsStream;
        }

        private void WriteToASM(int position, byte[] data)
        {
            string sectionName = String.Format(".ea_{0:x}", 0x8000000 + position);
            asmStream.WriteLine(".section {0},\"ax\",%progbits", sectionName);
            asmStream.WriteLine(".global {0}", sectionName);
            asmStream.WriteLine("{0}:", sectionName);
            asmStream.Write("\t.byte ");
            foreach (byte value in data)
                asmStream.Write("0x{0:x}, ", value);
            asmStream.WriteLine();
        }

        private void WriteToLDS(int position)
        {
            string sectionName = String.Format(".ea_{0:x}", 0x8000000 + position);
            ldsStream.WriteLine(". = 0x{0:x};", 0x8000000 + position);
            ldsStream.WriteLine("{0} : {{*.o({0})}}", sectionName);
        }

        public void WriteTo(int position, byte[] data)
        {
            WriteToASM(position, data);
            WriteToLDS(position);
        }

        public void Commit()
        {
            asmStream.Flush();
            ldsStream.Flush();
        }

        public void Close()
        {
            asmStream.Close();
            ldsStream.Close();
        }
    }
}
