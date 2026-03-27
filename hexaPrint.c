#include <stdio.h>
#include <stdlib.h>

void printHex(const unsigned char *buffer, size_t size, FILE* outputfile) {
    if (outputfile == NULL) {
        printf("Could not open file" "\n");
        return;
    }

    for (size_t i = 0; i < size; i++) {
        fprintf(outputfile, "%02x ", buffer[i]);
    }
    fprintf(outputfile, "\n");
}

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Need to provide one binary file" "\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        printf("Could not open file %s" "\n", argv[1]);
        return 1;
    }

    unsigned char buffer[16];
    size_t bytesRead;
    long position = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        printHex(buffer, bytesRead, stdout);
        position += bytesRead;
    }

    fclose(file);
    return 0;
}