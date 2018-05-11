#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0
static int N = 0;
static int M = 0;
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
    M = atoi(validity);
    #if DEBUG
    if(validity == NULL)
    {
        printf("M is not detected...\n");
    }
    #endif
}
int main(int argc, char *argv[])
{

    Validate_And_Extract(argc, argv);
    printf("M = %d\n", M);
    printf("N = %d\n",N);
    #if DEBUG
    printf("%d\n",argc);
    #endif
    return 0;
}
