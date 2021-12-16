using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Media;

namespace Simulator.Effects
{
    public class Rain : Effect
    {
        private class Drop
        {
            public int X { get; set; }
            public float Y { get; set; }
            public Color Color { get; set; }
        };

        private readonly Random rnd = new Random();
        private Drop[] drops = Array.Empty<Drop>();
        private long lastMillis;

        public Color Background { get; set; } = Color.FromRgb(10, 10, 10);
        public float Speed { get; set; } = 0.01f;
        public float Tail { get; set; } = 3f;
        public int MinDistance { get; set; } = 5;
        public int MaxDistance { get; set; } = 20;
        public Color[] Colors { get; set; } = new[]
        {
            Color.FromRgb(255, 0, 255),
            Color.FromRgb(100, 100, 255),
            Color.FromRgb(0, 100, 255),
            Color.FromRgb(100, 0, 255),
            Color.FromRgb(0, 255, 255),
            Color.FromRgb(0, 0, 255),
        };

        public Rain(Matrix matrix) : base(matrix)
        {
        }

        public override void Setup()
        {
            var dropCount = ((matrix.Height + Tail * 2) / (MinDistance + 1) + 1) * matrix.Width;
            var drops = new List<Drop>();
            var x = 0;
            for (var i = 0; i < dropCount; i++)
            {
                drops.Add(CreateDrop(drops, x));
                x = (x + 1) % matrix.Width;
            }
            this.drops = drops.ToArray();

            Clear();
            lastMillis = watch.ElapsedMilliseconds;
        }

        public override void Loop()
        {
            var ms = watch.ElapsedMilliseconds;
            var dt = ms - lastMillis;
            lastMillis = ms;

            if (dt == 0)
                return;

            Move(dt);
            Draw();
        }

        private void Move(long dt)
        {
            var dy = Speed * dt;

            foreach (var drop in drops)
                drop.Y += dy;

            for (var i = 0; i < drops.Length; i++)
                if (drops[i].Y > matrix.Height + Tail * 2)
                    drops[i] = CreateDrop(drops, drops[i].X);
        }

        private void Draw()
        {
            Clear();

            foreach (var drop in drops)
            {
                if (drop.Y < -1)
                    continue;

                for (var pixelY = 0; pixelY < matrix.Height; pixelY++)
                {
                    var dist = Math.Abs(pixelY - drop.Y);
                    var factor = 1f - MathF.Pow(dist + 0.3f, 2);
                    factor = Math.Max(0f, Math.Min(1f, factor));

                    if (pixelY < drop.Y && Tail > 0)
                    {
                        factor += 1 - MathF.Pow(1.5f, dist - Tail);
                        factor = Math.Max(0f, Math.Min(1f, factor));
                    }

                    var color = Color.Multiply(drop.Color, (float)factor);
                    var pixel = matrix[drop.X, pixelY];
                    pixel.Color = pixel.Color.BlendLighten(color);
                }
            }
        }

        private Drop CreateDrop(IEnumerable<Drop> existingDrops, int x)
        {
            var minY = 0f;
            foreach (var drop in existingDrops)
                if (drop.X == x)
                    minY = Math.Min(minY, drop.Y);

            return new Drop
            {
                X = x,
                Y = minY - rnd.Next(MinDistance, MaxDistance + 1),
                Color = Colors[rnd.Next(Colors.Length)]
            };
        }

        private void Clear()
        {
            foreach (var pixel in matrix.Pixels)
                pixel.Color = Background;
        }
    }
}
