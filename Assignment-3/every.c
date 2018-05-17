#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#define DEBUG 0
//Enviornment variable checking implemented
// use stdin of not given files
static int N = 0;
static int M = 0;
static int start = 0;
void process_file(FILE * file)
{
    int i,j;
    char * buf = NULL;
    size_t bytes_read = 0;
    size_t actual_bytes_read = 0;
    
    while(!feof(file))
    {
        for(i=0; i< M; i++)
        {
            getline(&buf, &bytes_read, file);
            printf("%s", buf);
        }
            //printf("%s", buf);
        if( N >1)
        {
            for(j=0; j<N; j++)
            {
                getline(&buf, &bytes_read, file);
            }
        }
    }
    free(buf);
}
void Validate_And_Extract(int argc, char * argv[])
{
    if(argc < 2)
    {
        printf("Too few arguments to the program\n");
        printf("Usage: ./every [-N,M] [File-1] [File-2] ....\n");
        exit(-1);
    }
    if(access(argv[1], F_OK) != 0)
    {

    
        char * validity = strtok(argv[1], ",");
        if(atoi(validity) > 0)
        {
            printf("Need to append a dash \'-\' in front of the first number\n");
            exit(-1);
        }
        N = atoi(validity)*-1;
        validity = strtok(NULL, ",");
        if(validity == NULL)
        {
            #if DEBUG
            printf("M is not detected...\n");
            #endif
            M=1;
        }
        else
        {
            M = atoi(validity);
        }
        if(M> N)
        {
            printf("M cannot be gretaer than N\nExiting...\n");
            printf("Usage: ./every [-N,M] [File-1] [File-2] ....\n");
            exit(EXIT_FAILURE);
        }
        start = 2;
    }
    else
    {
        char * every = NULL;
        char * take = NULL;
        if((every = getenv("EVERY")) != NULL && (take = getenv("take")) != NULL)
        {
            #if DEBUG
            printf("ENV AR FOUND\n");
            #endif
            N = atoi(every)*-1;
            M = atoi(take);
            start = 1;

            return;
        }
        else
        {
            N =1;
            M = 1;   
            start = 1;
        }
    }

}
int main(int argc, char *argv[])
{
 //   atexit(cleanup);
    Validate_And_Extract(argc, argv);
    
    int i;
    FILE * file;
    #if DEBUG
    printf("M = %d\n", M);
    printf("N = %d\n", N);
    #endif
    for(i=start; i< argc; i++)
    {
        if((file = fopen(argv[i], "r")) == NULL)
        {
            fprintf(stderr,"%s [file: %s]: %s\n", "every", argv[i], strerror(errno));
            exit(EXIT_FAILURE);

        }
        process_file(file);
        fclose(file);

    }
    #if DEBUG
    printf("M = %d\n", M);
    printf("N = %d\n",N);
    #endif
    #if DEBUG
    printf("%d\n",argc);
    #endif
    return 0;
}