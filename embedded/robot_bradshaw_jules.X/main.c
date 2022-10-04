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
// Configuration des entrées sorties
/****************************************************************************************************/
InitIO();

InitTimer23();
InitTimer1();
InitPWM();

//PWMSetSpeedConsigne(37, MOTEUR_GAUCHE);
/****************************************************************************************************/
// Boucle Principale
/****************************************************************************************************/
while(1){
    //LED_BLANCHE != LED_BLEUE;
} // fin main

}
