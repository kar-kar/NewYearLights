using System;
using System.Windows.Media;

namespace Simulator
{
    public static class ColorExtensions
    {
        public static Color Blend(this Color c1, Color c2, float ratio)
        {
            ratio = Math.Clamp(ratio, 0f, 1f);
            var r = (byte)(c1.R * (1 - ratio) + c2.R * ratio);
            var g = (byte)(c1.G * (1 - ratio) + c2.G * ratio);
            var b = (byte)(c1.B * (1 - ratio) + c2.B * ratio);
            return Color.FromRgb(r, g, b);
        }

        public static Color BlendLighten(this Color c1, Color c2)
        {
            return Color.FromRgb(Math.Max(c1.R, c2.R), Math.Max(c1.G, c2.G), Math.Max(c1.B, c2.B));
        }
    }
}
