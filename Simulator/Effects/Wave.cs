using System;
using System.Windows.Media;

namespace Simulator.Effects
{
    public class Wave : Effect
    {
        private float phase;

        public float WaveLength { get; set; } = 18f;
        public float WaveSpeed { get; set; } = 0.0004f;
        public Color LowColor { get; set; } = Color.FromRgb(127, 80, 0);
        public Color HighColor { get; set; } = Colors.Yellow;

        public Wave(Matrix matrix) : base(matrix)
        {
            Background = Colors.DarkGray;
        }

        protected override void LoopInternal(long dt)
        {
            phase = (phase + dt * WaveSpeed) % 1f;
            Draw();
        }

        private void Draw()
        {
            for (var x = 0; x < matrix.Width; x++)
            {
                var dist = Math.Abs(x - WaveLength * phase) % WaveLength;
                dist = Math.Min(dist, WaveLength - dist);
                var ratio = dist < 1f ? 1f : 1f / dist;
                ratio = Math.Clamp(ratio, 0f, 1f);
                var color = LowColor.Blend(HighColor, ratio);

                for (var y = 0; y < matrix.Height; y++)
                    if ((x + y) % 2 != 0)
                        matrix[x, y].Color = color;
            }
        }
    }
}
