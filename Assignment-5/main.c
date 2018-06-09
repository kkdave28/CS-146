#include "shell.h"

int main(int argc, char const *argv[])
{
    int exit_code = 0;
    if(argc < 2)
        exit_code = shell(stdin);
    else
    {
        FILE * input_file = fopen(argv[1], "r");
        exit_code = shell(input_file);
    }
    return exit_code;
}
