#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/types.h>
#include "main.h"

// Produces a trimmed copy of the specified string, excluding any trailing
// whitespaces.
char *strTrim(char *str) {
    size_t bufLen = strlen(str) + 1;
    char *buf = (char *) malloc(sizeof (char) * bufLen);

    size_t lastIndex = bufLen - 1;
    while (isspace(str[lastIndex])) {
        lastIndex--;
    }

    strncpy(buf, str, lastIndex - 1);

    return buf;
}

// Determines whether or not the specified string begins with the specified
// prefix.
int strBeginsWith(char *prefix, char *str) {
    return strncmp(prefix, str, strlen(prefix)) == 0;
}

// Produces the current wallclock time as a Time struct.
Time getTime() {
    Time time;
    if (gettimeofday(&time, NULL) == -1) {
        printf("Error getting time.");
        exit(-1);
    }
    return time;
}

// Converts a Time struct into a quantity of microseconds.
long toMicros(Time time) {
    long micros = time.tv_sec * 1000000 + time.tv_usec;
    return micros;
}

// Converts a Time struct into a quantity of milliseconds.
long toMillis(Time time) {
    long micros = toMicros(time);
    long millis = micros / 1000;
    return millis;
}

int execute(char *name, char **argv, int background) {
    // Create child process to run command
    pid_t pid = fork();
    if (pid == -1) {
        // Some error has occurred
        printf("Error spawning new process.\n");
        exit(-1);
    } else if (pid == 0) {
        // Child process
        execvp(name, argv);
        return 0; // To make gcc happy
    } else {
        // Parent process; pid = pid of child
        id_t childPid = -1;
        long start = toMicros(getTime());
        
        int status;
        childPid = wait(&status);

        long stop = toMicros(getTime());
        long millis = (stop - start) / 1000;
        Usage usage;
        getrusage(RUSAGE_CHILDREN, &usage);
        displayUsage(&usage, millis);

        printf("Finished executing child process %s (%d) with status %d.\n", name, childPid, status);
        return status;
    }
}

void dispTm(char *prefix, Time time) {
    printf("%s", prefix);

    // Convert times to milliseconds
    long millis = toMillis(time);

    printf("%ld ms\n", millis);
}

// Displays a line of '-'s of len length.
void displayLine(size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("-");
    }
    printf("\n");
}

// Formats the specified Usage struct into a readable format and displays it.
void displayUsage(Usage *usage, long elapsed) {
    displayLine(60);

    // Print time elapsed
    Time userTime = usage->ru_utime;
    Time sysTime = usage->ru_stime;
    dispTm("User CPU time:                  ", userTime);
    dispTm("System CPU time:                ", sysTime);
    printf("Wallclock time:                 %ld ms\n", elapsed);

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

// Parses arguments from a string and produces a Task struct representing the
// task to perform.
Task parseArgs(char *line) {
    Task task;

    task.argc = 0;
    task.argv = NULL;
    task.background = 0;

    size_t bufLen = 32;
    char **argBuf = (char **) malloc(sizeof (char *) * bufLen);

    size_t i = 0;
    char *token = strtok(line, " \t\n\r");
    
    if (token) {
        argBuf[0] = token;
        i++;
    }

    while (token) {
        token = strtok(NULL, " \t\n\r");
        argBuf[i] = token;
        i++;
    }

    // Examine last token to see if it is '&'
    if (i > 0 && strcmp("&", argBuf[i - 2]) == 0) {
        i--;
        argBuf[i - 1] = NULL;
        task.background = 1;
    }

    task.argc = i;
    task.argv = argBuf;

    return task;
}

// Runs the program specified by the specified Task struct.
void runTask(Task *task) {
    if (task->argc > 0) {
        char *name = task->argv[0];
        char **argv = &task->argv[0];
        execute(name, argv, task->background);
    }
}

// Prints the specified prompt, then produces the first line entered into
// stdin.
char *promptLine(char *prompt) {
    printf("%s", prompt);

    size_t bufSize = 128;
    char *buf = (char *) malloc(sizeof (char) * bufSize);

    size_t len;
    if (getline(&buf, &len, stdin) == -1) {
        // printf("Error reading input.");
        exit(-1);
    } else {
        return buf;
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        char *prompt = "==> ";
        // Use shell mode
        while (1) {
            char *line = promptLine(prompt);
            char *promptPrompt = "set prompt = ";
            if (strBeginsWith(promptPrompt, line)) {
                prompt = strTrim(&line[strlen(promptPrompt)]);
            } else if (strBeginsWith("cd ", line)) {
                char *dir = strTrim(&line[strlen("cd ")]);
                int res = chdir(dir);
                if (res != 0) {
                    printf("Could not change directory to \"%s\".\n", dir);
                }

                free(dir);
            } else {
                char *trimmed = strTrim(line);
                if (strcmp("exit", trimmed) == 0) {
                    printf("Goodbye for now!\n");
                    exit(0);
                }
                free(trimmed);

                Task task = parseArgs(line); 
                runTask(&task);
            }
        }
    } else {
        // Use command mode
        Task task;
        task.argc = argc - 1;
        task.argv = &argv[1];
        task.background = 0;
        runTask(&task);
    }
}