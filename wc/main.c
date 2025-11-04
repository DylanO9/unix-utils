#include <stdio.h>
#include <sys/stat.h> /* Gives us the stat struct*/
#include <fcntl.h>

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
        perror("There are not enough arguments\n");
        return 1;
    }  
    char *file = *(argv + 1);
    for (int i = 1; i < argc; file++, i++) {
        Counts *current_counts;
        if ((current_counts = my_wc(file)) == NULL) continue;
        
        print_count(file, current_counts);
    }
    
    return 0;
} 

Counts *my_wc(char *file) {
    if (!is_valid_file(file)) return NULL;
    
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
    fprintf(stdout, "%d\t%d\t%d\t%s", file_counts->newline_count, file_counts->word_count,
        file_counts->byte_count, file);
}
