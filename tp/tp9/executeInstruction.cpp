#include "executeInstruction.h"

ExecuteInstruction::ExecuteInstruction() 
    : led('A', 0, 1), motors(0, 0), music(), 
      isActive(false), adress(0), loopAdress(0), loopCounter(0) {
    Communication::initialisationUART();
}

void ExecuteInstruction::executeCommand(uint8_t instruction, uint8_t operand) {
    if (!isActive && instruction != 0x01) {
        return;
    }

    Communication::transmissionUART(instruction);
    Communication::transmissionUART(operand);

    if (instruction == 0x01) {
        isActive = true;
    }
    else if (instruction == 0xFF) {
        isActive = false;
        motors.setNoSpeed();
        music.stopSound();
        led.setOff();
    }

    else if (isActive) {
        switch (instruction) {
            case 0x02:
                for (uint8_t i = 0; i < operand; i++) {
                    _delay_ms(25);

                }
                break;
            case 0x44:
                if(operand == 1) {
                    led.setGreen();
                }
                if(operand == 2) {
                    led.setRed();
                }
                break;
            case 0x45:
                led.setOff();
                break;
            case 0x48:
                music.playSound(operand);
                break;
            case 0x09:
                music.stopSound();
                break;
            case 0x60:
            case 0x61:
                motors.setNoSpeed();
                break;
            case 0x62:
                motors.setSameSpeed(operand);
                motors.move(Direction::FRONT);
                break;
            case 0x63:
                motors.setSameSpeed(operand);
                motors.move(Direction::BACK);
                break;
            case 0x64:
                motors.setNoSpeed();
                _delay_ms(750);
                motors.turnRight90();
                break;
            case 0x65:
                motors.setNoSpeed();
                _delay_ms(750);
                motors.turnLeft90();
                break;
            case 0xC0:
                startLoop(operand);
                break;
            case 0xC1:
                endLoop();
                break;
        }
    }
}

void ExecuteInstruction::startLoop(uint8_t operand) {
    loopAdress = adress;
    loopCounter = operand;
}

void ExecuteInstruction::endLoop() {
    if (loopCounter != 0) {
        loopCounter--;
        adress = loopAdress;
    }
}

uint8_t ExecuteInstruction::getAdress() const {
    return adress;
}

void ExecuteInstruction::setAdress(uint8_t adress) {
    ExecuteInstruction::adress = adress;
}

void ExecuteInstruction::addToAdress(uint8_t number) {
    setAdress(getAdress() + number);
}