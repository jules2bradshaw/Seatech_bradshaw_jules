

#ifndef PWM_H
#define	PWM_H
void InitPWM(void);
void PWMSetSpeed(float vitesseEnPourcents, unsigned char motorNumber);
#define MOTEUR_DROIT 0
#define MOTEUR_GAUCHE 1

#endif	/* PWM_H */

