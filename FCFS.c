#include "Scheduler.h"
#include "Instruction.h"


Report simulateProcess(Instruction instruction, int startTime) {
    Report report;
    report.completionTime = startTime + instruction.burstDuration;
    report.turnaroundTime = (report.completionTime) - instruction.arrivalTime;
    report.waitingTime = startTime - instruction.arrivalTime;
    return report;
}

Report runFCFS(Instruction* instructions, int num) {
    Report* reportList = malloc(num * sizeof(Report));
    Instruction currentInstruction;
    int currentTime = instructions[0].arrivalTime;

    instructions = sortByArrivalTime(num, instructions);
    for (int i = 0; i < num; i++) {
        currentInstruction = instructions[i];
        if (currentInstruction.arrivalTime > currentTime) {
            currentTime = currentInstruction.arrivalTime;
        }
        reportList[i] = simulateProcess(currentInstruction, currentTime);
        currentTime = reportList[i].completionTime;
    }
    return calculateReport(num, reportList, currentTime);
}