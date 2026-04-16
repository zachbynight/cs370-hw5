#import "stdbool.h"

#ifndef ReportDefinition
#define ReportDefinition
typedef struct {
    int completionTime;
    float turnaroundTime;
    float waitingTime;
    float throughput;
} Report;
#endif

#ifndef ProcessDefinition
#define ProcessDefinition
typedef struct {
    int processID;
    int arrivalTime;
    int burstDuration;
    // The time this process first started executing
    int startTime;
    // The time this process started executing during the current stage of activity
    // not necessarily = startTime since preemption occurs
    int activeStartTime;
    int progress;
    int remainingBurst;
    int previousProgress;
    int completionTime;
    Report* reportPtr;
} Process;
#endif

extern Report addReports();

extern Report divideReport(Report report, float divisor);

extern Report nullReport();

extern bool isReportNull();

extern Report calculateReport();

extern char* reportAsText();


extern Process* newProcess();

extern Process* newProcessFromInstruction();

extern Process nullProcess();

extern bool processIsNull();

extern bool processIsComplete();