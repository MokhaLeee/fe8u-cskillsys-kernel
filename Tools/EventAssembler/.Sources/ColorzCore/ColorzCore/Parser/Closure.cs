using System.Collections.Generic;

namespace ColorzCore.Parser
{
    public class Closure
    {
        private Dictionary<string, int> Labels { get; }

        public Closure()
        {
            Labels = new Dictionary<string, int>();
        }
        public virtual bool HasLocalLabel(string label)
        {
            return Labels.ContainsKey(label);
        }
        public virtual int GetLabel(string label)
        {
            return Labels[label];
        }
        public void AddLabel(string label, int value)
        {
            Labels[label] = value;
        }

        public IEnumerable<KeyValuePair<string, int>> LocalLabels()
        {
            foreach (KeyValuePair<string, int> label in Labels)
            {
                yield return label;
            }
        }
    }
}