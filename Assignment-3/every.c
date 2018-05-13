#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define DEBUG 0
static int N = 0;
static int M = 0;

void process_file(FILE * file)
{
    int i,j;
    char * buf = NULL;
    size_t bytes_read = 0;
    size_t actual_bytes_read = 0;
    while((actual_bytes_read = getline(&buf, &bytes_read, file))!= -1)
    {
        printf("%s", buf);
        for(i=1; i< M; i++)
        {
            getline(&buf, &bytes_read, file);
            printf("%s", buf);
        }
        for(j=1; j<N-1; j++)
        {
            getline(&buf, &bytes_read, file);
        }
    }
    free(buf);
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
 //   atexit(cleanup);
    Validate_And_Extract(argc, argv);
    int i;
    FILE * file;
    for(i=2; i< argc; i++)
    {
        if((file = fopen(argv[i], "r")) == NULL)
        {
            fprintf(stderr,"%s [file: %s]: %s\n", "every", argv[i], strerror(errno));
            exit(EXIT_FAILURE);

        }
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