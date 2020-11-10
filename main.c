#include <stdio.h>
#include <stdbool.h>
#include <string.h>

enum {
    FLAG_FILE = 1
} Flag;

char *FILE_NAME = "";
bool v_output = false;

int main(int argc, char *argv[]) {
    // pasm -c file.c
    enum Flag nextFLag = (enum Flag) NULL;

    for (int i = 0; i < argc; ++i) {
        if (nextFLag == FLAG_FILE) {
            FILE_NAME = argv[i];
            nextFLag = (enum Flag) NULL;
        } else if (strcmp(argv[i], "-f") == 0) {
            nextFLag = FLAG_FILE;
        } else if (strcmp(argv[i], "-v") == 0) {
            v_output = true;
        }
    }

    return 0;
}
