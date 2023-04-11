using ExtendedSerialPort;
using System;
using System.Collections.Generic;
using System.IO.Ports;
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

namespace RobotInterfaceJulesBradshaw
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        ReliableSerialPort serialPort1;
        DispatcherTimer timerAffichage;
        Robot robot;
        Queue<byte> byteListReceived;


        public MainWindow()
        {
            InitializeComponent();
            serialPort1 = new ReliableSerialPort("COM15", 115200, Parity.None, 8, StopBits.One);
            serialPort1.DataReceived += serialPort1_DataReceived;
            serialPort1.Open();
            byteListReceived = new Queue<byte>();
            Robot robot = new Robot();


            timerAffichage = new DispatcherTimer();
            timerAffichage.Interval = new TimeSpan(0, 0, 0, 0, 100);
            timerAffichage.Tick += TimerAffichage_Tick; ;
            timerAffichage.Start();


        }

        private void TimerAffichage_Tick(object sender, EventArgs e)
        {
            // if (robot.receivedText != "")
            // {
            //Reception.Text += "Reçu Robot : " + robot.receivedText + "\n";
            //robot.receivedText = "";
            // Queue.enqueue(number);
            // }
            while (byteListReceived.Count != 0)
            {
                var c = byteListReceived.Dequeue();
                DecodeMessage(c);
                //Reception.Text += "0x" + c.ToString("X2") + " ";
            }
        }

        public void serialPort1_DataReceived(object sender, DataReceivedArgs e)
        {
            //Robot.receivedText += Encoding.UTF8.GetString(e.Data, 0, e.Data.Length);

            //Console.WriteLine("\nContents of the first copy:");
            for (int i = 0; i < e.Data.Length; i++)
            {
                byteListReceived.Enqueue(e.Data[i]);
            }
        }

        private void textBox_TextChanged(object sender, TextChangedEventArgs e)
        {

        }

        private void buttonEnvoyer_Click(object sender, RoutedEventArgs e)
        {
            if (buttonEnvoyer.Background == Brushes.RoyalBlue)
            {
                buttonEnvoyer.Background = Brushes.Beige;
            }
            else
            {
                buttonEnvoyer.Background = Brushes.RoyalBlue;
            }
            SendMessage();
        }

        private void buttonClear_Click(object sender, RoutedEventArgs e)
        {
            Reception.Text = "";
        }


        private void buttonTest_Click(object sender, RoutedEventArgs e)
        {
            //var byteList = new byte[20];
            //for (int i = 0; i < 20; i++)
            //{
            //    byteList[i] = (byte)(2 * i);
            //}
            //serialPort1.Write(byteList, 0, byteList.Length);

            string s = "Bonjour";
            byte[] array = Encoding.ASCII.GetBytes(s);
            UartEncodeAndSendMessage(0x0080, array.Length, array);
        }

        private void textBoxEmission_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SendMessage();
            }
        }
        private void SendMessage()
        {
            //Reception.Text += "Reçu : " + textBoxEmission.Text + "\n";
            serialPort1.WriteLine(textBoxEmission.Text);
            string s = textBoxEmission.Text;
            byte[] array = Encoding.ASCII.GetBytes(s);
            UartEncodeAndSendMessage(0x0080, array.Length, array);
            textBoxEmission.Text = "";


        }

        byte CalculateChecksum(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte cks = 0;
            cks ^= 0xFE;
            cks ^= (byte)(msgFunction >> 8);
            cks ^= (byte)(msgFunction >> 0);
            cks ^= (byte)(msgPayloadLength >> 8);
            cks ^= (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayload.Length; i++)
            {
                cks ^= (byte)msgPayload[i];
            }
            return cks;
        }

        private void UartEncodeAndSendMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            byte[] message = new byte[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (byte)(msgFunction >> 8);
            message[pos++] = (byte)(msgFunction >> 0);
            message[pos++] = (byte)(msgPayloadLength >> 8);
            message[pos++] = (byte)(msgPayloadLength >> 0);
            for (int i = 0; i < msgPayload.Length; i++)
            {
                message[pos++] = msgPayload[i];

            }

            message[pos++] = CalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            serialPort1.Write(message, 0, message.Length);
            //SendMessage();

        }
        public enum StateReception
        {
            Waiting,
            FunctionMSB,
            FunctionLSB,
            PayloadLengthMSB,
            PayloadLengthLSB,
            Payload,
            CheckSum
        }

        StateReception rcvState = StateReception.Waiting;
        int msgDecodedFunction = 0;
        int msgDecodedPayloadLength = 0;
        byte[] msgDecodedPayload;
        int msgDecodedPayloadIndex = 0;
        byte receivedChecksum;
        

        private void DecodeMessage(byte c)
        {
            switch (rcvState)
            {
                case StateReception.Waiting:
                    if (c == 0xFE) ;
                    rcvState = StateReception.FunctionMSB;
                    break;
                case StateReception.FunctionMSB:
                    msgDecodedFunction = c<<8;
                    rcvState = StateReception.FunctionLSB;
            break;
                case StateReception.FunctionLSB:
                    msgDecodedFunction += c << 0;
                    rcvState = StateReception.PayloadLengthMSB;
             break;
                case StateReception.PayloadLengthMSB:
                    msgDecodedPayloadLength = c <<8;
                    rcvState = StateReception.PayloadLengthLSB;
            break;
                case StateReception.PayloadLengthLSB:
                    msgDecodedPayloadLength += c<<0;
                    if (msgDecodedPayloadLength == 0)
                    {
                        rcvState = StateReception.CheckSum;
                    }
                    else
                    {
                        msgDecodedPayload = new byte[msgDecodedPayloadLength];
                        msgDecodedPayloadIndex = 0;
                        rcvState = StateReception.Payload;
                    }
            break;
                case StateReception.Payload:
                    if (msgDecodedPayloadIndex < msgDecodedPayloadLength - 1)
                    {
                        msgDecodedPayload[msgDecodedPayloadIndex++] = c;
                    }
                    else
                    {
                        msgDecodedPayload[msgDecodedPayloadIndex] = c;
                        rcvState = StateReception.CheckSum;
                    }
                    break;
                case StateReception.CheckSum:
                    receivedChecksum = c;
                    if (CalculateChecksum(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload) == receivedChecksum)
                    {
                        Console.WriteLine("C'est bon !");
                        //Reception.Text += "0xFE";
                        //Reception.Text += msgDecodedFunction.ToString("X4");
                        //Reception.Text += msgDecodedPayloadLength.ToString("X4");
                        //Reception.Text += Encoding.ASCII.GetString(msgDecodedPayload);
                        //Reception.Text += receivedChecksum.ToString("X4");

                        ProcessDecodeMessage();
                    }
                    else
                        Reception.Text += "erreur checksum incorrect";
                    rcvState = StateReception.Waiting;
            break;
                default:
                    rcvState = StateReception.Waiting;
                    break;
            }
        }


    }
}
