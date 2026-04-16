#include "Scheduler.h"
#include "PtrList.h"
#include "Instruction.h"
#include <stdlib.h>
#include <stdio.h>


Report* markComplete(Process* processPtr, int completionTime) {
    //printf("%d (pointer: %d) marked complete at time %d\n", (*processPtr).processID, processPtr, completionTime);
    (*processPtr).progress = (*processPtr).burstDuration;
    (*processPtr).remainingBurst = 0;
    (*processPtr).completionTime = completionTime;
    Report newReport;
    newReport.completionTime = (float) completionTime;
    newReport.turnaroundTime = (float) (completionTime - (*processPtr).arrivalTime);
    newReport.waitingTime = (float) ((*processPtr).activeStartTime - ((*processPtr).arrivalTime + (*processPtr).previousProgress));
    (*processPtr).reportPtr = malloc(sizeof(Report));
    *((*processPtr).reportPtr) = newReport;
    return (*processPtr).reportPtr;
}

Report* calculateProgress(Process* processPtr, int currentTime) {
    //printf("checking progress on PID %d at time %d\n", (*processPtr).processID, currentTime);
    (*processPtr).previousProgress = (*processPtr).progress;
    int naiveProgress = (currentTime - (*processPtr).activeStartTime) + (*processPtr).previousProgress;
    //printf("naive progress: %d\n", naiveProgress);
    if (naiveProgress > (*processPtr).burstDuration) {
        return markComplete(processPtr, ((*processPtr).activeStartTime + (*processPtr).burstDuration) - (*processPtr).previousProgress);
    }
    if (naiveProgress == (*processPtr).burstDuration) {
        return markComplete(processPtr, currentTime);
    }
    (*processPtr).progress = naiveProgress;
    (*processPtr).remainingBurst = processPtr->burstDuration - (*processPtr).progress;
    return NULL;
}

Process* getProcess(PtrList* listPtr, int i) {
    if (i >= (*listPtr).size) {
        //printf("Error: index %d out of bounds", i);
        return NULL;
    }
    return (Process*) ((*listPtr).list[i]);
}

Process* getProcessPtrFromId(PtrList* listPtr, int processID) {
    for (int i = 0; i < (*listPtr).size; i++) {
        if ((*getProcess(listPtr, i)).processID == processID) {
            return getProcess(listPtr, i);
        }
    }
    return NULL;
}

int compareProcesses(const void *p, const void *q) {
    //printf("comparing %d with %d\n", p, q);
    //printf("%d\n", *(void**) p);
    //printf("%d\n", *(void**) q);
    if  (*((void**) p) == NULL) {
        //printf("p was null, select q\n");
        return 1;
    }
    if  (*((void**) q) == NULL) {
        //printf("q was null, select q\n");
        return -1;
    }
    //printf("neither was null\n");
    Process* aPtr = (Process*) *((void**) p);
    //printf("ptr to a: %d\n", aPtr);
    Process* bPtr = (Process*) *((void**) q);
    //printf("ptr to b: %d\n", bPtr);
    //printf("%d\n", (*aPtr).processID);
    //printf("%d\n", (*bPtr).processID);
    if ((*aPtr).burstDuration != (*bPtr).burstDuration) {
        //printf("select shorter burst duration\n");
        return (*aPtr).remainingBurst - (*bPtr).remainingBurst;
    }
    //printf("select sooner arrival time\n");
    return (*aPtr).arrivalTime - (*bPtr).arrivalTime;
}

void sortProcessPtrList(PtrList* listPtr) {
    //printf("now sorting\n");
    qsort((*listPtr).list, (*listPtr).size, sizeof(void*), compareProcesses);
}

int compareInstructions(const void *p, const void *q) {
    Instruction a = *((Instruction*) p);
    Instruction b = *((Instruction*) q);
    if (a.arrivalTime == b.arrivalTime) {
        return a.burstDuration - b.burstDuration;
    }
    return a.arrivalTime - b.arrivalTime;
}

void sortInstructions(Instruction** instructionsPtr, int num) {
    qsort(*instructionsPtr, num, sizeof(Instruction), compareInstructions);
}

Process* chooseNext(PtrList* listPtr) {
    PtrList list = *listPtr;
    Process* ptrI;
    for (int i = 0; i < list.size; i++) {
        //printf("Pointer %d\n", list.list[i]);
        if (list.list[i] == NULL) {
            //printf("This point is null\n");
            continue;
        }
        ptrI = getProcess(listPtr, i);
        if (processIsComplete(*ptrI)) {
            //printf("This process is complete already\n");
            continue;
        }
        return ptrI;
    }
    return NULL;
}

void start(Process* processPtr, int currentTime) {
    if ((*processPtr).startTime == -1) {
        (*processPtr).startTime = currentTime;
    }
    (*processPtr).activeStartTime = currentTime;
    //printf("Starting %d at %d\n", (*processPtr).processID, currentTime);
}

bool hasIncompleteProcesses(PtrList* processList) {
    //printf("Checking for incomplete processes\n");
    Process* pi;
    for (int i = 0; i < processList->size; i++) {
        pi = getProcess(processList, i);
        if (pi == NULL) {
            continue;
        }
        if (!processIsComplete(*pi)) {
            //printf("PID %d is still incomplete\n", pi->processID);
            return true;
        }
        //printf("PID %d is complete\n", pi->processID);
    }
    return false;
}

// Assumes the list is sorted by arrival time
bool hasNextArrival(Instruction* instructions, int currentTime, int nextIndex, int numProcesses) {
    if (nextIndex == numProcesses) {
        return false;
    }
    if (instructions[nextIndex].arrivalTime > currentTime) {
        //printf("No arrivals\n", currentTime);
        return false;
    }
    //printf("Found one, %d\n", instructions[nextIndex].processID);
    return true;
}

Report runSJFP(Instruction* instructions, int numProcesses) {
    //printf("Sort instructions\n");
    sortInstructions(&instructions, numProcesses);
    int currentTime = -1;
    int nextInstructionIndex = 0;
    Instruction currentInstruction;
    Instruction nextInstructionPtr;
    Process* currentProcess = NULL;
    PtrList* processList = newPtrList(numProcesses);
    //printf("Start simulation\n");
    while ((nextInstructionIndex < numProcesses) || hasIncompleteProcesses(processList)) {
        hasIncompleteProcesses(processList);
        currentTime++;
        //printf("Check for new arrivals at time %d\n", currentTime);
        while (hasNextArrival(instructions, currentTime, nextInstructionIndex, numProcesses)) {
            currentInstruction = instructions[nextInstructionIndex];
            //printf("PID %d arrived at time %d\n", currentInstruction.processID, currentInstruction.arrivalTime);
            void* ptr = newProcessFromInstruction(currentInstruction);
            append(processList, (void*) newProcessFromInstruction(currentInstruction));
            nextInstructionIndex++;
        }
        if (currentProcess != NULL) {
            //printf("Calculate progress for PID %d\n", currentProcess->processID);
            calculateProgress(currentProcess, currentTime);
        }
        //printf("Sort process queue\n");
        sortProcessPtrList(processList);
        if (chooseNext(processList) != NULL) {
            currentProcess = chooseNext(processList);
            //printf("PID %d will execute next\n", currentProcess->processID);
            start(currentProcess, currentTime);
        }
    }
    //printf("Calculate final report\n");
    //printf("gen report\n");
    Report* reportList = malloc(numProcesses * sizeof(Report));
    for (int i = 0; i < numProcesses; i++) {
        currentProcess = getProcess(processList, i);
        reportList[i] = *(currentProcess->reportPtr);
        //printf("ID %d: %s", (*currentProcessPtr).processID, reportAsText(reportList[i]));
        //printf("Arrived at %d, started at %d, AST %d, previous progress %d, completed at %d\n\n", (*currentProcessPtr).arrivalTime, (*currentProcessPtr).startTime, (*currentProcessPtr).activeStartTime, (*currentProcessPtr).previousProgress, (*currentProcessPtr).completionTime);
    }
    Report final = calculateReport(numProcesses, reportList, currentTime);
    free(reportList);
    //freeList(processPtrList);
    return final;
}