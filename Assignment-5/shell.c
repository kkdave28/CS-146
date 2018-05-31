#include "shell.h"
void child_sigint_handler(int sig)
{
    exit(EXIT_SUCCESS);
}
void parent_sigint_handler(int sig)
{
    // Do nothing in parent process, print newline for better formatting
    printf("?\n");
    
}
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
        fprintf(stderr, "NSH: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return ret;
}
void run(struct basic_command * command)
{
    int pipe_des[2];

    struct execute_command * ex;   
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
            fprintf(stderr, "NSH: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
            break;
        case REDIRECT_COMMAND:
            rd = (struct redirect_command*)(command);
            close(rd->file_des);
            if(open(rd->redir_file, rd->mode,S_IRWXG|S_IRWXU) < 0)
            {
                fprintf(stderr, "%s\n", strerror(errno));
                exit(EXIT_FAILURE);
            }
            run(rd->command);
            break;
        case PIPED_COMMAND:
            pc = (struct piped_command*)(command);
            if(pipe(pipe_des) < 0)
            {
                fprintf(stderr,"NSH: %s\n", strerror(errno));
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
    }
    exit(EXIT_SUCCESS);
}
int is_cd(char * command)
{
    return command[0] == 'c' && command[1] == 'd' && command[2] == ' ';
}
void change_directory(char * dir, char * cwd)
{
    if(chdir(dir) < 0)
    {
        fprintf(stderr, "NSH: %s\n", strerror(errno));
    }
    else
    {
        char cwd_en[1024];
        sprintf(cwd_en, "PWD=%s", cwd);
        putenv(cwd_en);
        printf("Previous Working Directory: %s\n", cwd);
        printf("Current  Working Directory: %s\n", dir);
    }
}
void shell(void)
{
    static char command[MAXINPUT];
    char dwd[1024];
    sprintf(dwd, "DWD=/home/%s", getlogin());
    putenv(dwd);
    signal(SIGINT, &parent_sigint_handler);
    while(get_command(command) >=0)
    {
        if(is_cd(command))
        {
            command[strlen(command)-1] = 0;
            char cwd[1024];
            char home_directory[1024];
            char username [1024];
            if(getcwd(cwd, sizeof(cwd)) == NULL)
                fprintf(stderr, "NSH: %s\n", strerror(errno));
            else
            {
                if(strcmp(command+3, "-p") == 0)
                {
                    change_directory(getenv("PWD"), cwd);
                }
                else if(strcmp(command+3, "-d") == 0)
                {
                    change_directory(getenv("DWD"), cwd);
                }
                else
                {
                    change_directory(command+3, cwd);
                }
            }
            continue;
        }
    
        struct basic_command * ret = Parse(command);
        if(custom_fork() == 0)
        {
            signal(SIGINT, &child_sigint_handler);
            run(ret);
        }

        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}
