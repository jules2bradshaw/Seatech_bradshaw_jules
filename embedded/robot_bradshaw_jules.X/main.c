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
#include "main.h"

unsigned int ADCValues1, ADCValues2, ADCValues3;
float vitesse = 30;
float vitessem = 25;
float evitemment = 40;

int main(void) {
    /***************************************************************************************************/
    //Initialisation de l?oscillateur
    /****************************************************************************************************/
    InitOscillator();

    /****************************************************************************************************/
    // Configuration des entrées sorties
    /****************************************************************************************************/
    InitIO();

    //InitTimer23();
    InitTimer1(250);
    InitTimer4(1000);
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

            float volts = ((float) Result[0])*3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtDroit = 34 / volts - 5;
            volts = ((float) Result[1])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreDroit = 34 / volts - 5;
            volts = ((float) Result[2])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreCentre = 34 / volts - 5;
            volts = ((float) Result[3])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreExtGauche = 34 / volts - 5;
            volts = ((float) Result[4])* 3.3 / 4096 * 3.2;
            robotState.distanceTelemetreGauche = 34 / volts - 5;



            if (robotState.distanceTelemetreExtDroit < 30 || robotState.distanceTelemetreDroit < 30) LED_ORANGE = 1;
            else LED_ORANGE = 0;

            if (robotState.distanceTelemetreCentre < 30) LED_BLEUE = 1;
            else LED_BLEUE = 0;

            if (robotState.distanceTelemetreExtGauche < 30 || robotState.distanceTelemetreGauche < 30) LED_BLANCHE = 1;
            else LED_BLANCHE = 0;



        }
    }
}
unsigned char stateRobot;

void OperatingSystemLoop(void) {
    switch (stateRobot) {
        case STATE_ATTENTE:
            timestamp = 0;
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_ATTENTE_EN_COURS;

        case STATE_ATTENTE_EN_COURS:
            if (timestamp > 1000)
                stateRobot = STATE_AVANCE;
            break;

        case STATE_AVANCE:
            PWMSetSpeedConsigne(vitessem, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitessem, MOTEUR_GAUCHE);
            stateRobot = STATE_AVANCE_EN_COURS;
            break;
        case STATE_AVANCE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_GAUCHE:
            PWMSetSpeedConsigne(vitessem, MOTEUR_DROIT);
            PWMSetSpeedConsigne(0, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_DROITE:
            PWMSetSpeedConsigne(0, MOTEUR_DROIT);
            PWMSetSpeedConsigne(vitessem, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_GAUCHE:
            PWMSetSpeedConsigne(15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(-15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;

        case STATE_TOURNE_SUR_PLACE_DROITE:
            PWMSetSpeedConsigne(-15, MOTEUR_DROIT);
            PWMSetSpeedConsigne(15, MOTEUR_GAUCHE);
            stateRobot = STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS;
            break;
        case STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS:
            SetNextRobotStateInAutomaticMode();
            break;
        

        default:
            stateRobot = STATE_ATTENTE;
            break;
    }
}

unsigned char nextStateRobot = 0;

void SetNextRobotStateInAutomaticMode() {
    unsigned char positionObstacle = PAS_D_OBSTACLE;

    //Détermination de la position des obstacles en fonction des télémètres
    if (robotState.distanceTelemetreDroit < evitemment && robotState.distanceTelemetreExtDroit < 30 &&
            robotState.distanceTelemetreCentre > evitemment &&
            robotState.distanceTelemetreGauche > evitemment) //11000
        positionObstacle = OBSTACLE_EXT_DROIT;

    else if (robotState.distanceTelemetreDroit < evitemment && robotState.distanceTelemetreExtDroit < 30 &&
            robotState.distanceTelemetreCentre < evitemment &&
            robotState.distanceTelemetreGauche > evitemment) //11100
        positionObstacle = OBSTACLE_A_DROITE;

    else if (robotState.distanceTelemetreDroit > evitemment && robotState.distanceTelemetreExtDroit < 30 && robotState.distanceTelemetreExtGauche > 30 &&
            robotState.distanceTelemetreCentre > evitemment &&
            robotState.distanceTelemetreGauche > evitemment) //10000
        positionObstacle = OBSTACLE_A_DROITE;


    else if (robotState.distanceTelemetreCentre < evitemment || robotState.distanceTelemetreCentre < evitemment && robotState.distanceTelemetreDroit < evitemment && robotState.distanceTelemetreGauche < evitemment ) //00100
        positionObstacle = OBSTACLE_EN_FACE;

    
    else if (robotState.distanceTelemetreDroit > evitemment && robotState.distanceTelemetreExtGauche < 30 &&
            robotState.distanceTelemetreCentre > evitemment &&
            robotState.distanceTelemetreGauche < evitemment) //00011
        positionObstacle = OBSTACLE_EXT_GAUCHE;

    else if (robotState.distanceTelemetreDroit > evitemment && robotState.distanceTelemetreExtGauche < 30 &&
            robotState.distanceTelemetreCentre < evitemment &&
            robotState.distanceTelemetreGauche < evitemment) //00111
        positionObstacle = OBSTACLE_A_GAUCHE;

    else if (robotState.distanceTelemetreDroit > evitemment && robotState.distanceTelemetreExtGauche < 30 &&
            robotState.distanceTelemetreCentre > evitemment &&
            robotState.distanceTelemetreGauche > evitemment) //00001
        positionObstacle = OBSTACLE_A_GAUCHE;

    else if (robotState.distanceTelemetreDroit > evitemment && robotState.distanceTelemetreExtGauche > 30 && robotState.distanceTelemetreExtDroit > 30 &&
            robotState.distanceTelemetreCentre > evitemment &&
            robotState.distanceTelemetreGauche > evitemment) //00000
        positionObstacle = PAS_D_OBSTACLE;

    //Détermination de l?état à venir du robot
    if (positionObstacle == PAS_D_OBSTACLE)
        nextStateRobot = STATE_AVANCE;
    else if (positionObstacle == OBSTACLE_A_DROITE)
        nextStateRobot = STATE_TOURNE_GAUCHE;
    else if (positionObstacle == OBSTACLE_A_GAUCHE)
        nextStateRobot = STATE_TOURNE_DROITE;
    else if (positionObstacle == OBSTACLE_EN_FACE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;
    else if (positionObstacle == OBSTACLE_EXT_GAUCHE)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_DROITE;
    else if (positionObstacle == OBSTACLE_EXT_DROIT)
        nextStateRobot = STATE_TOURNE_SUR_PLACE_GAUCHE;

    //Si l?on n?est pas dans la transition de l?étape en cours
    if (nextStateRobot != stateRobot - 1)
        stateRobot = nextStateRobot;
}