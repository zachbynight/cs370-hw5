#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instruction.h"
#include <stdbool.h> 
#include "Scheduler.h"
#include "FCFS.h"
#include "SJFP.h"


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
    sprintf(text, "Turnaround time: %f\nWaiting time: %f\nThroughput: %f\n", report.turnaroundTime, report.waitingTime, report.throughput);
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


Process* newProcess(int processID, int arrivalTime, int burstDuration) {
    Process* processPtr = malloc(sizeof(Process));
    (*processPtr).processID = processID;
    (*processPtr).arrivalTime = arrivalTime;
    (*processPtr).burstDuration = burstDuration;
    (*processPtr).startTime = -1;
    (*processPtr).activeStartTime = -1;
    (*processPtr).completionTime = -1;
    (*processPtr).progress = 0;
    (*processPtr).previousProgress = 0;
    //printf("Allocating new process %d with pointer %d\n", (*processPtr).processID, processPtr);
    return processPtr;
}

Process* newProcessFromInstruction(Instruction instruction) {
    return newProcess(instruction.processID, instruction.arrivalTime, instruction.burstDuration);
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


int main(int argCount, char** args) {
    if (argCount == 1) {
        printf("Error: must provide at least one instruction");
        return 1;
    }
    Instruction* instructions = malloc((argCount - 1) * sizeof(Instruction));
    for (int i = 1; i < argCount; i++) {
        instructions[i - 1] = parseInstruction(args[i]);
    }
    Report report = runSJFP(argCount - 1, instructions);
    if (isReportNull(report)) {
        printf("Error: report could not be generated\n");
    }
    else {
        printf("Shortest job first:\n%s\n", reportAsText(report));
    }
    report = runFCFS(instructions, argCount - 1);
    if (isReportNull(report)) {
        printf("Error: report could not be generated\n");
    }
    else {
        printf("First-come, first-serve:\n%s", reportAsText(report));
    }
    free(instructions);
}