#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "../lib/controlLED.h"
#include "../lib/robotMotricity.h"
#include "../lib/RS232.h"
#include "../lib/debug.h"
#include "../lib/music.h"
#include "../lib/memoire_24.h"


class ExecuteInstruction{
public:
    ExecuteInstruction();

    void executeCommand(uint8_t instruction, uint8_t operand);

    void startLoop(uint8_t cpt);

    void endLoop();


    uint8_t getAdress() const;

    void setAdress(uint8_t adress);
    void addToAdress(uint8_t number);

private:
    Led led;
    Motors motors;
    Music music;

    bool isActive = false;
    uint8_t adress = 0;
    uint8_t loopAdress = 0;
    uint8_t loopCounter = 0;
};