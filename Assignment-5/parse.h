#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <strings.h>
#include <ctype.h>
#define MAXCOMMANDLEN 512
#define MAXCOMMANDS 512
#define MAXSIZE 512

char whitespace[] = " \t\r\n\v";
struct cmd
{
    char commands[MAXCOMMANDS][MAXCOMMANDLEN];
    int count;
};
// Data Structures


// Function prototypes
// struct cmd *parsecmd(char* buf);
// int get_command(char *buf );