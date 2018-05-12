#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0
static int N = 0;
static int M = 0;
static char * buf = NULL;

void cleanup()
{
    free(buf);
}
void process_file(FILE * file)
{
    int i,j;
    size_t bytes_read = 0;
    size_t actual_bytes_read = 0;
    while((actual_bytes_read = getline(&buf, &bytes_read, file))!= -1)
    {
        printf("%s", buf);
        for(i=1; i< M; i++)
        {
            if((actual_bytes_read = getline(&buf, &bytes_read, file))== -1)
            {
                #if DEBUG
                printf("Reached EOF\n");
                #endif
                exit(0);
            }
            printf("%s", buf);
        }
        for(j=1; j<N-1; j++)
        {
            getline(&buf, &bytes_read, file);
        }
    }
    // char buf[1024];
    // int i,j;
    // while(customgetLine(buf, 1024, file)!= NO_INPUT)
    // {
    //     printf("%s\n", buf);
    //     for(i=1; i< M; i++)
    //     {
    //         if(customgetLine(buf, 1024, file) == NO_INPUT)
    //         {
    //             exit(0);
    //         }
    //         printf("%s\n", buf);
    //     }
    //     for(j=0; j< N;j++)
    //     {
    //         customgetLine(buf, 1024, file);
    //     }
    // }
}
void Validate_And_Extract(int argc, char * argv[])
{
    if(argc < 3)
    {
        printf("Too few arguments to the program\n");
        printf("Usage: ./every [-N,M] [File-1] [File-2] ....\n");
        exit(-1);
    }
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
        printf("M is not detected...\n");
    }
    else
    {
        M = atoi(validity);
    }

}
int main(int argc, char *argv[])
{
    atexit(cleanup);
    Validate_And_Extract(argc, argv);
    int i;
    for(i=2; i< argc; i++)
    {
        FILE * file = fopen(argv[i], "r");
        process_file(file);

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
