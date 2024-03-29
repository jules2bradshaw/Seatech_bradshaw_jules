/* 
 * File:   main.h
 * Author: Table2
 *
 * Created on 4 octobre 2022, 15:51
 */

#ifndef MAIN_H
#define	MAIN_H

// Configuration des param�tres du Chip
#define FCY 40000000

//D�finition des �tats de la machine � �tat de gestion du robot
#define STATE_ATTENTE 0
#define STATE_ATTENTE_EN_COURS 1
#define STATE_AVANCE 2
#define STATE_AVANCE_EN_COURS 3
#define STATE_TOURNE_GAUCHE 4
#define STATE_TOURNE_GAUCHE_EN_COURS 5
#define STATE_TOURNE_DROITE 6
#define STATE_TOURNE_DROITE_EN_COURS 7
#define STATE_TOURNE_SUR_PLACE_GAUCHE 8
#define STATE_TOURNE_SUR_PLACE_GAUCHE_EN_COURS 9
#define STATE_TOURNE_SUR_PLACE_DROITE 10
#define STATE_TOURNE_SUR_PLACE_DROITE_EN_COURS 11
#define STATE_ARRET 12
#define STATE_ARRET_EN_COURS 13
#define STATE_RECULE 14
#define STATE_RECULE_EN_COURS 15
#define DEMI_TOUR 16
#define DEMI_TOUR_EN_COURS 17

#define PAS_D_OBSTACLE 0
#define OBSTACLE_A_GAUCHE 1
#define OBSTACLE_A_DROITE 2
#define OBSTACLE_EN_FACE 3
#define OBSTACLE_A_EXDROITE 4
#define OBSTACLE_A_EXGAUCHE 5
#define OBSTACLE_AUTOUR 6
#define OBSTACLE_COTE 7
void OperatingSystemLoop(void);
void SetNextRobotStateInAutomaticMode(void);


#endif	/* MAIN_H */

