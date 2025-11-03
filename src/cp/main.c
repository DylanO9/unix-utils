#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h> /* structure returned by stat */
#include <stdlib.h>

int copy_file(int, int);
struct File_Info *deconstruct_file_path(char *);
void free_file(struct File_Info *file); 
int cp(char *, char *);

struct File_Info {
    char *directory;
    char *file;
    char *full_path;
};

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Error: Not enough files\n");
        return 1;
    }
    
    for (int i = 1; i < argc - 1; i++) {
        cp(*(argv + i), *(argv + argc - 1));
    }
    
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

struct File_Info *deconstruct_file_path(char *path) {
    struct File_Info *file = (struct File_Info *)malloc(sizeof(struct File_Info)); 
    int len = strlen(path);

    char *converted = malloc(len + 3);
    converted[0] = '\0';
    strcat(converted, "./");
    strcat(converted, path);
    char *file_name = strrchr(converted, '/') + 1; 
    char *new_file = malloc(len + 3); 
    strcpy(new_file, file_name);

    int dir_len = file_name - converted;
    char *dir = malloc(dir_len+1);  
    strncpy(dir, converted, dir_len);
    
    file->directory = dir;
    file->file = new_file;
    file->full_path = converted;
    return file;
}

void free_file(struct File_Info *file) {
    free(file->directory);
    free(file->file);
    free(file->full_path);
    free(file);    
}

int cp(char *source, char *destination) {
    int source_fd;
    if ((source_fd = open(source, O_RDONLY, 0)) < 0) {
        fprintf(stderr, "Error: Can not read the file %s\n", source);
        return 1;
    }
    
    struct File_Info *source_file = deconstruct_file_path(source);
    struct File_Info *destination_file = deconstruct_file_path(destination);

    struct stat source_buf;
    struct stat destination_buf;    
    if (stat(source_file->full_path, &source_buf) == -1) {}
    if (stat(destination_file->full_path, &destination_buf) == -1) {}

    
    int len = strlen(destination);
    int target_fd;
    if ((destination_buf.st_mode & S_IFMT) == S_IFDIR) {
        if (destination_file->full_path[strlen(destination_file->full_path) - 1] != '/') {
            char *full_path_extended = malloc(strlen(destination_file->full_path) + strlen(source_file->file) + 2);
            strcpy(full_path_extended, destination_file->full_path);
            strcat(full_path_extended, "/");
            free(destination_file->full_path);
            free(destination_file->file);
            destination_file->full_path = full_path_extended;
            destination_file->file = NULL;
        }
        
        // Append the source file name to the directory given in the second argument
        destination_file->file = malloc(strlen(source_file->file)+1);
        strcpy(destination_file->file, source_file->file);
        strcat(destination_file->full_path, destination_file->file);

        if (stat(destination_file->full_path, &destination_buf) != -1) {
            if (source_buf.st_dev == destination_buf.st_dev && source_buf.st_ino == destination_buf.st_ino) {
                fprintf(stderr, "Error: Attempted to copy a file to the same location\n");
                return 1;
            }
        }

        if ((target_fd = creat(destination_file->full_path, 0666)) < 0) {
            fprintf(stderr, "Error: Could not create the new file %s\n", destination_file->full_path);
            return 1;
        } 
    } else if ((destination_buf.st_mode & S_IFMT) == S_IFREG) {
        if (source_buf.st_dev == destination_buf.st_dev && source_buf.st_ino == destination_buf.st_ino) {
            fprintf(stderr, "Error: Attempted to copy a file to the same location\n");
            return 1;
        }
        
        if (truncate(destination, 0) == -1) {
            fprintf(stderr, "Error: Could not clear the file to override: %s\n", destination_file->full_path);    
            return 1;
        }  
        
        if ((target_fd = open(destination, O_WRONLY, 0)) < 0) {
            fprintf(stderr, "Error: Could not open the file %s\n", destination_file->full_path);
            return 1;
        }
    } else {
        struct stat dirbuf; 
        if (stat(destination_file->directory, &dirbuf) == -1) {
            fprintf(stderr, "Error: Can not access the directory: %s\n", destination_file->directory);
            return 1;
        }
        if ((dirbuf.st_mode & S_IFMT) != S_IFDIR) {
            fprintf(stderr, "Error: There is no directory that matches %s\n", destination_file->directory);
            return 1;
        }
    
        if ((target_fd = creat(destination_file->full_path, 0666)) < 0) {
            fprintf(stderr, "Error: Could not create the new file %s\n", destination_file->full_path);
            return 1;
        } 
    }
    copy_file(target_fd, source_fd);
    free_file(source_file);
    free_file(destination_file);
    close(target_fd);
    close(source_fd);
    return 0;
}
