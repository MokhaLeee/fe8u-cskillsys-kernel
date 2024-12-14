using System;
using System.Collections.Generic;
using System.Diagnostics;

namespace ColorzCore
{
    public class ExecTimer
    {
        public static ExecTimer Timer { get; } = new ExecTimer();

        public const string KEY_RESET = "__reset";
        public const string KEY_GENERIC = "parsing-interpreting";
        public const string KEY_RAWPROC = "raw-processing";
        public const string KEY_DATAWRITE = "data-writing";

        private List<Tuple<DateTime, string>> timingPoints;

        private Dictionary<string, TimeSpan>? times;
        private Dictionary<string, int>? counts;

        private TimeSpan totalTime;
        
        private ExecTimer()
        {
            timingPoints = new List<Tuple<DateTime, string>>();
            timingPoints.Add(new Tuple<DateTime, string>(DateTime.Now, KEY_RESET));

            times = null;
            counts = null;
        }

        public SortedList<TimeSpan, string> SortedTimes
        {
            get
            {
                if (this.times == null)
                    ComputeTimes();

                SortedList<TimeSpan, string> sortedTimes = new SortedList<TimeSpan, string>();

                if (this.times != null)
                    foreach (KeyValuePair<string, TimeSpan> time in this.times)
                        sortedTimes.Add(time.Value, time.Key);

                return sortedTimes;
            }
        }

        public Dictionary<string, TimeSpan> Times
        {
            get
            {
                if (this.times == null)
                    ComputeTimes();

                return this.times!;
            }
        }

        public Dictionary<string, int> Counts
        {
            get
            {
                if (this.counts == null)
                    ComputeTimes();

                return this.counts!;
            }
        }

        public TimeSpan TotalTime
        {
            get
            {
                if (this.totalTime == null)
                    ComputeTimes();

                return this.totalTime;
            }
        }

        public void AddTimingPoint(string key)
        {
            timingPoints.Add(new Tuple<DateTime, string>(DateTime.Now, key));
        }

        private void ComputeTimes()
        {
            DateTime current = DateTime.Now;

            this.times = new Dictionary<string, TimeSpan>();
            this.counts = new Dictionary<string, int>();

            this.totalTime = TimeSpan.Zero;

            foreach (Tuple<DateTime, string> point in timingPoints)
            {
                if (point.Item2 != KEY_RESET)
                {
                    TimeSpan span = point.Item1.Subtract(current);

                    if (times.ContainsKey(point.Item2))
                    {
                        times[point.Item2] += span;

                        switch (point.Item2)
                        {
                            case KEY_RAWPROC:
                            case KEY_GENERIC:
                            case KEY_DATAWRITE:
                                break;

                            default:
                                counts[point.Item2] += 1;
                                break;
                        }
                    }
                    else
                    {
                        times[point.Item2] = span;
                        counts[point.Item2] = 1;
                    }

                    totalTime += span;
                }

                current = point.Item1;
            }
        }
    }
}
