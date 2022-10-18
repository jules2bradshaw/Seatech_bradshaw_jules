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
#include "robot.h"

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
            ADCClearConversionFinishedFlag();
            unsigned int *Result = ADCGetResult();
            
            float volts = ((float) Result[2])*3.3 / 4096*3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) Result[1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) Result[0])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;

            ADCValues1 = Result[0];
            if (robotState.distanceTelemetreDroit < 30) LED_ORANGE = 1;
            else LED_ORANGE = 0;

            ADCValues2 = Result[1];
            if (robotState.distanceTelemetreCentre < 30) LED_BLEUE = 1;
            else LED_BLEUE = 0;


            ADCValues3 = Result[2];
            if (robotState.distanceTelemetreGauche < 30) LED_BLANCHE = 1;
            else LED_BLANCHE = 0;



        }
    } // fin main

}
