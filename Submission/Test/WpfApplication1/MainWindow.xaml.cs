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
            ListBoxSessionOutput.Items.Add(msg);
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

        private String MakeNewCheckinMessage(String checkinName)
        {
            String header;
            header = "[newCheckin;";
            header += "checkinName='" + checkinName + "'";
            header += "]";
            return header;
        }

        private String MakeCheckinRequestMessage()
        {
            String header;
            header = "[checkinRequest;";
            header += "]";
            return header;
        }

        private String MakeSendBinMessage(iPacketizer.PacketizerWrapper p, String destIp, int destPort, uint packetIndex)
        {
            String header;
            header = "[sendBin;";
            header += "file='" + p.getFileName() + "'";
            header += "pCount='" + p.size().ToString() + "'";
            header += "pInd='" + packetIndex.ToString() + "'";
            header += "dIp='" + destIp + "'";
            header += "dPort='" + destPort.ToString() + "'";
            header += "]";
            header += p.op_Subscript(packetIndex);
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

            Dispatcher.BeginInvoke(new Action(() =>
            {
                setFormControls(false);
            }));


            string msg = MakeLoginRequestMessage();
            chan.postMessage(msg);

            msg = MakeNewCheckinMessage("new");
            chan.postMessage(msg);

            while (true)
            {
                msg = chan.getMessage();
                processMessage(msg);
                Dispatcher.Invoke(myItemDelegate, DispatcherPriority.Background, new object[] { msg });
            }

        }

        private void setFormControls(Boolean ctrl)
        {
            buttonPostMessage.IsEnabled = ctrl;
            buttonBrowse.IsEnabled = ctrl;
            buttonGetFiles.IsEnabled = ctrl;
            textBoxDirectory.IsEnabled = ctrl;
            listBoxLocalFiles.IsEnabled = ctrl;
            if (ctrl == true)
                labelConnected.Content = "Connected";
            else
                labelConnected.Content = "Disconnected";
        }

        private void Window_Loaded_1(object sender, RoutedEventArgs e)
        {
            myItemDelegate = this.AddItem;
            textBoxDirectory.Text = Directory.GetCurrentDirectory();
            string[] files = Directory.GetFiles(textBoxDirectory.Text, "*.*");
            foreach (string file in files)
                listBoxLocalFiles.Items.Add(System.IO.Path.GetFileName(file));

            string[] dirs = Directory.GetDirectories(textBoxDirectory.Text, "*.*");
            foreach (string dir in dirs)
                listBoxLocalFiles.Items.Add(System.IO.Path.GetFileName(dir) + "/");

            setFormControls(false);

            chan = mockChannel.IChannel.CreateChannel();

            Thread clientReceiveThread = new Thread(new ThreadStart(this.Run));
            clientReceiveThread.IsBackground = true;
            clientReceiveThread.Start();

        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.ShowNewFolderButton = false;
            fbd.SelectedPath = textBoxDirectory.Text;
            DialogResult result = fbd.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                Directory.SetCurrentDirectory(fbd.SelectedPath);
                textBoxDirectory.Text = fbd.SelectedPath;
                listBoxLocalFiles.Items.Clear();
                string[] files = Directory.GetFiles(textBoxDirectory.Text, "*.*");
                foreach (string file in files)
                    listBoxLocalFiles.Items.Add(System.IO.Path.GetFileName(file));

                string[] dirs = Directory.GetDirectories(textBoxDirectory.Text, "*.*");
                foreach (string dir in dirs)
                    listBoxLocalFiles.Items.Add(System.IO.Path.GetFileName(dir) + "\\");


            }
        }

        private void Button_Click_2(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = textBoxDirectory.Text;
            ofd.Multiselect = true;
            System.Windows.Forms.DialogResult result = ofd.ShowDialog();
            if (result == System.Windows.Forms.DialogResult.OK)
            {
                listBoxLocalFiles.Items.Clear();
                string[] files = ofd.FileNames;
                foreach (string file in files)
                    listBoxLocalFiles.Items.Add(System.IO.Path.GetFileName(file));
            }
            buttonPostMessage.IsEnabled = true;
        }

        private void Button_Click_3(object sender, RoutedEventArgs e)
        {
            String msg = MakeCheckinRequestMessage();
            chan.postMessage(msg);
        }

        private void sendBinFileToRemoteServer(string filename)
        {
            iPacketizer.PacketizerWrapper pack = new iPacketizer.PacketizerWrapper(filename);

            for (uint i = 0; i < pack.size(); i++)
            {
                chan.postMessage(MakeSendBinMessage(pack, "127.0.0.1", 8050, i));
            }

            //ListBoxSessionOutput.Items.Add("Successfully uploaded " + filename);

        }

        private void processMissingPackMsg(string message)
        {
            Dispatcher.BeginInvoke(new Action(() =>
            {
                ListBoxSessionOutput.Items.Add("Got Missing Package Message");
            }));
        }


        private Boolean processAckLoginMsg(string message)
        {
            int posCredentialsAccepted = message.IndexOf("result='");

            string credentialsAccepted = "";
            if (posCredentialsAccepted > -1)
                credentialsAccepted = message.Substring(posCredentialsAccepted + 8, 1);

            return credentialsAccepted == "1";
        }

        private void processMessage(string message)
        {
            int posOpenSquareBracket = message.IndexOf("[");
            int posMsgTypeSemicolon = message.IndexOf(";");

            string messageType = "";
            if (posOpenSquareBracket > -1)
                if (posMsgTypeSemicolon > -1)
                    messageType = message.Substring(posOpenSquareBracket + 1, posMsgTypeSemicolon - posOpenSquareBracket - 1);

            if (messageType == "missingPack")
            {
                processMissingPackMsg(message);
            }

            if (messageType == "ackLogin")
            {
                if (processAckLoginMsg(message))
                {
                    Dispatcher.BeginInvoke(new Action(() =>
                    {
                        setFormControls(true);
                    }));
                }
            }

        }

        private void ButtonRemoveFromPending_Click(object sender, RoutedEventArgs e)
        {
            ListBoxPending.Items.Remove(ListBoxPending.SelectedItem);
        }



        private void ButtonAddToPending_Click(object sender, RoutedEventArgs e)
        {
            String fileToAdd;                 

            for (int i = 0; i < listBoxLocalFiles.SelectedItems.Count; i++)
            {
                fileToAdd = textBoxDirectory.Text + "\\" + listBoxLocalFiles.SelectedItems[i].ToString();
                sendBinFileToRemoteServer(fileToAdd);
                ListBoxPending.Items.Add(fileToAdd);
            }
    
        }

        private void ListBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }

        private void listBoxLocalFiles_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

        }





    }
}
