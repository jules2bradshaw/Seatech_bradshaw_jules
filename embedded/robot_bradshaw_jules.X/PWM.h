

#ifndef PWM_H
#define	PWM_H
void InitPWM(void);
//void PWMSetSpeed(float vitesseEnPourcents, unsigned char motorNumber);
void PWMSetSpeedConsigne(float VitesseEnPourcent, unsigned char motor);
void PWMUpdateSpeed(void);
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

#endif	/* PWM_H */

