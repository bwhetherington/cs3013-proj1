typedef struct rusage Usage;

int execute(char *name, char **argv);

void displayUsage(Usage *usage);

int main(int argc, char **argv);