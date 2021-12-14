using System.Windows.Media;

namespace Simulator
{
    public class Pixel: NotifyPropertyChanged
    {
        private int x;
        private int y;
        private Color color;

        public int X
        {
            get { return x; }
            set { SetProperty(ref x, value); }
        }

        public int Y
        {
            get { return y; }
            set { SetProperty(ref y, value); }
        }

        public Color Color
        {
            get { return color; }
            set { SetProperty(ref color, value); }
        }
    }
}
