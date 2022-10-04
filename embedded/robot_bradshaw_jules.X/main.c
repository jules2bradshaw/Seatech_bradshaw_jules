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
#include "ADC.h"

unsigned int ADCValues1, ADCValues2, ADCValues3;

int main(void) {
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
    while (1) {
        //LED_BLANCHE != LED_BLEUE;
        if (ADCIsConversionFinished() == 1) {
            unsigned int *Result = ADCGetResult();
            ADCClearConversionFinishedFlag();
            ADCValues1 = Result[0];
            ADCValues2 = Result[1];
            ADCValues3 = Result[2];
        }
    } // fin main

}
