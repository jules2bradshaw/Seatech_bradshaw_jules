/* 
 * File:   UART.h
 * Author: TABLE 6
 *
 * Created on 27 janvier 2023, 12:07
 */

#ifndef UART_H
#define UART_H
    void InitUART(void);
    void SendMessageDirect(unsigned char* message, int length) ;
    void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void) ;
#endif /* UART_H */



