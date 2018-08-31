typedef struct rusage Usage;
typedef struct timeval Time;
typedef struct {
    size_t argc;
    char **argv;
    int background;
} Task;

// Produces a trimmed copy of the specified string, excluding any trailing
// whitespaces.
char *strTrim(char *str);

// Determines whether or not the specified string begins with the specified
// prefix.
int strBeginsWith(char *prefix, char *str);

// Converts a Time struct into a quantity of microseconds.
long toMicros(Time time);

// Converts a Time struct into a quantity of milliseconds.
long toMillis(Time time);

int execute(char *name, char **argv, int background);

// Displays a line of '-'s of len length.
void displayLine(size_t len);

// Formats the specified Usage struct into a readable format and displays it.
void displayUsage(Usage *usage, long elapsed);

// Parses arguments from a string and produces a Task struct representing the
// task to perform.
Task parseArgs(char *line);

// Runs the program specified by the specified Task struct.
void runTask(Task *task);

// Produces the current wallclock time as a Time struct.
Time getTime();

int main(int argc, char **argv);