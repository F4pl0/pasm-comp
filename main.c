#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pasm.h"
#include "utils.h"

enum {
    FLAG_FILE = 1
} Flag;

char *SOURCE_FILE_NAME = "";
bool set_verbose = false;
bool set_input_file = false;

void printUsage();

/* Let's assume the user would use the flags which contain a certain value as
 *  pasm -f file.c
 */
int main(int argc, char *argv[]) {

    // Parse the input flags
    // TODO Flag for destination file
    enum Flag nextFLag = (enum Flag) NULL;

    for (int i = 0; i < argc; ++i) {
        if (nextFLag == FLAG_FILE) {
            SOURCE_FILE_NAME = argv[i];
            nextFLag = (enum Flag) NULL;
            set_input_file = true;
        } else if (strcmp(argv[i], "-f") == 0) {
            nextFLag = FLAG_FILE;
        } else if (strcmp(argv[i], "-v") == 0) {
            set_verbose = true;
        }
    }

    // Check for input file
    if (!set_input_file) {
        printe("missing source file.");
        printUsage();
        return 1;
    }

    // Load in the source file
    FILE *source_file = NULL;
    fopen_s(&source_file , SOURCE_FILE_NAME, "r"); // read only

    // Check if the file is open
    if ( !source_file ) {
        printe("Unable to open %s\n", SOURCE_FILE_NAME);
        return 1;
    } else {
        printv("Successfully opened %s\n", SOURCE_FILE_NAME);
    }

    // Read the file and store it in temp. buffer
    int CUR_MAX = 4095;
    char *buffer = (char*) malloc(sizeof(char) * CUR_MAX); // allocate buffer.
    int length = 1;

    int ch = getc(source_file);
    buffer[0] = (char)ch;

    while ( ch != EOF ) {
        if(length == CUR_MAX) { // time to expand ?
            CUR_MAX *= 2; // expand to double the current size of anything similar.
            buffer = realloc(buffer, CUR_MAX); // re allocate memory.
        }
        ch = getc(source_file); // read from stream.
        buffer[length] = (char)ch; // stuff in buffer.
        length++;
    }

    pasm_compile(buffer, length);

    free(buffer);
    return 0;
}

void printUsage() {
    printf("Usage: \n");
    printf("pasm [ATTRIBUTE] [VALUE]\n\n");
    printf("ATTRIBUTES\n");
    printf("-f <file>          Source code file to be compiled\n");
    printf("-d <file>          Destination file to write to\n");
    printf("-v                 Toggle verbose output\n\n");
    printf("EXAMPLE\n");
    printf("pasm -f main.c -v -d program.PCA\n");
}