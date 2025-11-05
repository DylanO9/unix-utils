#include <stdio.h>

#define MAX_CHARS   1024

int my_getline(char *);
int is_valid_file(char*);
char **my_grep(char *);

int main (int argc, char *argv[]) {
        
}

char **my_grep(char *file) {
    // Initailize a buffer to read lines into    

    // Open the file using open()
        // Check for errors
    // Now read a line from the file into a buffer
        // If no line exists, then exit
        // If a line exists -> look for the pattern in the line -> if the pattenr exists, print the line and line number
    // Close the file
       
}

int my_getline(char *buf) {
    // getchar until we see an EOF or a newline character
    // Place these characters into the bfufer
    // Finish the line with a null string
}

int is_valid_file(char *file) {

}
