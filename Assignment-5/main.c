#include "shell.h"

int main(int argc, char const *argv[])
{
    if(argc < 2)
        shell(stdin);
    else
    {
        FILE * input_file = argv[1];
    }
    return 0;
}
