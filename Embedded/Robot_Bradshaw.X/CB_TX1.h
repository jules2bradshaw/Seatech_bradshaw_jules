

#ifndef CB_TX1_H
#define	CB_TX1_H

    void SendMessage(unsigned char* message, int length);
    void CB_TX1_Add(unsigned char value);
    void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
    void SendOne();
    unsigned char CB_TX1_IsTransmitting(void);
    int CB_TX1_RemainingSize(void);
    
#endif

#endif	/* CB_TX1_H */

