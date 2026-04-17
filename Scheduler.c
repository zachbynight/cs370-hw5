#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instruction.h"
#include <stdbool.h> 
#include "Scheduler.h"
#include "FCFS.h"
#include "Preemptive.h"
#include "PtrList.h"
#include "string.h"


Report nullReport() {
    Report newReport;
    newReport.completionTime = -1;
    return newReport;
}

bool isReportNull(Report report) {
    return report.completionTime == -1;
}

Report addReports(Report a, Report b) {
    Report newReport;
    newReport.completionTime = a.completionTime + b.completionTime;
    newReport.turnaroundTime = a.turnaroundTime + b.turnaroundTime;
    newReport.waitingTime = a.waitingTime + b.waitingTime;
    newReport.throughput = a.throughput + b.throughput;
    return newReport;
}

Report divideReport(Report a, float b) {
    Report newReport;
    newReport.turnaroundTime = a.turnaroundTime / b;
    //printf("%f / %f = %f\n", a.turnaroundTime, b, newReport.turnaroundTime);
    newReport.waitingTime = a.waitingTime / b;
    return newReport;
}

char* reportAsText(Report report) {
    char* text = malloc(1000 * sizeof(char));
    sprintf(text, "Average Turnaround Time: %.3f\nAverage Waiting Time: %.3f\nThroughput: %.3f\n", report.turnaroundTime, report.waitingTime, report.throughput);
    return text;
}

Report calculateReport(int numReports, Report* reportList, int completionTime) {
    if (numReports == 0) {
        return nullReport();
    }
    if (numReports == 1) {
        return reportList[0];
    }
    Report total;
    total.completionTime = 0.;
    total.turnaroundTime = 0.;
    total.waitingTime = 0.;
    total.throughput = 0.;
    for (int i = 0; i < numReports; i++) {
        total = addReports(total, reportList[i]);
    }
    total = divideReport(total, (float) numReports);
    total.throughput = (float) numReports / (float) completionTime;
    return total;
}


Process* newProcess(int processID, int priority, int arrivalTime, int burstDuration) {
    Process* processPtr = malloc(sizeof(Process));
    processPtr->processID = processID;
    processPtr->priority = priority;
    processPtr->arrivalTime = arrivalTime;
    processPtr->burstDuration = burstDuration;
    processPtr->startTime = -1;
    processPtr->activeStartTime = -1;
    processPtr->completionTime = -1;
    processPtr->progress = 0;
    processPtr->remainingBurst = burstDuration;
    processPtr->previousProgress = 0;
    //printf("Allocating new process %d with pointer %d\n", processPtr->processID, processPtr);
    return processPtr;
}

Process* newProcessFromInstruction(Instruction instruction) {
    return newProcess(instruction.processID, instruction.priority, instruction.arrivalTime, instruction.burstDuration);
}

Process nullProcess() {
    Process process;
    process.arrivalTime = -1;
    return process;
}

bool processIsNull(Process process) {
    return process.arrivalTime == -1;
}

bool processIsComplete(Process process) {
    return process.completionTime != -1;
}

Instruction* readInstructionsFromFile(int* size, char* filename) {
    FILE* filePtr;
    filePtr = fopen(filename, "r");
    if (filePtr == NULL) {
        printf("Error: could not open file %s\n", filename);
        return NULL;
    }
    int numInstructions = 0;
    char line[25];
    Instruction* raw = malloc(125 * sizeof(Instruction));
    while(fscanf(filePtr, "%s", line) != EOF) {
        if (line == "") {
            continue;
        }
        raw[numInstructions] = parseInstruction(line);
        numInstructions++;
    }
    fclose(filePtr);
    *size = numInstructions;
    Instruction* instructions = malloc(numInstructions * sizeof(Instruction));
    for (int i = 0; i < numInstructions; i++) {
        instructions[i] = raw[i];
    }
    return instructions;
}

int main(int argCount, char** args) {
    printf("enter\n");
    if (argCount < 2) {
        printf("Error: must provide filename");
        return 1;
    }
    //printf("read instructions\n");
    int* numProcessesPtr = malloc(sizeof(int));
    Instruction* instructions = readInstructionsFromFile(numProcessesPtr, args[1]);
    //printf("sort instructions\n");
    int numProcesses = *numProcessesPtr;
    //sortByArrivalTime(numProcesses, instructions);
    //printf("run sjfp\n");
    Report report;
    report = runFCFS(instructions, numProcesses);
    if (isReportNull(report)) {
        printf("Error: report could not be generated\n");
    }
    else {
        printf("--- FCFS ---\n%s\n", reportAsText(report));
    }
    report = run(instructions, numProcesses, SJF_MODE);
    if (isReportNull(report)) {
        printf("Error: report could not be generated\n");
    }
    else {
        printf("--- SJFP ---\n%s\n", reportAsText(report));
    }
    report = run(instructions, numProcesses, PRIORITY_MODE);
    if (isReportNull(report)) {
        printf("Error: report could not be generated\n");
    }
    else {
        printf("--- Priority ---\n%s\n", reportAsText(report));
    }
}