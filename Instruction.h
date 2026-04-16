#ifndef InstructionDefinition
#define InstructionDefinition
typedef struct {
    int processID;
    int arrivalTime;
    int burstDuration;
    int priority;
} Instruction;
#endif

extern Instruction* parseInstructionPtr();

extern Instruction parseInstruction();

extern char* instructionAsText();

extern Instruction* sortByArrivalTime();