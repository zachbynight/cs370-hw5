#include "Scheduler.h"
#include "PtrList.h"
#include "Instruction.h"
#include <stdlib.h>
#include <stdio.h>


Report* markComplete(Process* processPtr, int completionTime) {
    //printf("%d (pointer: %d) marked complete at time %d\n", (*processPtr).processID, processPtr, completionTime);
    (*processPtr).progress = (*processPtr).burstDuration;
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
    if  (*((void**) p) == NULL) {
        return 1;
    }
    if  (*((void**) q) == NULL) {
        return -1;
    }
    Process* aPtr = (Process*) *((void**) p);
    Process* bPtr = (Process*) *((void**) q);
    if ((*aPtr).burstDuration != (*bPtr).burstDuration) {
        return (*aPtr).burstDuration - (*bPtr).burstDuration;
    }
    return (*aPtr).arrivalTime - (*bPtr).arrivalTime;
}

void sortProcessPtrList(PtrList* listPtr) {
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

Report runSJFP(int numProcesses, Instruction* instructions) {
    sortInstructions(&instructions, numProcesses);
    for (int i = 0; i < numProcesses; i++) {
        //printf("PID %d with burst %d arrived at time %d\n", instructions[i].processID, instructions[i].burstDuration, instructions[i].arrivalTime);
    }

    PtrList processPtrList = *newPtrList(numProcesses);
    int currentTime = 0;

    Instruction currentInstruction;
    Process* currentProcessPtr = NULL;
    for (int i = 0; i < numProcesses; i++) {
        currentInstruction = instructions[i];
        currentTime = currentInstruction.arrivalTime;
        if (currentProcessPtr != NULL) {
            calculateProgress(currentProcessPtr, currentTime);
        }
        int j = append(&processPtrList, (void*) newProcessFromInstruction(currentInstruction));
        currentProcessPtr = getProcess(&processPtrList, j);
        sortProcessPtrList(&processPtrList);
        if (chooseNext(&processPtrList) != NULL) {
            currentProcessPtr = chooseNext(&processPtrList);
            start(currentProcessPtr, currentTime);
        }
    }

    while (chooseNext(&processPtrList) != NULL) {
        currentProcessPtr = chooseNext(&processPtrList);
        start(currentProcessPtr, currentTime);
        currentTime += (*currentProcessPtr).burstDuration - (*currentProcessPtr).progress;
        calculateProgress(currentProcessPtr, currentTime);
    }

    Report* reportList = malloc(numProcesses * sizeof(Report));
    for (int i = 0; i < numProcesses; i++) {
        currentProcessPtr = getProcess(&processPtrList, i);
        reportList[i] = *((*currentProcessPtr).reportPtr);
        //printf("ID %d: %s", (*currentProcessPtr).processID, reportAsText(reportList[i]));
        //printf("Arrived at %d, started at %d, AST %d, previous progress %d, completed at %d\n\n", (*currentProcessPtr).arrivalTime, (*currentProcessPtr).startTime, (*currentProcessPtr).activeStartTime, (*currentProcessPtr).previousProgress, (*currentProcessPtr).completionTime);
    }
    Report final = calculateReport(numProcesses, reportList, currentTime);

    free(reportList);
    freeList(processPtrList);
    return final;
}