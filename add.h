#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096

char* calculateHash(FILE* file);

int add(int fileCount, char* files[]);

int dealWithChangedFile(char* changedFile, char* newHash);



