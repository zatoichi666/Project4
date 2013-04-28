/////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - Demo of techniques to build WPF interface  //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013       //
/////////////////////////////////////////////////////////////////////
/*
 * Note: 
 *   You must build the MockChannel DLL before building this
 *   WPF application.  If you don't the build will fail.
 *   To do that you simply right-click on the MockChannel project
 *   and select Rebuild.
 */
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
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using System.IO;
using System.Windows.Forms;
using System.Threading;

namespace WpfApplication1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        String serverIP;
        String userName;
        String password;

        private mockChannel.IChannel chan;

        private Action<string> myItemDelegate;

        public MainWindow(String srvIP, String uName, String pWord)
        {
            serverIP = srvIP;
            userName = uName;
            password = pWord;

            InitializeComponent();

        }

        private void AddItem(string item)
        {
            string msg = "Received Message: " + item;
            listBox1.Items.Add(msg);
        }

        private String MakeQueryMd5AckMessage(string filename, string ipSender, int portSender)
        {
            String header = "";
            header = "[queryMd5;";
            header += "file='" + filename + "'";
            header += "ipSender='" + ipSender + "'";
            header += "portSender='" + (portSender).ToString() + "'";
            header += "]";

            return header;
        }

        private String MakeLoginRequestMessage()
        {
            String header = "";
            header += "[loginReqMsg;";
            header += "uName='" + userName + "'";
            header += "pWord='" + password + "'";
            header += "]";
            return header;
        }

        private void Run()
        {

            Dispatcher.BeginInvoke(new Action(() => { labelConnected.Visibility = Visibility.Hidden; }));
            string msg = MakeLoginRequestMessage();
            chan.postMessage(msg);

            while (true)
            {
                msg = chan.getMessage();
                processMessage(msg);
                Dispatcher.Invoke(myItemDelegate, DispatcherPriority.Background, new object[] { msg });
            }

        }

        private void Window_Loaded_1(object sender, RoutedEventArgs e)
        {
            myItemDelegate = this.AddItem;
            textBox1.Text = Directory.GetCurrentDirectory();
            string[] files = Directory.GetFiles(textBox1.Text, "*.*");
            foreach (string file in files)
                listBox1.Items.Add(System.IO.Path.GetFileName(file));

            button3.IsEnabled = false;
            chan = mockChannel.IChannel.CreateChannel();

            Thread clientReceiveThread = new Thread(new ThreadStart(this.Run));
            clientReceiveThread.IsBackground = true;
            clientReceiveThread.Start();
        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.ShowNewFolderButton = false;
            fbd.SelectedPath = textBox1.Text;
            DialogResult result = fbd.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                Directory.SetCurrentDirectory(fbd.SelectedPath);
                textBox1.Text = fbd.SelectedPath;
                listBox1.Items.Clear();
                string[] files = Directory.GetFiles(textBox1.Text, "*.*");
                foreach (string file in files)
                    listBox1.Items.Add(System.IO.Path.GetFileName(file));
            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = textBox1.Text;
            ofd.Multiselect = true;
            System.Windows.Forms.DialogResult result = ofd.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                listBox1.Items.Clear();
                string[] files = ofd.FileNames;
                foreach (string file in files)
                    listBox1.Items.Add(System.IO.Path.GetFileName(file));
            }
            button3.IsEnabled = true;
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            int size = listBox1.Items.Count;
            for (int i = 0; i < size; ++i)
            {
                string item = listBox1.Items[i] as string;
                chan.postMessage(item);
                listBox1.Items.Add("    Posted Message: " + item);
            }
            button3.IsEnabled = false;
        }

        private void Button_Click_4(object sender, RoutedEventArgs e)
        {
            string msg = MakeLoginRequestMessage();
            chan.postMessage(msg);
        }

        private void processMessage(string message)
        {
            int posOpenSquareBracket = message.IndexOf("[");
            int posMsgTypeSemicolon = message.IndexOf(";");
            
            string messageType = "";
            if (posOpenSquareBracket > -1)
                if (posMsgTypeSemicolon > -1)
                    messageType = message.Substring(posOpenSquareBracket + 1, posMsgTypeSemicolon - posOpenSquareBracket - 1);
         
            if (messageType == "sendBin")
            {

            }
            if (messageType == "queryMd5")
            {

            }
            if (messageType == "ackMd5")
            {

            }
            if (messageType == "ackBin")
            {

            }
            if (messageType == "loginReqMsg")
            {

            }
            if (messageType == "ackLogin")
            {
                Dispatcher.BeginInvoke(new Action(() => {
                    listBox1.Items.Add("    Just Received ackLogin ");
                }));
            }

        }

    }
}
