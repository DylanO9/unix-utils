#include <stdio.h>
#include <sys/stat.h> /* Stat library for the struct stat */
#include <fcntl.h> /* File control library that provides symbolic constants and function for file descriptors */
#include <unistd.h> /* Unix standard library providing system calls */

#define MAX_CHARS   1024

int my_getline(char *);
int is_valid_file(char*);
int my_grep(char **, char *);

int main (int argc, char *argv[]) {
        
}

int my_grep(char matched_lines, char *file) {
    // Initailize a buffer to read lines into    

    // Open the file using open()
        // Check for errors
    // Now read a line from the file into a buffer
        // If no line exists, then exit
        // If a line exists -> look for the pattern in the line -> if the pattenr exists, print the line and line number
    // Close the file
    char *buf = malloc(MAX_CHARS); 
    if (buf == NULL) {
        perror("Error: Could not allocate memory\n");    
        return 0;
    }
    
    if (!is_valid_file(file)) {
        fprintf(stderr, "Error: Could not access the file %s\n", file);
    }
}

int my_getline(char *buf) {
    // getchar until we see an EOF or a newline character
    // Place these characters into the bfufer
    // Finish the line with a null string
}

int is_valid_file(char *file) {
    struct stat stbuf;
    
    if (stat(file, &stbuf) == -1) {
        fprintf(stderr, "Error: The file could not be found %s\n", file);
        return 1;
    }
    if ((stbuf.st_mode & S_IFMT) != S_IFREG) {
        fprintf(stderr, "Error: The path provided does not lead to a file %s\n", file);    
        return 1;
    }
    return 0;
}
