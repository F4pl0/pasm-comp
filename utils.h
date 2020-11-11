//
// Created by strak on 11/11/2020.
//

#include <stdarg.h>
#include <stdio.h>

#ifndef PASM_COMP_UTILS_H
#define PASM_COMP_UTILS_H

extern bool v_output;

// Verbose printing
void printv(const char* format,...) {

    // Print only in verbose mode
    if (!v_output)
        return;

    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    va_end(args);
}

// Error printing
void printe(const char* format,...) {
    va_list args;
    fprintf(stderr, "Error: ");
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fprintf(stderr, "\n");
}
#endif //PASM_COMP_UTILS_H