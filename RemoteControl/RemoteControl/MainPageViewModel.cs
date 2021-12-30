using Microsoft.Azure.Devices;
using System;
using System.ComponentModel;
using System.Reactive.Linq;
using System.Text;
using System.Threading;
using System.Windows.Input;
using Utils;

namespace RemoteControl
{
    public class MainPageViewModel: NotifyPropertyChanged
    {
        private int brightness = 20;
        private string? status;

        public int Brightness
        {
            get { return brightness; }
            set { SetProperty(ref brightness, value); }
        }

        public string? Status
        {
            get { return status; }
            set { SetProperty(ref status, value); }
        }

        public ICommand StartCmd { get; }
        public ICommand StopCmd { get; }
        public ICommand NextCmd { get; }
        public ICommand ResetCmd { get; }

        public MainPageViewModel()
        {
            StartCmd = new DelegateCommand(_ => Start());
            StopCmd = new DelegateCommand(_ => Stop());
            NextCmd = new DelegateCommand(_ => Next());
            ResetCmd = new DelegateCommand(_ => Reset());

            Observable.FromEventPattern<PropertyChangedEventHandler, PropertyChangedEventArgs>(
                handler => handler.Invoke,
                h => PropertyChanged += h,
                h => PropertyChanged -= h)
                .Where(e => e.EventArgs.PropertyName == nameof(Brightness))
                .Select(_ => Brightness)
                .Throttle(TimeSpan.FromSeconds(1))
                .ObserveOn(SynchronizationContext.Current)
                .Subscribe(OnBrightnessChanged);
        }

        private void OnBrightnessChanged(int value)
        {
            SendMessage($"br {value}");
        }

        private void Start()
        {
            SendMessage("start");
        }

        private void Stop()
        {
            SendMessage("stop");
        }

        private void Next()
        {
            SendMessage("next");
        }

        private void Reset()
        {
            SendMessage("reset");
        }

        private async void SendMessage(string text)
        {
            Status = $"Sending message '{text}'...";
            var message = new Message(Encoding.ASCII.GetBytes(text)) { Ack = DeliveryAcknowledgement.None };

            try
            {
                using var client = ServiceClient.CreateFromConnectionString(Secrets.IotHubConnectionString);
                await client.SendAsync(Secrets.IotHubDeviceId, message);
                Status = $"Message '{text}' sent";
                await client.CloseAsync();
            }
            catch (Exception ex)
            {
                Status = ex.Message;
            }
        }
    }
}
