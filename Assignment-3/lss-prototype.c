#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
static char buf[512];
static char buf2[512];
static char directory_name[512];
void print_permissions(struct stat fileStat)
{
    

    //printf("File Permissions: \t");
    if(printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IROTH) ? "r" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-") <0)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    
    
}
int sort_by_size(const void * const X, const void * const Y)
{
    struct stat stat1;
    struct stat stat2;
    sprintf(buf, "%s/%s", directory_name, (*(struct dirent **) X)->d_name);
    sprintf(buf2, "%s/%s", directory_name, (*(struct dirent **) Y)->d_name);
    //(*(struct dirent **) X)->d_name;        
    if(stat(buf, &stat1) == -1)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(stat(buf2, &stat2) == -1)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    return (stat1.st_size < stat2.st_size);
    
}
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "%s: %s\n", "lss", "Too few arguments to the program");
        fprintf(stderr, "%s: %s\n", "Usage", "./lss-c [DIR]");
        exit(EXIT_FAILURE);
    }
    //close(1);
    struct dirent * entry;
    struct dirent ** file_list;
    DIR * dir;
    int file_count;
    int i;
    char modebuf[512];
    char timebuf[512];
    char findbuf[512];
    struct group* grp_query;
    struct passwd* usr_query;
    struct tm* time_query;
    struct stat mystat;
    for(i = 1; i< argc; i++)
    {


        if((dir = opendir(argv[i])) == NULL) // IF the directory doesn't exist or cannot be opened.
        {
            perror("lss");
            exit(EXIT_FAILURE);
        }
        file_count = 0;
        while((entry = readdir(dir)) != NULL)
        {
            file_count++;
        }
        file_list = malloc(file_count*sizeof(*file_list));
        if(file_list == NULL)
        {
            closedir(dir);
            perror("lss");
            exit(EXIT_FAILURE);
        }
        rewinddir(dir);
        file_count = 0;
        while((entry = readdir(dir)) != NULL)
        {
            file_list[file_count++] = entry;
        }
        sprintf(directory_name, "%s", argv[i]);
        qsort(file_list, file_count, sizeof(*file_list), sort_by_size);
        int j;

        if(printf("%s:\n", argv[i])<0)
        {
            perror("lss");
            exit(EXIT_FAILURE);
        }
        for(j=0; j< file_count; j++)
        {

            sprintf(findbuf, "%s/%s", argv[i], file_list[j]->d_name);

            if(stat(findbuf, &mystat) == -1)
            {
                perror("lss");
                exit(EXIT_FAILURE);
            }
            if((grp_query = getgrgid(mystat.st_gid)) == NULL)
            {
                perror("lss");
                exit(EXIT_FAILURE);
            } // check for errors.
            if((usr_query = getpwuid(mystat.st_uid)) == NULL)
            {
                perror("lss");
                exit(EXIT_FAILURE);
            } // check for errors.
            if((time_query = localtime(&mystat.st_atime)) == NULL)
            {
                perror("lss");
                exit(EXIT_FAILURE);
            } // check for errors.
           strftime(timebuf, 20, "%b %d %H:%M", time_query);
           print_permissions(mystat);
           if( printf("%-s\t",modebuf) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
           if( printf("%-ld\t", mystat.st_nlink) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
           if( printf("%-s\t", usr_query->pw_name) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
           if( printf("%-s\t", grp_query->gr_name) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
           if( printf("%-ld\t",mystat.st_size) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
           if( printf("%s\t", timebuf) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
           if( printf(" %-s\t\n", file_list[j]->d_name) < 0)
           {
                perror("lss");
               exit(EXIT_FAILURE);
           }
        }

    if(closedir(dir) == -1)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    printf("%s\n","");
    free(file_list);
    }

    return 0;
}
