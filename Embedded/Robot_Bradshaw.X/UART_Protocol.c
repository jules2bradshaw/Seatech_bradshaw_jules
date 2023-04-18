#include <xc.h>
#include "CB_TX1.h"
#include "CB_RX11.h"
#include "UART_Protocol.h"

unsigned char UartCalculateChecksum(int msgFunction,
int msgPayloadLength, unsigned char* msgPayload)
{
//Fonction prenant entrée la trame et sa longueur pour calculer le checksum
    unsigned char cks = 0;
            cks ^= 0xFE;
            cks ^= (msgFunction >> 8);
            cks ^= (msgFunction >> 0);
            cks ^= (msgPayloadLength >> 8);
            cks ^= (msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                cks ^= msgPayload[i];
            }
            return cks;
}

void UartEncodeAndSendMessage(int msgFunction,
int msgPayloadLength, unsigned char* msgPayload)
{
//Fonction d?encodage et d?envoi d?un message
    unsigned char message[msgPayloadLength + 6];
            int pos = 0;
            message[pos++] = 0xFE;
            message[pos++] = (msgFunction >> 8);
            message[pos++] = (msgFunction >> 0);
            message[pos++] = (msgPayloadLength >> 8);
            message[pos++] = (msgPayloadLength >> 0);
            for (int i = 0; i < msgPayloadLength; i++)
            {
                message[pos++] = msgPayload[i];

            }

            message[pos++] = UartCalculateChecksum(msgFunction, msgPayloadLength, msgPayload);
            SendMessage(message,msgPayloadLength);
            //SendMessage();

}

//int msgDecodedFunction = 0;
//int msgDecodedPayloadLength = 0;
//unsigned char msgDecodedPayload[128];
//int msgDecodedPayloadIndex = 0;
//
//void UartDecodeMessage(unsigned char c)
//{
////Fonction prenant en entrée un octet et servant à reconstituer les trames
//    
//
//}
//
//void UartProcessDecodedMessage(unsigned char function,
//unsigned char payloadLength, unsigned char* payload)
//{
////Fonction appelée après le décodage pour exécuter l?action
////correspondant au message reçu
//
//}

 

//*************************************************************************/
//Fonctions correspondant aux messages
//*************************************************************************/
