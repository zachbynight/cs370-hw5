#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Instruction.h"


typedef struct {
    int completionTime;
    float turnaroundTime;
    float waitingTime;
    float throughput;
} Report;


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
    newReport.waitingTime = a.waitingTime / b;
    return newReport;
}

Report simulateProcess(Instruction instruction, int startTime) {
    Report report;
    report.completionTime = startTime + instruction.burstDuration;
    report.turnaroundTime = (report.completionTime) - instruction.arrivalTime;
    report.waitingTime = startTime - instruction.arrivalTime;
    return report;
}

Report runFCFS(Instruction* instructions, int num) {
    Report totalReport;
    Report currentReport;
    Instruction currentInstruction;
    int currentTime = instructions[0].arrivalTime;

    instructions = sortByArrivalTime(num, instructions);
    for (int i = 0; i < num; i++) {
        currentInstruction = instructions[i];
        currentReport = simulateProcess(currentInstruction, currentTime);
        currentTime = currentReport.completionTime;
        totalReport = addReports(totalReport, currentReport);
    }
    totalReport = divideReport(totalReport, (float) num);
    totalReport.throughput = (float) num / (float) currentTime;
    return totalReport;
}

char* reportAsText(Report report) {
    char* text = malloc(1000 * sizeof(char));
    sprintf(text, "Average turnaround time: %f\nAverage waiting time: %f\nThroughput: %f\n", report.turnaroundTime, report.waitingTime, report.throughput);
    return text;
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
    printf(reportAsText(runFCFS(instructions, argCount - 1)));
}