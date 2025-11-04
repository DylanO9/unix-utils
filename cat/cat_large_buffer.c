#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXCHARS    1024
int print_file(int, char *);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        print_file(STDIN_FILENO, "Standard Input");
        return 0;
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
    int chars_written;
    char buf[MAXCHARS];
    
    while ((n = read(fd, buf, MAXCHARS)) > 0) {
        chars_written = write(STDOUT_FILENO, buf, n);

        if (chars_written < n) {
            fprintf(stderr, "There was a problem writing to the file: %s\n", file_name);
            return 0;
        }
    }

    return 1;
}
