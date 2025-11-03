#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h> /* structure returned by stat */
#include <stdlib.h>
#include <dirent.h>

#define BUFFER_SIZE 8192
#define MAX_PATH    1024

int is_valid_dir(char *dir_name);
void *safe_malloc(size_t);
int copy_file(int, int);
struct File_Info *deconstruct_file_path(char *);
void free_file(struct File_Info *file); 
int cp(char *, char *);
int dir_walk(char *, char *);

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
    if (argc == 3) {
        cp(*(argv + 1), *(argv + 2));    
        return 0;
    } 

    int recursive_copy = 0;
    if (argc == 4) {
        if (*(*(argv + 1)) == '-' && *(*(argv + 1) + 1) != '\0' && *(*(argv + 1) + 1) == 'r') {
            // Check that source and destination are both valid directories 
            if (!is_valid_dir(*(argv + 2)) || !is_valid_dir(*(argv + argc - 1))) return 1;
            // Find the path of the source directory, so that we can find hte name of teh folder that we want to recursivley copy
            char *destination_dir = *(argv + argc - 1); 
            dir_walk(*(argv + 2), *(argv + argc - 1));
            return 0;
        }
    }
    
    if (!is_valid_dir(*(argv + argc - 1))) return 1; 
    for (int i = 1; i < argc - 1; i++) {
        cp(*(argv + i), *(argv + argc - 1));
    }
    
    return 0;
}

int dir_walk(char *source,  char *destination) {
    struct dirent *dp;
    DIR *dfd;   
    char name[MAX_PATH];
    
    if ((dfd = opendir(source)) == NULL) {
        fprintf(stderr, "dirwalk: can't open %s\n", source);
        return 1;
    }
    while ((dp = readdir(dfd)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;
        if (strlen(source)+strlen(dp->d_name)+2 > sizeof(name))
            fprintf(stderr, "dirwalk: name %s %s too long\n", source, dp->d_name);
        else {
            sprintf(name, "%s%s", source, dp->d_name);
            printf("source: %s\n", source);
            printf("dp->dname: %s\n", dp->d_name);
            if (is_valid_dir(name)) {
                sprintf(name, "%s/%s/", source, dp->d_name);
                char new_dir[MAX_PATH];
                sprintf(new_dir, "%s/%s/", destination, dp->d_name);
                if (mkdir(new_dir, 0755) != 0) {
                    fprintf(stderr, "Error: Could not create the directory %s\n", name);
                }
                dir_walk(name, new_dir);    
            } else {
                cp(name, destination); 
            }
        }
    }
    closedir(dfd);
    return 0;
}


int copy_file(int target_fd, int source_fd) {
    int n;
    char c[BUFFER_SIZE];
    while((n = read(source_fd, &c, BUFFER_SIZE)) > 0) {
        if (write(target_fd, &c, n) != n) {
            fprintf(stderr, "Error writing to the file\n");
            return 0;
        }
    } 
    return 1;
}

struct File_Info *deconstruct_file_path(char *path) {
    struct File_Info *file = (struct File_Info *)safe_malloc(sizeof(struct File_Info)); 
    int len = strlen(path);

    char *converted = safe_malloc(len + 3);
    converted[0] = '\0';
    strcat(converted, "./");
    strcat(converted, path);
    char *file_name = strrchr(converted, '/') + 1; 
    char *new_file = safe_malloc(len + 3); 
    strcpy(new_file, file_name);

    int dir_len = file_name - converted;
    char *dir = safe_malloc(dir_len+1);  
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
        return 0;
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
            char *full_path_extended = safe_malloc(strlen(destination_file->full_path) + strlen(source_file->file) + 2);
            strcpy(full_path_extended, destination_file->full_path);
            strcat(full_path_extended, "/");
            free(destination_file->full_path);
            free(destination_file->file);
            destination_file->full_path = full_path_extended;
            destination_file->file = NULL;
        }
        
        // Append the source file name to the directory given in the second argument
        destination_file->file = safe_malloc(strlen(source_file->file)+1);
        strcpy(destination_file->file, source_file->file);
        strcat(destination_file->full_path, destination_file->file);

        if (stat(destination_file->full_path, &destination_buf) != -1) {
            if (source_buf.st_dev == destination_buf.st_dev && source_buf.st_ino == destination_buf.st_ino) {
                fprintf(stderr, "Error: Attempted to copy a file to the same location\n");
                return 0;
            }
        }

        if ((target_fd = creat(destination_file->full_path, 0666)) < 0) {
            fprintf(stderr, "Error: Could not create the new file %s\n", destination_file->full_path);
            return 0;
        } 
    } else if ((destination_buf.st_mode & S_IFMT) == S_IFREG) {
        if (source_buf.st_dev == destination_buf.st_dev && source_buf.st_ino == destination_buf.st_ino) {
            fprintf(stderr, "Error: Attempted to copy a file to the same location\n");
            return 0;
        }
        
        if (truncate(destination, 0) == -1) {
            fprintf(stderr, "Error: Could not clear the file to override: %s\n", destination_file->full_path);    
            return 0;
        }  
        
        if ((target_fd = open(destination, O_WRONLY, 0)) < 0) {
            fprintf(stderr, "Error: Could not open the file %s\n", destination_file->full_path);
            return 0;
        }
    } else {
        if (!is_valid_dir(destination_file->directory)) return 0;
        if ((target_fd = creat(destination_file->full_path, 0666)) < 0) {
            fprintf(stderr, "Error: Could not create the new file %s\n", destination_file->full_path);
            return 0;
        } 
    }
    copy_file(target_fd, source_fd);
    free_file(source_file);
    free_file(destination_file);
    close(target_fd);
    close(source_fd);
    return 1;
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

int is_valid_dir(char *dir_name) {
    struct stat destbuf;
    if (stat(dir_name, &destbuf) == -1) {
        return 0;
    }
    
    if ((destbuf.st_mode & S_IFMT) != S_IFDIR) {
        return 0;
    } 
    return 1;
}
