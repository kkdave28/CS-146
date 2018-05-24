#include "parse.h"
char whitespace[] = " \t\r\n\v";
int get_command(char * buf)
{
    bzero(buf, MAXCOMMANDLEN);
    fprintf(stdout, "%s ", "?");
    fgets(buf, MAXCOMMANDLEN, stdin);
    if(buf[0] == 0)
    {
        return -1;
    }
    return 0;
}
struct cmd* parse_pipes(char * command)
{
    char * temp = strtok(command, "|");
    struct cmd* ret;
    ret = malloc(sizeof(*ret));
    ret->count = 0;
    while(temp != NULL)
    {
        strcpy(ret->commands[ret->count++], temp);
        temp = strtok(NULL, "|");
    }
    return ret;
}
int num_tokens(char * arg)
{
    int tokens = 1;
    char * temp = strtok(arg, whitespace);
    while (temp != NULL)
    {
        tokens++;
        temp = strtok(NULL, whitespace);
    }
    return tokens;
}
void tokenize_and_print(struct cmd * all_commands)
{
    printf("%d: ", all_commands->count);
    for(int i=0; i< all_commands->count; i++)
    {
        char * copy = all_commands->commands[i];
        char * temp = strtok(copy, whitespace);
        char print_string[256][256];
        unsigned char redir_flag_in = 0;
        unsigned char redir_flag_out = 0;
        char * redir_in = NULL;
        int count = 0;
        while(temp != NULL)
        {
            strcpy(print_string[count++], temp);
            temp = strtok(NULL, whitespace);
        }
        for(int j=0; j< count; j++)
        {
            if(strcmp("<", print_string[j]) == 0)
            {
                redir_in = print_string[j+1];
                redir_flag_in = 1;
            }
            if(strcmp(">", print_string[j]) == 0)
            {
                redir_flag_out = 1;
            }
            if(print_string[j][0] == '<' && !redir_flag_in)
            {
                redir_flag_in = 1;
                redir_in = print_string[j]+1;
            }
        }
        if(redir_flag_in && i == 0)
            printf(" < \'%s\' ", redir_in);
        for(int j=0; j< count; j++)
        {
            if(strcmp("<", print_string[j]) == 0)
            {
                j+=1;
                continue;
            }
            if(print_string[j][0] == '<')
            {
                continue;
            }
            else if(strcmp(">", print_string[j]) == 0 && i != all_commands->count -1)
            {
                j+=1;
                continue;
            }
            else if(strcmp(">", print_string[j]) == 0)
            {
                printf(" > ");
                continue;
            }
            else
            {
                if(j > 0 )
                    printf(" \'%s\' ", print_string[j]);
                else
                    printf(" \'%s\' ", print_string[j]);
            }
        }
        if(i< all_commands->count-1)
        printf("| ");
    }
    printf("\n");
}
void Parse()
{
    static char command[MAXCOMMANDLEN];
    struct cmd* all_commands;
    while(get_command(command) >=0)
    {
        all_commands = parse_pipes(command);
        tokenize_and_print(all_commands);
        free(all_commands);
    }
}
