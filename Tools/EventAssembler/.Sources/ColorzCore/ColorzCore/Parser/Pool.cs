using System;
using System.Collections.Generic;
using ColorzCore.DataTypes;
using ColorzCore.Lexer;

namespace ColorzCore.Parser
{
    class Pool
    {
        public struct PooledLine
        {
            public ImmutableStack<Closure> Scope { get; private set; }
            public List<Token> Tokens { get; private set; }

            public PooledLine(ImmutableStack<Closure> scope, List<Token> tokens)
            {
                Scope = scope;
                Tokens = tokens;
            }
        }

        public static readonly string pooledLabelPrefix = "__POOLED$";

        public List<PooledLine> Lines { get; private set; }

        private long poolLabelCounter;

        public Pool()
        {
            Lines = new List<PooledLine>();
            poolLabelCounter = 0;
        }

        public string MakePoolLabelName()
        {
            // The presence of $ in the label name guarantees that it can't be a user label
            return string.Format("{0}{1}", pooledLabelPrefix, poolLabelCounter++);
        }
    }
}
