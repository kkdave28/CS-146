#ifndef PARSE_H
#define PARSE_H
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


struct cmd
{
    char commands[MAXCOMMANDS][MAXCOMMANDLEN];
    int count;
};
struct parsed_cmd
{
    char all_commands[MAXCOMMANDS][MAXCOMMANDLEN];
    int total_count;
    char  redir_in [256];
    unsigned char redir_in_flag;
    unsigned char redit_out_flag;
};
void shell();
void tokenize_and_print(struct cmd*);
int num_tokens(char *);
struct cmd* parse_pipes(char * );
int get_command(char *);

#endif //PARSE_H
// Data Structures


// Function prototypes
// struct cmd *parsecmd(char* buf);
// int get_command(char *buf );