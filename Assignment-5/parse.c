#include "parse.h"

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
    if(temp == NULL) // only 1 command
    {
        strcpy(ret->commands[ret->count], command);
        ret->count++;
        //printf("%d: %s\n",1,command);
    }
    //int count = 1;
    while(temp != NULL)
    {
        strcpy(ret->commands[ret->count++], temp);
        //printf("%d: %s\n",count++,temp);
        temp = strtok(NULL, "|");
    }
    return ret;
}
int num_tokens(char * arg)
{
    int tokens = 0;
    char * temp = strtok(arg, whitespace);
    if(temp == NULL)
    {
        return 1;
    }
    
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
        int count = 0;
        if(temp == NULL)
        {
            printf("\"%s\"|", copy);
            continue;
        }
        while(temp != NULL)
        {
            printf("\"%s,\"", temp);
            temp = strtok(NULL, whitespace);
        }
        if(i < all_commands->count -1)
            printf (" |");
    }
    printf("\n");
}
int main(int argc, char const *argv[])
{
    static char command[MAXCOMMANDLEN];
    struct cmd* all_commands;
    while(get_command(command) >=0)
    {
        all_commands = parse_pipes(command);
        // for(int i=0; i< all_commands->count; i++)
        // {
        //     printf("%d: %s\n", i+1, all_commands->commands[i]);
        // }
        tokenize_and_print(all_commands);
        free(all_commands);
    }
    

    return 0;
}
