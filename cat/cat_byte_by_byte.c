#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <unistd.h>

int print_file(int, char *);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_file(STDIN_FILENO, "Standard Input"); 
        return 1;
    } 

    int fd;    
    while (--argc > 0) {
        if ((fd = open(*++argv, O_RDONLY, 0)) == -1) {
            fprintf(stderr, "There was a problem opening the file: %s\n", *argv); 
            return 1;
        }

        if (!print_file(fd, *argv)) {
            return 1;
        }

        if (close(fd) == -1) { 
            fprintf(stderr, "Failure to close file: %s\n", *argv);
            return 1;
        }
    } 
    return 0;
}

int print_file(int fd, char *file_name) {
    int n;
    char buf;

    int chars_written;
    while ((n = read(fd, &buf, 1)) == 1) {
        chars_written = write(STDOUT_FILENO, &buf, n);

        if (chars_written < n) {
            fprintf(stderr, "Some problem writing for file: %s", file_name);
            return 0;
        }
    }

    return 1;
}
