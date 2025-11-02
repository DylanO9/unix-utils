#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h> /* structure returned by stat */

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Not enough files\n");
        return 1;
    }
    
    int fd;
    if ((fd = open(*(argv + 1), 0, 0)) < 0) {
        fprintf(stderr, "Open: No such file\n");
        return 1;
    }
    
    char *destination;
    destination = *(argv + 2);

    struct stat stbuf;
    if (stat(destination, &stbuf) == -1) {
        printf("File isn't created\n");
    } 
    if ((stbuf.st_mode & S_IFMT) == S_IFDIR)
        printf("This is a directory\n");
    if ((stbuf.st_mode & S_IFMT) == S_IFREG)
        printf("This is a file already made, that we will override\n");

}
