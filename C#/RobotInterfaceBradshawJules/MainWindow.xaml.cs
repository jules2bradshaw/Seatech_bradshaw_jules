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

            byte[] payload = new byte[] { 45, 54 };
            UartEncodeAndSendMessage(0x0040, payload.Length, payload);
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
            // Reception.Text += "";
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

        public enum typemessage
        {
            Texte=0x0080,
            Leds=0x0020,
            TelemetreIR=0x0030,
            Vitesse=0x0040,
            State=0x0050
        }
        public enum StateRobot
        {
            STATE_ATTENTE = 0,
            STATE_ATTENTE_EN_COURS = 1,
            STATE_AVANCE = 2,
            STATE_AVANCE_EN_COURS = 3,
            STATE_TOURNE_GAUCHE = 4,
            STATE_TOURNE_GAUCHE_EN_COURS = 5,
            STATE_TOURNE_DROITE = 6,
            STATE_TOURNE_DROITE_EN_COURS = 7,
            STATE_TOURNE_SUR_PLACE_GAUCHE = 8,
            STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS = 9,
            STATE_TOURNE_SUR_PLACE_DROITE = 10,
            STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS = 11,
            STATE_ARRET = 12,
            STATE_ARRET_EN_COURS = 13,
            STATE_RECULE = 14,
            STATE_RECULE_EN_COURS = 15
        }


        private void ProcessDecodeMessage(int msgFunction, int msgPayloadLength, byte[] msgPayload)
        {
            switch((typemessage)msgFunction)
            {
                case typemessage.Texte:
                    Reception.Text += "\n Message reçu : " + Encoding.ASCII.GetString(msgPayload);
                    break;
                case typemessage.Leds:
                    if (msgPayload[0] == 1 & msgPayload[1] == 1)
                    {
                        checkboxLed1.IsChecked = true;
                    }
                    else
                    {
                        checkboxLed1.IsChecked = false;
                    }
                    if (msgPayload[0] == 2 & msgPayload[1] == 1)
                    {
                        checkboxLed2.IsChecked = true;
                    }
                    else
                    {
                        checkboxLed2.IsChecked = false;
                    }
                    if (msgPayload[0] == 3 & msgPayload[1] == 1)
                    {
                        checkboxLed3.IsChecked = true;
                    }
                    else
                    {
                        checkboxLed3.IsChecked = false;
                    }
                    break;
                case typemessage.TelemetreIR:
                    for(int i=0; i<3; i++)
                    {
                        CG.Content = "IR_gauche =" + msgDecodedPayload[0];
                        CC.Content = "IR_centre =" + msgDecodedPayload[1];
                        CD.Content = "IR_droit =" + msgDecodedPayload[2];
                    }
                    break;
                case typemessage.Vitesse:
                    {
                        MG.Content = "moteur gauche=" + msgDecodedPayload[0] + "%" ; 
                        MD.Content = "moteur droit=" + msgDecodedPayload[1] + "%" ; 
                    }
                    break;
                case typemessage.State:
                    int instant = (((int)msgPayload[1]) << 24) + (((int)msgPayload[2]) << 16)
                    + (((int)msgPayload[3]) << 8) + ((int)msgPayload[4]);
                    Reception.Text += "\n RobotState :" +
                    ((StateRobot)(msgPayload[0])).ToString() +
                    " - " + instant.ToString() + " ms";
                    break;

            }
        }
        
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

                        ProcessDecodeMessage(msgDecodedFunction, msgDecodedPayloadLength, msgDecodedPayload);
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
