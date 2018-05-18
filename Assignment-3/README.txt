lss:
    This is the lss implementation in C, it takes the given options:
    -L: When showing file information for a symbolic link, show information for the file the link references rather than for the link itself.
    -A: Almost All, list all files in a given directory except "." and ".."
    -a: Lists all files in the specified directoriy(ies).
    These options can be used together like -LAa, -Aa -La -LA or can be used individually as well like -L -A -all
    If no directory argument is given to lss, it assumes the current working directory to be listed
 
 Every:
    Filter that prints every M lines out of N lines
    usage every -N,M [files...]
    if no arg is given, looks for env vars EVERY and take, if not set then  assumes N,M to be 1;

C-Interp
    acts like a c-interpreter
    Runs the c programs and cleans up
    traps trap 0 1 2 3 15