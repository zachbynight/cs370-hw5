#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instruction.h"


Instruction parseInstruction(char* line) {
    Instruction newInstruction;
    newInstruction.processID = atoi(strtok(line, ","));
    newInstruction.arrivalTime = atoi(strtok(NULL, ","));
    newInstruction.burstDuration = atoi(strtok(NULL, ","));
    newInstruction.priority = atoi(strtok(NULL, ","));
    return newInstruction;
}

Instruction* parseInstructionPtr(char* line) {
    Instruction* newInstruction = malloc(sizeof(Instruction));
    (*newInstruction).processID = atoi(strtok(line, ","));
    (*newInstruction).arrivalTime = atoi(strtok(NULL, ","));
    (*newInstruction).burstDuration = atoi(strtok(NULL, ","));
    (*newInstruction).priority = atoi(strtok(NULL, ","));
    return newInstruction;
}

char* instructionAsText(Instruction instruction) {
    char* text = malloc(100 * sizeof(char));
    sprintf(text, "Process ID: %d\nArrival time: %d\nBurst duration: %d\nPriority: %d\n", instruction.processID, instruction.arrivalTime, instruction.burstDuration, instruction.priority);
    return text;
}

int compareByArrivalTime(const void *p, const void *q) {
    Instruction *a = (Instruction*) p;
    Instruction *b = (Instruction*) q;
    return (*a).arrivalTime - (*b).arrivalTime;
}

Instruction* sortByArrivalTime(int numInstructions, Instruction* instructions) {
    qsort(instructions, numInstructions, sizeof(Instruction), compareByArrivalTime);
    return instructions;
}