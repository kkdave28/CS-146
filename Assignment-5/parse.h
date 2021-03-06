#ifndef PARSE_H
#define PARSE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#define MAXCOMMANDS         1000
#define MAXINPUT            1000
#define DEBUG_MODE          1

#define EXECUTE_COMMAND     1
#define REDIRECT_COMMAND    2
#define PIPED_COMMAND       3

static const char * whitespace =        " \t\r\n\v";
static const char * special_symbols =   "<|>";
struct basic_command // Basic command hold one of the three types of command. Shell executes this command, rest others are for pretty formatting and parsing.
{
    unsigned char               type;
};
struct execute_command // Command to format executing commands like ls, ps etc.
{
    unsigned char               type;
    char                        *commands[MAXCOMMANDS];
    char                        *enviornment_vars[MAXCOMMANDS];
};
struct redirect_command // Command to format redirection commmands like ls > one, ls < one or ls >> one.
{
    unsigned char               type;
    struct basic_command        *command;
    char                        *redir_file;
    char                        *environment_file;
    long int                    mode;
    long int                    file_des;
};
struct piped_command // Command to hold and organize piped commands.
{
    unsigned char type;
    struct basic_command        *left_command;
    struct basic_command        *right_command;
};

struct basic_command * parse_command(char *);
struct basic_command * set_execute_command();
struct basic_command * set_redirect_command(struct basic_command*, char *, char *, long int, long int);
struct basic_command * set_piped_command(struct basic_command *, struct basic_command *);

char tokenize(char **, char *, char **, char **);
int scan(char **, char *, char *);
struct basic_command * parse_piped_command(char **, char *);
struct basic_command * parse_exec_command(char **, char *);
struct basic_command * validate_command( struct basic_command*);
struct basic_command * parse_redirect_command(struct basic_command *, char **, char *);
struct basic_command * Parse(char *);
#endif //PARSE_H