#ifndef TIMER_H
#define TIMER_H
void InitTimer23(void);
void InitTimer1(float freq);
void InitTimer4(float freq);
void SetFreqTimer1(float freq);
void SetFreqTimer4(float freq);

extern unsigned long timestamp ;
#endif /* TIMER_H */
