#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 16

void PrintHex(const unsigned char *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n");
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

    unsigned char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        PrintHex(buffer, bytesRead);
    }

    fclose(file);
    return 0;
}
