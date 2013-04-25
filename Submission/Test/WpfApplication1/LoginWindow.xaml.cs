using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        String ipAddress;
        String userName;
        String passWord;

        public LoginWindow()
        {
            InitializeComponent();

        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ipAddress = textBoxIPAddress.Text;
            userName = textBoxUserName.Text;
            passWord = textBoxPassword.Password;

            MainWindow mw = new MainWindow(ipAddress, userName, passWord);
            this.Hide();
            mw.ShowDialog();

        }

        private void textBoxUserName_TextChanged(object sender, TextChangedEventArgs e)
        {

        }
    }
}
