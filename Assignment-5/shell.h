#include "parse.h"

extern int execvpe(const char *file, char *const argv[], char *const envp[]);
void shell(void);
void run(struct basic_command * command);
int custom_fork(void);
