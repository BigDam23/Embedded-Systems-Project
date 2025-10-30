/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Declaration des methodes de House, qui decrit les actions du robot durant la boucle ou maison
*/
#pragma once

#include <avr/io.h>
#include "robotMotricity.h"
#include "lineDetector.h"
#include "stateMachine.h"
#include "infraredCaptor.h"
#include "movement.h"
#include "constants.h"

enum class StageHouse {BEGINING, FIRST_TURN, INTERSECT, SECOND_TURN, LAST_TURN, END};

void executeHouse(LineDetector& lineDetector, Motors& motors, InfraredCaptor& infraredCaptor);

void nextStage(StageHouse& etape, LineDetector& lineDetector, Motors& motors);

void advanceInIntersect(LineDetector& lineDetector, Motors& motors, InfraredCaptor& infraredCaptor, Led& led);