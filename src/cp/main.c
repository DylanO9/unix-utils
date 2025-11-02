#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h> /* structure returned by stat */
#include <stdlib.h>

int copy_file(int, int);

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough files\n");
        return 1;
    }
    
    int source_fd;
    if ((source_fd = open(*(argv + 1), O_RDONLY, 0)) < 0) {
        fprintf(stderr, "Open: No such file\n");
        return 1;
    }
    
    struct stat stbuf;
    if (stat(*(argv + 2), &stbuf) == -1) {
        fprintf(stderr, "Can not access this directory/file\n");
    }

    int len = strlen(*(argv + 2));
    int target_fd;
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
        char *converted = malloc(len + 2);
        strcpy(converted, *(argv + 2));
        if (*(*(argv + 2) + len - 1) != '/') strcat(converted, "/");

        char *dir = malloc(len + strlen(*(argv + 1)) + 1);
        strcat(dir, converted);
        strcat(dir, *(argv + 1));
        if ((target_fd = creat(dir, 0666)) < 0) {
            fprintf(stderr, "Error creating new file\n");
            return 1;
        } else {
            fprintf(stderr, "Success! Created new file %s\n", *(argv + 1));
        }
    } else if ((stbuf.st_mode & S_IFMT) == S_IFREG) {
        fprintf(stderr, "This is a file that is already created, so we will have to override\n");

        if (truncate(*(argv + 2), 0) == -1) {
            fprintf(stderr, "There was an error clearing the file\n");    
            return 1;
        }  
        
        if ((target_fd = open(*(argv + 2), O_WRONLY, 0)) < 0) {
            fprintf(stderr, "There was an error opening the file\n");
        }
    } else {
        fprintf(stderr, "We have a new file name\n"); 

        char *converted = malloc(len + 3);
        strcat(converted, "./");
        strcat(converted, *(argv + 2));
        char *file_name = strrchr(converted, '/');
        fprintf(stderr, "New File that has not been created yet: %s\n", ++file_name);

        int dir_len = file_name - converted;
        char *dir = malloc(dir_len+1);  
        strncpy(dir, converted, dir_len);
        fprintf(stderr, "Directory %s\n", dir);
        
        struct stat dirbuf; 
        if (stat(dir, &dirbuf) == -1) {
            fprintf(stderr, "Can not access this directory: %s\n", dir);
            return 1;
        }
        if ((dirbuf.st_mode & S_IFMT) != S_IFDIR) {
            fprintf(stderr, "There is no directory that matches: %s\n", dir);
            return 1;
        }
    
        if ((target_fd = creat(*(argv + 2), 0666)) < 0) {
            fprintf(stderr, "There was an error creating the new file: %s\n", file_name);
        } 
    }
    copy_file(target_fd, source_fd);
    close(target_fd);
    close(source_fd);
    return 0;
}


int copy_file(int target_fd, int source_fd) {
    int n;
    char c;
    while((n = read(source_fd, &c, 1)) > 0) {
        if (write(target_fd, &c, n) <= 0) {
            fprintf(stderr, "Error writing to the file\n");
            return 0;
        }
    } 
    return 1;
}
