/*
Noms : Damien Jean (2386708), William Komeiha (2382445)
       Maxime Grégoire (2385202), Sacha Turgeon (2373772)
Section : 01

Declaration des methodes de movement, qui decrit les actions du robot durant le circuit concernant la grille ainsi
que les mouvements generaux comme s'aligner, tourner jusqu'a une ligne, etc.
*/
#include <avr/io.h>
#include "robotMotricity.h"
#include "infraredCaptor.h"
#include "lineDetector.h"
#include "controlLED.h"
#include "constants.h"

void moveWithLineDetector(uint8_t captorValue, Motors& motors);

void alingCenter(LineDetector& lineDetector, Motors& motors);

void turnUntilLine(Direction dir, Motors& motors, LineDetector& lineDetector);

Direction turnUntilLineScan(Direction dir, Motors& motors, LineDetector& lineDetector, InfraredCaptor& infraredCaptor);

void turnUntilLineNoVerif(Direction dir, Motors& motors, LineDetector& lineDetector);

void centrerRobot(Motors& motors);

void centrerRobotShort(Motors& motors);

void centrerRobotWithLine(LineDetector& lineDetector, Motors& motors);

void centrerRobotWithLineShort(LineDetector& lineDetector, Motors& motors);

void confirmationPost(Led& led);

void turnToPosts(Motors& motors, Direction dir);

Direction scanPostsGrille(Motors& motors, InfraredCaptor& infraredCaptor, LineDetector& lineDetector);

bool balayScan(Motors& motors, LineDetector& lineDetector, InfraredCaptor& infraredCaptor);

void advanceInGrille(Motors& motors, LineDetector& lineDetector, InfraredCaptor& infraredCaptor, bool isSecondCross);

void advanceToSidePosts(Motors& motors, Direction dir, LineDetector& lineDetector, bool isSecondCross);

void advanceToMiddlePost(Motors& motors, LineDetector& lineDetector);
