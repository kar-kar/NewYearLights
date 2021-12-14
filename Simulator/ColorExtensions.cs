using System;
using System.Windows.Media;

namespace Simulator
{
    public static class ColorExtensions
    {
        public static Color BlendLighten(this Color c1, Color c2)
        {
            return Color.FromRgb(Math.Max(c1.R, c2.R), Math.Max(c1.G,c2.G), Math.Max(c1.B,c2.B));
        }
    }
}
