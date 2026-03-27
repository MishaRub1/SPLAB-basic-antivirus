#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
    } virus;

typedef struct link link;
struct link {
    link *nextVirus;
    virus *vir;
};

typedef enum {
    ENDIAN_LITTLE = 0,
    ENDIAN_BIG = 1
} EndianMode;

static EndianMode g_fileEndian = ENDIAN_LITTLE;
link* SignatureList = NULL;



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

virus* readSignature(FILE* filepointer) {
    virus* v = malloc(sizeof(virus));
    size_t readCount;

    if (v == NULL) {
        return NULL;
    }

    readCount = fread(v, 1, 18, filepointer);
    if (readCount != 18) {
        free(v);                 /* EOF or truncated record */
        return NULL;
    }

    if (g_fileEndian == ENDIAN_BIG) {
        v->SigSize = (unsigned short)((v->SigSize >> 8) | (v->SigSize << 8));
    }

    v->sig = malloc(v->SigSize);
    if (v->sig == NULL) {
        free(v);
        return NULL;
    }

    readCount = fread(v->sig, 1, v->SigSize, filepointer);
    if (readCount != v->SigSize) {
        free(v->sig);
        free(v);
        return NULL;
    }

    return v;
}

void printSignature(virus* v, FILE* outputfile) {
    fprintf(outputfile, "Virus name: %.*s\n", 16, v->virusName);
    fprintf(outputfile, "Virus signature length: %u\n", v->SigSize);
    fprintf(outputfile, "Virus signature:\n");
    printHex(v->sig, v->SigSize, outputfile);
}

link* list_append(link* virus_list, virus* data) {
    link* new_link = malloc(sizeof(link));
    if (new_link == NULL) {
        printf("Failed to allocate memory for new link\n");
        return NULL;
    }
    new_link->vir = data;
    new_link->nextVirus = virus_list;
    return new_link;
}

void list_print(link* virus_list, FILE* outputfile) {
    if (outputfile == NULL) {
        printf("Could not open file" "\n");
        return;
    }
    link* current = virus_list;
    while (current != NULL) {
        printSignature(current->vir, outputfile);
        current = current->nextVirus;
    }
}

void list_free(link* virus_list) {
    link* current = virus_list;
    while (current != NULL) {
        link* next = current->nextVirus;
        free(current->vir->sig);
        free(current->vir);
        free(current);
        current = next;
    }
}

void detectViruses(unsigned char* buffer, size_t size, link* virus_list) {
    for (link* current = virus_list; current != NULL; current = current->nextVirus) {
        if (current->vir->SigSize > size) {
            continue;
        }

        for (size_t i = 0; i + current->vir->SigSize <= size; i++) {
            if (memcmp(buffer + i, current->vir->sig, current->vir->SigSize) == 0) {
                printf("Starting byte at: %zu\n", i);
                printf("Virus name: %.*s\n", 16, current->vir->virusName);
                printf("Virus size: %hu\n\n", current->vir->SigSize);
            }
        }
    }
}

static void PrintSignatures(void) {
    if (SignatureList == NULL) {
        printf("No signatures" "\n");
        return;
    }
    list_print(SignatureList, stdout);
}

static void LoadSignatures(void) {
    char filename[256];
    if (fscanf(stdin, "%255s", filename) != 1) {
        printf("Failed to read file name\n");
        return;
    }
    fgetc(stdin);

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    char magic[4];
    if (fread(magic, 1, 4, file) != 4) {
        printf("Not a valid signature file\n");
        fclose(file);
        return;
    }

    if (strncmp(magic, "VIRL", 4) == 0) {
        g_fileEndian = ENDIAN_LITTLE;
    } else if (strncmp(magic, "VIRB", 4) == 0) {
        g_fileEndian = ENDIAN_BIG;
    } else {
        printf("Not a valid signature file\n");
        fclose(file);
        return;
    }

    if (SignatureList != NULL) {
        list_free(SignatureList);
        SignatureList = NULL;
    }

    while (1) {
        virus* v = readSignature(file);
        if (v == NULL) {
            break;
        }

        link* newHead = list_append(SignatureList, v);
        if (newHead == NULL) {
            free(v->sig);
            free(v);
            printf("Failed to append signature to list\n");
            break;
        }
        SignatureList = newHead;
    }

    fclose(file);
}

static void DetectViruses(void) {
    char filename[256];
    if (fscanf(stdin, "%255s", filename) != 1) {
        printf("Failed to read file name\n");
        return;
    }
    fgetc(stdin);

    if (SignatureList == NULL) {
        printf("No signatures\n");
        return;
    }

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Could not open file %s\n", filename);
        return;
    }

    unsigned char buffer[10000];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    detectViruses(buffer, bytesRead, SignatureList);
}

static void FixFile(void) {
    printf("Not implemented yet\n");
}

static void AIAnalysisOfFile(void) {
    printf("Not implemented yet\n");
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    char input[128];
    int option = 0;

    while (1) {
        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("4) Fix file\n");
        printf("5) AI analysis of file\n");
        printf("6) Quit\n");
        printf("Select one of the above options:\n");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* End input cleanly on EOF/read error. */
            break;
        }

        if (sscanf(input, "%d", &option) != 1) {
            printf("Not within bounds\n");
            continue;
        }

        switch (option) {
        case 1:
            LoadSignatures();
            break;
        case 2:
            PrintSignatures();
            break;
        case 3:
            DetectViruses();
            break;
        case 4:
            FixFile();
            break;
        case 5:
            AIAnalysisOfFile();
            break;
        case 6:
            if (SignatureList != NULL) {
                list_free(SignatureList);
                SignatureList = NULL;
            }
            return 0;
        default:
            printf("Not within bounds\n");
            break;
        }
    }

    return 0;
}