#include <stdio.h>
#include <string.h> /* String library that has many common string functions like strcpy */
#include <stdlib.h> /* Standard library that has allocation functions like malloc */
#include <sys/stat.h> /* Stat library for the struct stat */
#include <fcntl.h> /* File control library that provides symbolic constants and function for file descriptors */
#include <unistd.h> /* Unix standard library providing system calls */

#define MAX_CHARS   1024
#define MAX_MATCHED 1024

int my_getline(char *, unsigned, FILE *);
int is_valid_file(char *);
int my_grep(char *, char *);

static char *matched_lines[MAX_MATCHED];
static char **matched_it = matched_lines;

int main (int argc, char *argv[]) {
    if (argc == 1) {
        perror("Error: No pattern or file\n");
        return 1;    
    } else if (argc == 2) {
        perror("Error: No files provided\n");   
        return 1;
    } 

    memset(matched_lines, 0, sizeof(matched_lines));
    
    for (int i = 2; i < argc; i++) {
        my_grep(*(argv + i), *(argv + 1)); 
    }  
        
    for (char **matched_ptr = matched_lines; *matched_ptr != NULL; matched_ptr++) {
        printf("%s", *matched_ptr);    
    }
    
    return 0;
}

int my_grep(char *file, char *pattern) {
    size_t size = MAX_CHARS;
    char *buf = malloc(size); 

    if (buf == NULL) {
        perror("Error: Could not allocate memory\n");    
        return 0;
    }
    
    if (!is_valid_file(file)) {
        fprintf(stderr, "Error: Could not access the file %s\n", file);
        return 0;
    }
    
    int fd;
    if ((fd = open(file, O_RDONLY, 0)) >= 0) {
        int len;
        FILE *file_stream = fdopen(fd, "r");
        if (file_stream == NULL) {
            fprintf(stderr, "Error: Could not associate file descriptor with FILE * %s\n", file);    
            close(fd);
        }
        while ((len = my_getline(buf, size, file_stream)) > 0) {
            if (len > size) size = len;
            char *buf_ptr = buf;
            while (*(buf_ptr++) != '\0') {
                if (*(buf_ptr) == *(pattern)) {
                    char *curr_pattern = pattern;
                    char *curr_line = buf_ptr;

                    while (*++curr_line != '\0' && *++curr_pattern != '\0' && *curr_line == *curr_pattern)
                        ;
                    
                    if (*curr_pattern == '\0') {
                        *matched_it = malloc(len + 1);  
                        strcpy(*matched_it, buf); 
                        matched_it++;
                        *matched_it = NULL;
                        break;
                    }
                }    
            }
        } 
        fclose(file_stream);
    }
    
    if (fd < 0) {
        fprintf(stderr, "Error: Could not open the file %s\n", file);
        return 0;
    }    

    free(buf);
    close(fd);
    return 1;
}

int my_getline(char *buf, unsigned size, FILE *file_stream) {
    unsigned pos = 0;
    int c;
    while ((c = fgetc(file_stream)) != EOF && pos < size - 1) {
        buf[pos++] = c;
        if (c == '\n') break;
    }

    if (pos == 0 && c == EOF) {
        return -1;
    }
    buf[pos] = '\0';
    return pos;
}

int is_valid_file(char *file) {
    struct stat stbuf;
    
    if (stat(file, &stbuf) == -1) {
        fprintf(stderr, "Error: The file could not be found %s\n", file);
        return 0;
    }
    if ((stbuf.st_mode & S_IFMT) != S_IFREG) {
        fprintf(stderr, "Error: The path provided does not lead to a file %s\n", file);    
        return 0;
    }
    return 1;
}
