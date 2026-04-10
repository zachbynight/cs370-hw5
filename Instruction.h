#ifndef InstructionDefinition
#define InstructionDefinition

struct Instruction {
    int processID;
    int arrivalTime;
    int burstDuration;
    int priority;
};

#endif


extern struct Instruction parseInstruction();

extern char* instructionAsText();