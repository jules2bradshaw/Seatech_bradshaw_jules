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

int main (void){
/***************************************************************************************************/
//Initialisation de l?oscillateur
/****************************************************************************************************/
InitOscillator();

/****************************************************************************************************/
// Configuration des entr�es sorties
/****************************************************************************************************/
InitIO();

LED_BLANCHE = 1;
LED_BLEUE = 1;
LED_ORANGE = 1;
InitTimer23();
InitTimer1();
InitPWM();
PWMSetSpeed(-20, MOTEUR_GAUCHE);
/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while(1){
    //LED_BLANCHE != LED_BLEUE;
} // fin main

}
