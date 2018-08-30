#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main.h"



int execute(char *name, char **argv) {
    int pid = fork();
    if (pid == -1) {
        // Error has occured.
        printf("Error spawning new process.");
        exit(-1);
    } else if (pid == 0) {
        // Child process
        execvp(name, argv);
    } else {
        int child = wait(NULL);
        Usage usage;
        getrusage(RUSAGE_CHILDREN, &usage);
        displayUsage(&usage);
        printf("Finished executing child process %d.\n", child);
        return child;
    }
}

void dispTm(char *prefix, struct timeval time) {
    printf("%s", prefix);

    // Convert times to milliseconds
    long secs = time.tv_sec;
    int micros = time.tv_usec;

    printf("%ld.%06d seconds\n", secs, micros);
}

void displayLine(int len) {
    for (int i = 0; i < len; i++) {
        printf("-");
    }
    printf("\n");
}

void displayUsage(Usage *usage) {
    displayLine(60);

    // Print time elapsed
    struct timeval userTime = usage->ru_utime;
    struct timeval sysTime = usage->ru_stime;
    dispTm("User CPU time:                  ", userTime);
    dispTm("System CPU time:                ", sysTime);

    printf("Times involuntarily preempted:  %ld\n", usage->ru_nivcsw);
    printf("Times voluntarily preempted:    %ld\n", usage->ru_nvcsw);

    // Signals
    // printf("Signals received:   %d\n", )

    // Page faults
    printf("Major page faults:              %ld\n", usage->ru_majflt);
    printf("Minor page faults:              %ld\n", usage->ru_minflt);

    // Maximum resident set size used
    printf("Maximum resident set size used: %ld\n", usage->ru_maxrss);

    displayLine(60);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        char *name = argv[1];
        char **args = &argv[1];
        execute(name, args);
    } 
}