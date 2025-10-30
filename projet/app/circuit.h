/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Declaration des methodes du circuit, qui decrit les actions du robot durant le circuit selon une machine a etat
*/
#pragma once

#include <avr/io.h>
#include "robotMotricity.h"
#include "lineDetector.h"
#include "stateMachine.h"
#include "infraredCaptor.h"
#include "music.h"
#include "movement.h"
#include "house.h"
#include "constants.h"

enum class Stages {FORK_1, FORK_2, TOP_RIGHT, HOUSE, BOTTOM_RIGHT, CROSS_1, CROSS_2, TOP_LEFT, LEFT_SIDE};

// Pas le choix d'avoir plus de 4 parametres pour la logique de notre circuit.
void executeCircuit(Stages stage, LineDetector& lineDetector, Motors& motors, StateMachine& stateMachine, InfraredCaptor& infraredCaptor);
// Pas le choix d'avoir plus de 4 parametres pour la logique de notre circuit.
void nextStage(Stages& stage, LineDetector& lineDetector, Motors& motors, Direction direction, InfraredCaptor& infraredCaptor);