#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
static char buf[512];
static char buf2[512];
static char directory_name[512];
int A_OPT = 0;
int a_OPT = 0;
int L_OPT = 0;
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
void print_file_info(struct stat mystat, char *filename)
{
    
    char timebuf[512];
    char file_link_buf[512];
    int link_count = 0;
    struct group* grp_query;
    struct passwd* usr_query;
    struct tm* time_query;
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

    if((link_count = readlink(filename, file_link_buf, sizeof(file_link_buf))) >=0)
    {
        file_link_buf[link_count] = '\0';
        printf("%s -> %s\n", filename,file_link_buf);
    }
    else
    {
        printf("%s\n", filename);
    }
    
    
}
int sort_by_size(const void * const X, const void * const Y)
{
    struct stat stat1;
    struct stat stat2;
    sprintf(buf, "%s/%s", directory_name, (*(struct dirent **) X)->d_name);
    sprintf(buf2, "%s/%s", directory_name, (*(struct dirent **) Y)->d_name);
    //(*(struct dirent **) X)->d_name;        
    if(lstat(buf, &stat1) == -1)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    if(lstat(buf2, &stat2) == -1)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    return (stat1.st_size < stat2.st_size);
    
}
void set_options(int argc, char* argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "AaL")) != -1)
    {
        switch(opt)
        {
            case 'A':
                A_OPT = 1;
                break;
            case 'a':
                a_OPT = 1;
                break;
            case 'L':
                L_OPT = 1;
                break;
        }
    }
}
int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        fprintf(stderr, "%s: %s\n", "lss", "Too few arguments to the program");
        fprintf(stderr, "%s: %s\n", "Usage", "./lss-c [DIR]");
        exit(EXIT_FAILURE);
    }
    set_options(argc, argv);
    //printf("A: %d, a: %d, L: %d\n", A_OPT, a_OPT, L_OPT);
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
            if(lstat(argv[i], &mystat) != -1)
            {
                print_file_info(mystat, argv[i]);
                continue;
            }
            else
            {
                //perror("lss");
                continue;
            }
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
            if(A_OPT)
            {
                if(strcmp(".", file_list[j]->d_name) == 0 || strcmp("..", file_list[j]->d_name) == 0)
                {
                    continue;
                }
            }
            if(!a_OPT)
            {
                if(file_list[j]->d_name[0] == '.')
                {
                    continue;
                }
            }
            if(L_OPT)
            {
                if(stat(findbuf, &mystat) == -1)
                {
                    perror("lss");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                if(lstat(findbuf, &mystat) == -1)
                {
                    perror("lss");
                    exit(EXIT_FAILURE);
                }
            }

            print_file_info(mystat, file_list[j]->d_name);
        }

    if(closedir(dir) == -1)
    {
        perror("lss");
        exit(EXIT_FAILURE);
    }
    printf("%s\n","");
    free(file_list);
    file_list = NULL;
    }

    return 0;
}
