#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h> /* Gives us the stat struct*/
#include <fcntl.h>
#include <stdlib.h> /* Gives us access to malloc */
#include <unistd.h>

#define MAX_SIZE    1024
#define IN          1
#define OUT         2

typedef struct {
    unsigned newline_count;
    unsigned word_count;
    unsigned byte_count;
} Counts;

Counts *my_wc(char *);
void print_count(char *file, Counts *file_counts);
int is_valid_file(char *);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        perror("Error: There are not enough arguments\n");
        return 1;
    }  
    char **file_ptr = argv + 1;
    for (int i = 1; i < argc; file_ptr++, i++) {
        Counts *current_counts;
        if ((current_counts = my_wc(*file_ptr)) == NULL) continue;
        
        print_count(*file_ptr, current_counts);
    }
    
    return 0;
} 

Counts *my_wc(char *file) {
    if (!is_valid_file(file)) return NULL;
    
    int fd;
    if ((fd = open(file, O_RDONLY, 0666)) == -1) {
        fprintf(stderr, "Error: The file could not be opened %s\n", file);    
        return 0;
    }
    
    int n;
    char buf[MAX_SIZE];

    /* Initialize Counts object */
    Counts *file_counts = (Counts *)malloc(sizeof(Counts));
    file_counts->newline_count = 0;
    file_counts->word_count = 0;
    file_counts->byte_count = 0;

    int state = OUT;
    while ((n = read(fd, &buf, MAX_SIZE)) > 0) {
        file_counts->byte_count += n; 
        for (int i = 0; i < n; i++) {
            
            /* State machine to count words */
            if (isspace(*(buf + i))) {
                state = OUT;
            } else if (!isspace(*(buf + i)) && state == OUT) {
                file_counts->word_count += 1;
                state = IN;
            } else if (!isspace(*(buf + i)) && state == IN)
                ;
            
            /* Count new line characters */
            if (*(buf + i) == '\n') {
                file_counts->newline_count += 1;
            }
        }
    } 
    close(fd);
    return file_counts;
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

void print_count(char *file, Counts *file_counts) {
    fprintf(stdout, "  %d %d %d %s\n", file_counts->newline_count, file_counts->word_count,
        file_counts->byte_count, file);
}
