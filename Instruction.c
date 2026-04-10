#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instruction.h"


struct Instruction parseInstruction(char* line) {
    struct Instruction newInstruction;
    newInstruction.processID = atoi(strtok(line, ","));
    newInstruction.arrivalTime = atoi(strtok(NULL, ","));
    newInstruction.burstDuration = atoi(strtok(NULL, ","));
    newInstruction.priority = atoi(strtok(NULL, ","));
    return newInstruction;
}

char* instructionAsText(struct Instruction instruction) {
    char* text = malloc(100 * sizeof(char));
    sprintf(text, "Process ID: %d\nArrival time: %d\nBurst duration: %d\nPriority: %d\n", instruction.processID, instruction.arrivalTime, instruction.burstDuration, instruction.priority);
    return text;
}