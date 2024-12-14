using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.IO
{
    class ROM : IOutput
    {
        private BufferedStream myStream;
        private byte[] myData;
        private int size;

        public ROM(Stream myROM)
        {
            myStream = new BufferedStream(myROM);
            myData = new byte[0x2000000];
            size = myStream.Read(myData, 0, 0x2000000);
            myStream.Position = 0;
        }

        public void Commit()
        {
            myStream.Write(myData, 0, size);
            myStream.Flush();
        }

        public void WriteTo(int position, byte[] data)
        {
            Array.Copy(data, 0, myData, position, data.Length);
            if (data.Length + position > size)
                size = data.Length + position;
        }

        public void Close()
        {
            myStream.Close();
        }
    }
}
