using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ColorzCore.DataTypes
{
    public delegate R UnaryFunction<T,R>(T val);
    public delegate R RConst<R>();
    public delegate void TAction<T>(T val);
    public delegate void NullaryAction();
    public delegate Maybe<R> MaybeAction<T, R>(T val);

#pragma warning disable IDE1006 // Naming Styles
    public interface Maybe<T>
#pragma warning restore IDE1006 // Naming Styles
    {
        bool IsNothing { get; }
        T FromJust { get; }
        Maybe<R> Fmap<R>(UnaryFunction<T, R> f);
        Maybe<R> Bind<R>(MaybeAction<T, R> f);
        R IfJust<R>(UnaryFunction<T, R> just, RConst<R> nothing);
        void IfJust(TAction<T> just, NullaryAction? nothing = null);
    }
    public class Just<T> : Maybe<T>
    {
        public bool IsNothing { get { return false; } }
        public T FromJust { get; }

        public Just(T val)
        {
            FromJust = val;
        }

        public Maybe<R> Fmap<R>(UnaryFunction<T, R> f)
        {
            return new Just<R>(f(FromJust));
        }
        public Maybe<R> Bind<R>(MaybeAction<T, R> f)
        {
            return f(FromJust);
        }
        public R IfJust<R>(UnaryFunction<T, R> just, RConst<R> nothing)
        {
            return just(FromJust);
        }
        public void IfJust(TAction<T> just, NullaryAction? nothing)
        {
            just(FromJust);
        }
    }
    public class Nothing<T> : Maybe<T>
    {
        public bool IsNothing { get { return true; } }
        public T FromJust { get { throw new MaybeException(); } }

        public Nothing() { }

        public Maybe<R> Fmap<R>(UnaryFunction<T, R> f)
        {
            return new Nothing<R>();
        }
        public Maybe<R> Bind<R>(MaybeAction<T, R> f)
        {
            return new Nothing<R>();
        }
        public R IfJust<R>(UnaryFunction<T, R> just, RConst<R> nothing)
        {
            return nothing();
        }
        public void IfJust(TAction<T> just, NullaryAction? nothing)
        {
            nothing?.Invoke();
        }
    }

    public class MaybeException : Exception { }
}
