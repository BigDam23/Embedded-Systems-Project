#include <avr/io.h>

#include "executeInstruction.h"
#include "../lib/memoire_24.h"


void readAndExecute(ExecuteInstruction &executeInstruction)
{
    DEBUG_INIT;

    Memoire24CXXX memory;

    uint8_t instruction = 0;
    uint8_t operand = 0;

    uint16_t fileSize = 0;

    memory.lecture(0, &operand);
    fileSize = operand << 8u;
    memory.lecture(1, &operand);
    fileSize |= operand;

    executeInstruction.getAdress();
    
    for (executeInstruction.setAdress(2); executeInstruction.getAdress() < fileSize;)
    {
        memory.lecture(executeInstruction.getAdress(), &instruction);
        memory.lecture(executeInstruction.getAdress() + 1, &operand);
        
        executeInstruction.executeCommand(instruction, operand);
    
        executeInstruction.addToAdress(2); // Mise à jour de l'adresse après l'exécution
    }
}

int main()
{
    ExecuteInstruction executeInstruction;
    readAndExecute(executeInstruction);
}