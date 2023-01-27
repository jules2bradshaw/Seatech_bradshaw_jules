using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RobotInterfaceBradshawJules
{
    public class Robot
    {
        Queue<byte> byteListReceived = new Queue<byte>();
        public string receivedText = "" ;
        public float distancetelementreDroit;
        public float distancetelementreCentre;
        public float distancetelementreGauche;
        public Robot()
        {

        }
    }
}
