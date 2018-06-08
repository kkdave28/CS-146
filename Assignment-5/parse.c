#include "parse.h"

struct basic_command * set_execute_command()
{
    struct execute_command * command;
    command = malloc(sizeof(*command));
    bzero(command, sizeof(*command));
    command->type = EXECUTE_COMMAND;
    struct basic_command * ret = (struct basic_command*)(command);
    return ret;
}
struct basic_command * set_redirect_command(struct basic_command * command, char * filename, char * enviornment_file, long int mode, long int file_des)
{
    struct redirect_command * another_command;
    another_command = malloc(sizeof(*another_command));
    bzero(another_command, sizeof(*another_command));
    another_command->type = REDIRECT_COMMAND;
    another_command->command = command;
    another_command->redir_file = filename;
    another_command ->environment_file = enviornment_file;
    another_command->mode = mode;
    another_command->file_des = file_des;

    struct basic_command * ret = (struct basic_command *)(another_command);
    return ret;
}
struct basic_command * set_piped_command(struct basic_command * left, struct basic_command * right)
{
    struct piped_command * command;
    command = malloc(sizeof(*command));
    bzero(command, sizeof(*command));
    command->type = PIPED_COMMAND;
    command->left_command = left;
    command->right_command = right;
    struct basic_command * ret = (struct basic_command*)(command);
    return ret;
}
char * skip_whitespace(char * str1, char * str2)
{
    while(str1 < str2 && strchr(whitespace, *str1))
    {
        str1++;
    }
    return str1;
}
char * skip_whitespace_symbols(char * str1, char * str2)
{
    while(str1 < str2 && !strchr(whitespace, *str1) && !strchr(special_symbols, *str1))
    {
        str1++;
    }
    return str1;
}
char tokenize(char ** unparsed_command, char * final_parsed_command, char ** pased_tokens, char ** set_tokens)
{
    char * temp;
    char ret;

    temp = *unparsed_command;
    temp = skip_whitespace(temp, final_parsed_command);
    if(pased_tokens != NULL)
    {
        *pased_tokens = temp;
    }
    ret = *temp;
    if(*temp != 0)
    {
        if(*temp == '|' || *temp == '<')
        {
            temp ++;
        }
        else if(*temp == '>')
        {
            temp++;
            if(*temp == '>')
            {
                ret = '+';
                temp++;
            }
        }
        else
        {
            ret = 'a';
            temp = skip_whitespace_symbols(temp, final_parsed_command);
        }
    }
    if(set_tokens != NULL)
        *set_tokens = temp;
    temp = skip_whitespace(temp, final_parsed_command);

    *unparsed_command = temp;
    return ret;
}
int scan(char ** unparsed_command, char * final_parsed_command, char * tokens)
{
    char * temp;
    temp = * unparsed_command;
    temp = skip_whitespace(temp, final_parsed_command);
    *unparsed_command = temp;
    return *temp && strchr(tokens, *temp);
}
struct basic_command * parse_command(char * command)
{
    char * temp;
    struct basic_command * ret;
    temp = command + strlen(command);
    ret = parse_piped_command(&command, temp);
    scan(&command, temp, "");
    if(command != temp)
    {
        fprintf(stderr,"LEFTOVER :%s\n", command);
        fprintf(stderr, "Syntax Error\n");
        exit(EXIT_FAILURE);
    }
    validate_command(ret);
    return ret;
}
struct basic_command * parse_piped_command(char **unparsed_command, char * final_parsed_command)
{
    struct basic_command * ret;
    ret = parse_exec_command(unparsed_command,final_parsed_command);
    if(scan(unparsed_command, final_parsed_command, "|"))
    {
        tokenize(unparsed_command, final_parsed_command, NULL, NULL);
        ret = set_piped_command(ret, parse_piped_command(unparsed_command,final_parsed_command));
    }
    return ret;
}
struct basic_command * parse_redirect_command(struct basic_command * command, char ** unparsed_command, char * final_parsed_command)
{
    char tokens;
    char *pased_tokens;
    char * set_tokens;
    while(scan(unparsed_command, final_parsed_command, "<>"))
    {
        tokens = tokenize(unparsed_command,final_parsed_command, NULL, NULL);
        if(tokenize(unparsed_command, final_parsed_command, &pased_tokens, &set_tokens) != 'a')
        {
            fprintf(stderr , "Missing file for redirection");
            exit(EXIT_FAILURE);
        }
        if(tokens == '<')
        {
            command = set_redirect_command(command, pased_tokens, set_tokens, O_RDONLY, 0);
        }
        else if(tokens == '>')
        {
            command = set_redirect_command(command, pased_tokens, set_tokens, O_WRONLY|O_CREAT|O_TRUNC, 1);
        }
        else if(tokens == '+')
        {
            command = set_redirect_command(command, pased_tokens, set_tokens, O_WRONLY|O_CREAT|O_APPEND ,1);
        }
        else
        {
            fprintf(stderr, "Error parsing redirect command\n");
            exit(EXIT_FAILURE);
        }
    }
    return command;
}
struct basic_command * parse_exec_command( char ** unparsed_command, char * final_parsed_command)
{
    char * pased_tokens;
    char * set_tokens;
    char tokens;
    int num_args;
    struct execute_command * command;
    struct basic_command * ret;
    ret = set_execute_command();
    command = (struct execute_command*)(ret);
    num_args = 0;
    ret = parse_redirect_command(ret, unparsed_command, final_parsed_command);
    while(!scan(unparsed_command,final_parsed_command, "|"))
    {
        tokens = tokenize(unparsed_command,final_parsed_command, &pased_tokens, &set_tokens);
        if(tokens == 0)
        {
            break;
        }
        if(tokens != 'a')
        {
            fprintf(stderr, "Syntax Error\n");
            exit(EXIT_FAILURE);
        }
        command->commands[num_args] = pased_tokens;
        command->enviornment_vars[num_args] = set_tokens;
        num_args+=1;
        if(num_args >= MAXCOMMANDS)
        {
            fprintf(stderr, "Exceeded, maximum arguments\n");
            exit(EXIT_FAILURE);
        }
        ret = parse_redirect_command(ret, unparsed_command, final_parsed_command);
    }
    command->commands[num_args] = 0;
    command->enviornment_vars[num_args] = 0;
    return ret;
}
struct basic_command * validate_command(struct basic_command * command)
{
    int i;
    struct execute_command * ex;
    struct piped_command * pc;
    struct redirect_command * rd;

    if(!command)
        return 0;
    
    switch(command->type)
    {
        case EXECUTE_COMMAND:
            ex = (struct execute_command*)(command);
            for(i=0; ex->commands[i]; i++)
            {
                *ex->enviornment_vars[i] = 0;
            }
            break;
        case REDIRECT_COMMAND:
            rd = (struct redirect_command*)(command);
            validate_command(rd->command);
            *rd->environment_file = 0;
            break;
        case PIPED_COMMAND:
            pc = (struct piped_command *)(command);
            validate_command(pc->left_command);
            validate_command(pc->right_command);
            break;
    }
    return command;

}
struct basic_command * Parse(char * command)
{
    return parse_command(command);   
}