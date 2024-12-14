using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.DataTypes
{
    static class Extension
    {
        public static IEnumerable<T> PadTo<T>(this IEnumerable<T> self, int totalLength, T zero)
        {
            int count = 0;
            foreach (T t in self)
            {
                yield return t;
                count++;
            }
            for (; count < totalLength; count++)
                yield return zero; 
        }
        public static BitArray Append(this BitArray x, BitArray y)
        {
            int startLen = x.Length;
            x.Length += y.Length;
            for(int i=startLen; i<x.Length; i++)
            {
                x[i] = y[i - startLen];
            }
            return x;
        }
        public static int ToInt(this string numString)
        {
            if (numString.StartsWith("$"))
            {
                return Convert.ToInt32(numString.Substring(1), 16);
            }
            else if (numString.StartsWith("0x"))
            {
                return Convert.ToInt32(numString.Substring(2), 16);
            }
            else if (numString.EndsWith("b"))
            {
                return Convert.ToInt32(numString.Substring(0, numString.Length - 1), 2);
            }
            else
            {
                return Convert.ToInt32(numString);
            }
        }
        public static Dictionary<K, IList<V>> AddTo<K,V>(this Dictionary<K, IList<V>> self, K key, V val)
            where K: notnull
        {
            if (self.ContainsKey(key))
                self[key].Add(val);
            else
            {
                self[key] = new List<V> { val };
            }
            return self;
        }

        public static void SetBits(this byte[] array, int bitOffset, int bitSize, int value)
        {
            array.SetBits(0, bitOffset, bitSize, value);
        }

        public static void SetBits(this byte[] array, int byteOffset, int bitOffset, int bitSize, int value)
        {
            if (bitOffset >= 8)
            {
                array.SetBits(byteOffset + bitOffset / 8, bitOffset % 8, bitSize, value);
                return;
            }

            long bytes = 0;

            int byteSize = (bitOffset + bitSize + 7) / 8;

            // Read bytes into integer

            for (int i = 0; i < byteSize; ++i)
                bytes |= ((long)array[byteOffset + i]) << i * 8;

            // Apply value to integer

            long mask = (((long)1 << bitSize) - 1) << bitOffset;

            bytes &= ~mask;
            bytes |= ((long)value << bitOffset) & mask;

            // Write integer back into bytes

            for (int i = 0; i < byteSize; ++i)
                array[byteOffset + i] = (byte)(bytes >> i * 8);
        }

        public static void SetBits(this byte[] array, int bitOffset, int bitSize, byte[] data)
        {
            array.SetBits(0, bitOffset, bitSize, data);
        }

        public static void SetBits(this byte[] array, int byteOffset, int bitOffset, int bitSize, byte[] data)
        {
            if (bitOffset >= 8)
            {
                array.SetBits(byteOffset + bitOffset / 8, bitOffset % 8, bitSize, data);
                return;
            }

            int byteSize = (bitOffset + bitSize + 7) / 8;

            if (bitOffset == 0)
            {
                for (int i = 0; i < byteSize - 1; ++i)
                    array[byteOffset + i] = data[i];

                int shift = bitSize % 8 == 0 ? 8 : bitSize % 8;
                uint endMask = (uint)((1 << shift) - 1);

                array[byteOffset + byteSize - 1] &= (byte)~endMask;
                array[byteOffset + byteSize - 1] |= (byte)(data[byteSize - 1] & endMask);
            }
            else
            {
                for (int i = 0; i < byteSize; ++i)
                {
                    int mask = ((1 << Math.Min(bitSize - i * 8, 8)) - 1) << bitOffset;

                    byte loMask = (byte)mask;
                    byte hiMask = (byte)(mask >> 8);

                    array[byteOffset + i] &= (byte)~loMask;
                    array[byteOffset + i] |= (byte)((data[i] << bitOffset) & loMask);

                    array[byteOffset + i + 1] &= (byte)~hiMask;
                    array[byteOffset + i + 1] |= (byte)((data[i] >> (8 - bitOffset)) & hiMask);
                }
            }
        }
    }
}
