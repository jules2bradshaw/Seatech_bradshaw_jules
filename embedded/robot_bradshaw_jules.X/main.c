/* 
 * File:   main.c
 * Author: TABLE 6
 *
 * Created on 26 septembre 2022, 14:51
 */

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "ChipConfig.h"
#include "IO.h"
#include "timer.h"
#include "PWM.h"
#include "ADC.c"
unsigned int Values1,Values2,Values3;
int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();

/****************************************************************************************************/
// Configuration des entrées sorties
/****************************************************************************************************/
InitIO();

InitTimer23();
InitTimer1();
InitPWM();
InitADC1();

//PWMSetSpeedConsigne(37, MOTEUR_GAUCHE);
/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while(1){
    //LED_BLANCHE != LED_BLEUE;
    if (ADCConversionFinishedFlag == 1){
       unsigned int *Result = ADCGetResult();
       Values1 = Result[0];
       Values2 = Result[1];
       Values3 = Result[2];
    }
} // fin main

}
