using System.Collections.Generic;
using System.Linq;
using System.Windows.Media;

namespace Simulator
{
    public class Matrix
    {
        public Pixel[] Pixels { get; }
        public int Width  => 14;
        public int Height => 11;

        public Pixel this[int x, int y]
        {
            get
            {
                if (x == 4 || x == 9)
                    return new Pixel(); //invisible lines
                if (x > 9)
                    x--;
                if (x > 4)
                    x--;
                return Pixels[x * Height + y];
            }
        }

        public Matrix()
        {
            Pixels = InitPixels().ToArray();
        }

        private IEnumerable<Pixel> InitPixels()
        {
            for (var x = 0; x < Width; x++)
            {
                if (x == 4 || x == 9)
                    continue;

                for (var y = 0; y < Height; y++)
                {
                    yield return new Pixel
                    {
                        X = x * 80,
                        Y = y * 40,
                        Color = Colors.Green
                    };
                }
            }
        }
    }
}
