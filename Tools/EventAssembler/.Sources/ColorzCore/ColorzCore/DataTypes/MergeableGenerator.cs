using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.DataTypes
{
    public class MergeableGenerator<T>: IEnumerable<T>
    {
        private Stack<IEnumerator<T>> myEnums;
        public bool EOS { get; private set; }
        public MergeableGenerator(IEnumerable<T> baseEnum)
        {
            myEnums = new Stack<IEnumerator<T>>();
            myEnums.Push(baseEnum.GetEnumerator());
        }

        public T Current { get { return myEnums.Peek().Current; } }
        public bool MoveNext()
        {
            if(!myEnums.Peek().MoveNext())
            {
                if(myEnums.Count > 1)
                {
                    myEnums.Pop();
                    return true;
                }
                else
                {
                    EOS = true;
                    return false;
                }
            }
            else
            {
                return true;
            }
        }
        public void PrependEnumerator(IEnumerator<T> nextEnum)
        {
            if (EOS)
                myEnums.Pop();
            myEnums.Push(nextEnum);
            Prime();
        }
        public void PutBack(T elem)
        {
            this.PrependEnumerator(new List<T> { elem }.GetEnumerator());
        }
        private bool Prime()
        {
            if (!myEnums.Peek().MoveNext())
            {
                if (myEnums.Count == 1)
                    EOS = true;
                else 
                    myEnums.Pop();
            }
            else
                EOS = false;
            return !EOS;
        }
        IEnumerator<T> IEnumerable<T>.GetEnumerator()
        {
            while(!EOS) {
                yield return this.Current;
                this.MoveNext();
            }
        }
        public IEnumerator GetEnumerator()
        {
            while (!EOS)
            {
                yield return this.Current;
                this.MoveNext();
            }
        }
    }
}
