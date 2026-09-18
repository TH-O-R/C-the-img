#include <stdio.h>
#include "utils.h"

// Skip all comment lines and leading whitespace until a number or other token
void skip_ppm_comments(FILE *f) {

    int c;
    do {
        c = fgetc(f);
    } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');

    // If we hit a comment, skip the whole line
    while (c == '#') {
        // Skip until end of line
        while ((c = fgetc(f)) != '\n' && c != EOF);
        // Skip any whitespace after the comment line
        do {
            c = fgetc(f);
        } while (c == ' ' || c == '\t' || c == '\n' || c == '\r');
    }

    // Put back the first non-comment character for the next fscanf()
    if (c != EOF) ungetc(c, f);
}
