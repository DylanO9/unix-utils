#include <stdio.h>
#include <sys/stat.h> /* Gives us the stat struct*/
#include <fcntl.h>

typedef struct {
    unsigned newline_count;
    unsigned word_count;
    unsigned byte_count;
} Counts;

Counts *my_wc(char *);
int is_valid_file(char *);

int main(int argc, char *argv[]) {
} 

Counts *my_wc(char *file) {
    
}

int is_valid_file(char *file) {
    struct stat stat_buf;
    
    if (stat(file, &stat_buf) == -1) {
        fprintf(stderr, "Error: File could not be found %s\n", file);
        return 0;
    }
    
    if ((stat_buf.st_mode & S_IFMT) != S_IFREG) {
        fprintf(stderr, "Error: Path provided is not a file %s\n", file);
    }
    
    return 1;
}
