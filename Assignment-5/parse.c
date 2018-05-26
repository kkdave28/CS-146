#include "parse.h"
int get_command(char * buf)
{
    bzero(buf, MAXINPUT);
    fprintf(stdout, "%s ", "?");
    fgets(buf, MAXINPUT, stdin);
    if(!(*buf))
        return -1;
    return 0;
}
struct basic_command * set_execute_command()
{
    struct execute_command * command;
    command = malloc(sizeof(*command));
    bzero(command, sizeof(*command));
    command->type = EXECUTE_COMMAND;
    struct basic_command * ret = (struct basic_command*)(command);
    return ret;
}
struct basic_command * set_redirect_command(struct basic_command * command, char * filename, char * enviornment_file, unsigned char mode, long int file_des)
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
struct basic_command * set_piped_list_command(struct basic_command * left, struct basic_command * right, unsigned char type)
{
    struct piped_command * command;
    command = malloc(sizeof(*command));
    bzero(command, sizeof(*command));
    if(type == 1)
    {
        command->type = LIST_COMMAND;
    }
    else if(type == 2)
    {
        command->type = PIPED_COMMAND;
    }
    else
    {
        fprintf(stderr, "Unknown type\n");
        exit(EXIT_FAILURE);
    }
    command->left_command = left;
    command->right_command = right;
    struct basic_command * ret = (struct basic_command*)(command);
    return ret;
}
struct basic_command * set_back_command(struct basic_command * command)
{
    struct back_command * another;
    another = malloc(sizeof(*another));
    another->type = BACK_COMMAND;
    another->command = command;
    struct basic_command * ret = (struct basic_command*)(another);
    return ret;
}
char tokenize(char ** ps, char * es, char ** q, char ** eq)
{
    char * temp; // s
    char ret;

    temp = *ps;
    while(temp < es && strchr(whitespace, *temp))
    {
        temp++;
    }
    if(q != NULL)
    {
        *q = temp;
    }
    ret = *temp;
    switch(*temp)
    {
        case 0:
            break;
        case '|':
        case '(':
        case ')':
        case ';':
        case '&':
        case '<':
        case '>':
            temp++;
            break;
        default:
            ret = 'a';
            while(temp < es && !strchr(whitespace, *temp) && !strchr(special_symbols, *temp))
                temp++;
            break;
    }
    if(eq != NULL)
        *eq = temp;
    while(temp < es && strchr(whitespace, *temp))
        temp++;
    *ps = temp;
    return ret;
}
int peek(char ** ps, char * es, char * tokens)
{
    char * temp;
    temp = * ps;
    while(temp < es && strchr(whitespace, *temp))
        temp++;
    *ps = temp;
    return *temp && strchr(tokens, *temp);
}
struct basic_command * parse_command(char * command)
{
    char * temp;
    struct basic_command * ret;
    temp = command + strlen(command);
    ret = parse_line_command(&command, temp);
    peek(&command, temp, "");
    if(command != temp)
    {
        fprintf(stderr,"LEFTOVER :%s\n", command);
        fprintf(stderr, "Syntax Error\n");
        exit(EXIT_FAILURE);
    }
    validate_command(ret);
    return ret;
}
struct basic_command * parse_line_command(char **ps, char * es)
{
    struct basic_command * ret;
    ret = parse_piped_command(ps, es);
    while(peek(ps, es, "&"))
    {
        tokenize(ps, es, NULL,NULL);
        ret = set_back_command(ret);
    }
    if(peek(ps,es, ";"))
    {
        tokenize(ps, es, NULL,NULL);
        ret = set_piped_list_command(ret, parse_line_command(ps,es), 1);
    }
    return ret;
}
struct basic_command * parse_piped_command(char **ps, char * es)
{
    struct basic_command * ret;
    ret = parse_exec_command(ps,es);
    if(peek(ps, es, "|"))
    {
        tokenize(ps, es, NULL, NULL);
        ret = set_piped_list_command(ret, parse_piped_command(ps,es), 2);
    }
    return ret;
}
struct basic_command * parse_redirect_command(struct basic_command * command, char ** ps, char * es)
{
    char tokens;
    char *q;
    char * eq;
    while(peek(ps, es, "<>"))
    {
        tokens = tokenize(ps,es, NULL, NULL);
        if(tokenize(ps, es, &q, &eq) != 'a')
        {
            fprintf(stderr , "Missing file for redirection");
            exit(EXIT_FAILURE);
        }
        switch(tokens)
        {
            case '<':
                command = set_redirect_command(command, q, eq, O_RDONLY, 0);
                break;
            case '>':
                command = set_redirect_command(command, q, eq, O_WRONLY|O_CREAT, 1);
                break;
        }
    }
    return command;
}
struct basic_command * parse_block_commands(char ** ps, char * es)
{
    struct basic_command * ret;
    if(!peek(ps,es, "("))
    {
        fprintf(stderr, "Parseblock error\n");
        exit(EXIT_FAILURE);
    }
    tokenize(ps,es,NULL,NULL);
    ret = parse_line_command(ps,es);
    if(!peek(ps, es , ")"))
    {
        fprintf(stderr, "Syntax Error: Missing the \')\'");
        exit(EXIT_FAILURE);
    }
    tokenize(ps, es, NULL, NULL);
    ret = parse_redirect_command(ret, ps, es);
    return ret;
}
struct basic_command * parse_exec_command( char ** ps, char * es)
{
    char * q;
    char * eq;
    char tokens;
    int num_args;
    struct execute_command * command;
    struct basic_command * ret;
    if(peek(ps,es, "("))
    {
        return parse_block_commands(ps, es);
    }
    ret = set_execute_command();
    command = (struct execute_command*)(ret);
    num_args = 0;
    ret = parse_redirect_command(ret, ps, es);
    while(!peek(ps,es, "|)&;"))
    {
        tokens = tokenize(ps,es, &q, &eq);
        if(tokens == 0)
        {
            break;
        }
        if(tokens != 'a')
        {
            fprintf(stderr, "Syntax Error\n");
            exit(EXIT_FAILURE);
        }
        command->commands[num_args] = q;
        command->enviornment_vars[num_args] = eq;
        num_args+=1;
        if(num_args >= MAXCOMMANDS)
        {
            fprintf(stderr, "Exceeded, maximum arguments\n");
            exit(EXIT_FAILURE);
        }
        ret = parse_redirect_command(ret, ps, es);
    }
    command->commands[num_args] = 0;
    command->enviornment_vars[num_args] = 0;
    return ret;
}
struct basic_command * validate_command(struct basic_command * command)
{
    int i;
    struct back_command * bk;
    struct execute_command * ex;   
    struct list_command * ls;
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
        case LIST_COMMAND:
            ls = (struct list_command *)(command);
            validate_command(ls->left_command);
            validate_command(ls->right_command);
            break;
        case BACK_COMMAND:
            bk = (struct back_command*)(command);
            validate_command(bk->command);
            break;
    }
    return command;

}
struct basic_command * Parse()
{
    static char command[MAXINPUT];
    struct basic_command * parsed_command;
    while(get_command(command) >= 0)
    {
        parsed_command = parse_command(command);
    
    }
}