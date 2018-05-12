#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <bsd/string.h>
#include <time.h>
int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "%s: %s\n", "lss", "Too few arguments to the program");
        fprintf(stderr, "%s: %s\n", "Usage", "./lss-c [DIR]");
        exit(EXIT_FAILURE);
    }
    DIR *mydir;
    struct dirent *myfile;
    struct stat mystat;
    struct stat filestat;
    struct group* grp_query;
    struct passwd* usr_query;
    struct tm* time_query;
    char buf[512];
    char modebuf[512];
    char timebuf[512];
    if((mydir = opendir(argv[1])) == NULL)
    {
        //perror("lss %s ", argv[1]);
        fprintf(stderr, "%s %s: %s\n", "lss", argv[1], strerror(errno));
        exit(EXIT_FAILURE);
    }
    while((myfile = readdir(mydir)) != NULL)
    {
        strmode(mystat.st_mode, modebuf);
    
        sprintf(buf, "%s/%s", argv[1], myfile->d_name);
        stat(buf, &mystat); // check for errors...
        grp_query = getgrgid(mystat.st_gid); // check for errors.
        usr_query = getpwuid(mystat.st_uid); // check for errors.
        time_query = localtime(&mystat.st_atime); // check for errors.
        strftime(timebuf, 20, "%b %d %H:%M", time_query);
        printf("%-s\t",modebuf);
        printf("%-ld\t", mystat.st_nlink);
        printf("%-s\t", usr_query->pw_name);
        printf("%-s\t", grp_query->gr_name);
        printf("%-ld\t",mystat.st_size);
        printf("%s\t", timebuf);
        printf(" %-s\t\n", myfile->d_name);
        
    }
    closedir(mydir);
}