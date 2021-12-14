using System.Diagnostics;

namespace Simulator.Effects
{
    public abstract class Effect
    {
        protected readonly Matrix matrix;
        protected readonly Stopwatch watch = Stopwatch.StartNew();

        protected Effect(Matrix matrix)
        {
            this.matrix = matrix;
        }

        public abstract void Setup();
        public abstract void Loop();
    }
}
