using System.Diagnostics;
using System.Windows.Media;

namespace Simulator.Effects
{
    public abstract class Effect
    {
        private long lastMillis;
        protected readonly Matrix matrix;
        protected readonly Stopwatch watch = Stopwatch.StartNew();

        public Color Background { get; set; } = Colors.Black;

        protected Effect(Matrix matrix)
        {
            this.matrix = matrix;
        }

        public void Setup()
        {
            Clear();
            lastMillis = 0;
            watch.Restart();
            SetupInternal();
        }

        protected virtual void SetupInternal()
        {
        }

        public void Loop()
        {
            var ms = watch.ElapsedMilliseconds;
            var dt = ms - lastMillis;
            lastMillis = ms;

            if (dt > 0)
                LoopInternal(dt);
        }

        protected virtual void LoopInternal(long dt)
        {
        }

        protected void Clear()
        {
            foreach (var pixel in matrix.Pixels)
                pixel.Color = Background;
        }
    }
}
