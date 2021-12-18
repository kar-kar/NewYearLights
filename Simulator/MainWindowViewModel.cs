using Simulator.Effects;
using System;
using System.Diagnostics;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Threading;
using Utils;

namespace Simulator
{
    public class MainWindowViewModel: NotifyPropertyChanged
    {
        private readonly DispatcherTimer timer;
        private readonly Effect[] effects;
        private Effect effect;
        private bool isRunning;
        private Stopwatch fpsWatch = Stopwatch.StartNew();
        private int frameCount;
        private double fps;

        public Matrix Matrix { get; } = new Matrix();
        public ICommand StartCmd { get; }
        public ICommand StopCmd { get; }
        public ICommand NextCmd { get; }

        public double Fps
        {
            get { return fps; }
            set { SetProperty(ref fps, value); }
        }

        public MainWindowViewModel()
        {
            StartCmd = new DelegateCommand(p => Start());
            StopCmd = new DelegateCommand(p => Stop());
            NextCmd = new DelegateCommand(p => Next());

            effects = new[]
            {
                new Rain(Matrix),
                new Rain(Matrix)
                {
                    Speed = 0.003f,
                    Background = Colors.Black,
                    Colors = new[] { Colors.White },
                    Tail = 0f,
                    MinDistance = 3,
                    MaxDistance = 7
                }
            };

            effect = effects[0];

            timer = new(DispatcherPriority.Background) { Interval = TimeSpan.FromMilliseconds(1) };
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private void Timer_Tick(object? sender, EventArgs e)
        {
            if (isRunning)
            {
                effect.Loop();
                frameCount++;

                if (fpsWatch.Elapsed > TimeSpan.FromSeconds(1))
                {
                    Fps = frameCount / (double)fpsWatch.ElapsedMilliseconds * 1000;
                    frameCount = 0;
                    fpsWatch = Stopwatch.StartNew();
                }
            }
        }

        private void Start()
        {
            effect?.Setup();
            isRunning = true;
            fpsWatch = Stopwatch.StartNew();
            frameCount = 0;
        }

        private void Stop()
        {
            isRunning = false;
            Fps = 0;
        }

        private void Next()
        {
            effect = effects[(Array.IndexOf(effects, effect) + 1) % effects.Length];

            if (isRunning)
                effect.Setup();
        }
    }
}
