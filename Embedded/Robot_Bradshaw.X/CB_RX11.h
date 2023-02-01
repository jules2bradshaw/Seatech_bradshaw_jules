/* 
 * File:   CB_RX11.h
 * Author: TABLE 6
 *
 * Created on 1 février 2023, 17:01
 */

#ifndef CB_RX11_H
#define	CB_RX11_H

    void CB_RX1_Add(unsigned char value);
    unsigned char CB_RX1_Get(void);
    unsigned char CB_RX1_IsDataAvailable(void);
    void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
    int CB_RX1_GetDataSize(void);
    int CB_RX1_GetRemainingSize(void);

#endif	/* CB_RX11_H */

