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
bool v_output = false;

/* Let's assume the user would use the flags which contain a certain value as
 *  pasm -f file.c
 */
int main(int argc, char *argv[]) {

    // Parse the input flags
    enum Flag nextFLag = (enum Flag) NULL;

    for (int i = 0; i < argc; ++i) {
        if (nextFLag == FLAG_FILE) {
            SOURCE_FILE_NAME = argv[i];
            nextFLag = (enum Flag) NULL;
        } else if (strcmp(argv[i], "-f") == 0) {
            nextFLag = FLAG_FILE;
        } else if (strcmp(argv[i], "-v") == 0) {
            v_output = true;
            printv("Verbose mode enabled.\n");
        }
    }

    // Load in the source file
    FILE *source_file = NULL;
    fopen_s(&source_file , SOURCE_FILE_NAME, "r"); // read only

    // Check if the file is open
    if ( !source_file ) {
        printe("Unable to open %s\n", SOURCE_FILE_NAME);
        return 1;
    } else if ( source_file != NULL && v_output ) {
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
