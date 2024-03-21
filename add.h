#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096

int store_file(const char *file_path, const char *hash);

char* calculateHash(FILE* file);

int add(int fileCount, char* argv[]);


