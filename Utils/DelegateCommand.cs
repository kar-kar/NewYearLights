using System;
using System.Windows.Input;

namespace Utils
{
    public class DelegateCommand : ICommand
    {
        private readonly Func<object?, bool>? canExecute;
        private readonly Action<object?>? execute;

        public event EventHandler? CanExecuteChanged;

        public DelegateCommand(Action<object?>? execute = null, Func<object?, bool>? canExecute = null)
        {
            this.execute = execute;
            this.canExecute = canExecute;
        }

        public bool CanExecute(object? parameter)
        {
            return canExecute?.Invoke(parameter) ?? true;
        }

        public void Execute(object? parameter)
        {
            execute?.Invoke(parameter);
        }
    }
}
