#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instruction.h"


int main(int argCount, char** args) {
    if (argCount == 1) {
        printf("Error: must provide at least one instruction");
        return 1;
    }
    struct Instruction instruction = parseInstruction(args[1]);
    printf(instructionAsText(instruction));
}