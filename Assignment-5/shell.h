#include "parse.h"

extern int execvpe(const char *file, char *const argv[], char *const envp[]);
extern int putenv(char * string);
void shell(void);
void run(struct basic_command *);
int is_cd(char *);
void change_directory(char *, char *);
int custom_fork(void);