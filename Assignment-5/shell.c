#include "shell.h"
int get_command(char * buf)
{
    bzero(buf, MAXINPUT);
    fprintf(stdout, "%s ", "?");
    fgets(buf, MAXINPUT, stdin);
    if(!(*buf))
        return -1;
    return 0;
}
int custom_fork()
{
    int ret = fork();
    if(ret  == -1)
    {
        fprintf(stderr, "Fork Error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ret;
}
void run(struct basic_command * command)
{
    int pipe_des[2];
    struct back_command * bk;
    struct execute_command * ex;   
    struct list_command * ls;
    struct piped_command * pc;
    struct redirect_command * rd;

    char * path = getenv("PATH");
    char pathenv[strlen(path) + sizeof("PATH=")];
    sprintf(pathenv, "PATH=%s", path);
    char* envp[] = {pathenv, NULL};
    
    if(!command)
        exit(EXIT_SUCCESS);
    
    switch(command->type)
    {
        default:
            fprintf(stderr, "Type not understood\n");
            exit(EXIT_FAILURE);
            break;
        
        case EXECUTE_COMMAND:
            ex = (struct execute_command*)(command);
            if(ex->commands[0] == 0)
                exit(EXIT_SUCCESS);
            execvpe(ex->commands[0], ex->commands, envp);
            fprintf(stderr, "EXEC FAILED\n");
            exit(EXIT_FAILURE);
            break;
        case REDIRECT_COMMAND:
            rd = (struct redirect_command*)(command);
            close(rd->file_des);
            if(open(rd->redir_file, rd->mode) < 0)
            {
                fprintf(stderr, "%s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            run(rd->command);
            break;
        case LIST_COMMAND:
            ls = (struct list_command*)(command);
            if(custom_fork() == 0)
                run(ls->left_command);
            wait(NULL);
            run(ls->right_command);
            break;
        case PIPED_COMMAND:
            pc = (struct piped_command*)(command);
            if(pipe(pipe_des) < 0)
            {
                fprintf(stderr,"Shell Error, Pipe: %s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            if(custom_fork() == 0)
            {
                close(1);
                dup(pipe_des[1]);
                close(pipe_des[0]);
                close(pipe_des[1]);
                run(pc->left_command);
            }
            if(custom_fork() == 0)
            {
                close(0);
                dup(pipe_des[0]);
                close(pipe_des[0]);
                close(pipe_des[1]);
                run(pc->right_command);
            }
            close(pipe_des[0]);
            close(pipe_des[1]);
            wait(NULL);
            wait(NULL);
            break;
        case BACK_COMMAND:
            bk = (struct back_command*)(command);
            if(custom_fork() == 0)
            {
                run(bk->command);
            }
            break;
    }
    exit(EXIT_SUCCESS);
}
void shell(void)
{
    static char command[MAXINPUT];
    while(get_command(command) >=0)
    {
        struct basic_command * ret = Parse(command);
        if(custom_fork() == 0)
        {
            run(ret);
        }
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}
